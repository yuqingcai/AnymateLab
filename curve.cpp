#include "curve.h"
#include "features.h"
#include <QFile>
#include <QCursor>
#include <QRandomGenerator>
#include <QtMath>
#include <QPainter>

#define STD140_ALIGN_INT   4
#define STD140_ALIGN_FLOAT 4
#define STD140_ALIGN_VEC2  8
#define STD140_ALIGN_VEC3 16
#define STD140_ALIGN_VEC4 16
#define STD140_ALIGN_MAT4 64

static QShader getShader(const QString &name)
{
    QFile f(name);
    return f.open(QIODevice::ReadOnly) ?
               QShader::fromSerialized(f.readAll()) : QShader();
}

static size_t align(size_t offset, size_t alignment) {
    return (offset + alignment - 1) & ~(alignment - 1);
}

void CurveRenderer::createVertices()
{
    m_vertices = new float[m_segments * m_vertexAttributeStrip * 2];

    glm::vec2 p0(-100.0,  -100.0);
    glm::vec2 p1(  0.0,  -100.0);
    glm::vec2 p2(  0.0,  100.0);
    glm::vec2 p3( 100.0,  100.0);

    for (int i = 0; i < m_segments; i ++) {

        float t = i / (float)m_segments;
        float delta = 1.0f / m_segments;

        glm::vec2 currentPoint = bezier(t, p0, p1, p2, p3);
        glm::vec2 nextPoint = bezier(std::min(t + delta, 1.0f), p0, p1, p2, p3);

        // 计算切线
        glm::vec2 tangent = glm::normalize((nextPoint - currentPoint));
        // 计算法线
        glm::vec2 normal = glm::vec2(-tangent.y, tangent.x);

        // 计算左右两边的点
        glm::vec2 left = currentPoint + (normal * (float)(m_width / 2.0));
        glm::vec2 right = currentPoint - (normal * (float)(m_width / 2.0));

        m_vertices[i * m_vertexAttributeStrip * 2 + 0] = left.x;
        m_vertices[i * m_vertexAttributeStrip * 2 + 1] = left.y;
        m_vertices[i * m_vertexAttributeStrip * 2 + 2] = right.x;
        m_vertices[i * m_vertexAttributeStrip * 2 + 3] = right.y;
    }
}

void CurveRenderer::deleteVertices()
{
    if (m_vertices) {
        delete [] m_vertices;
    }
}

glm::vec2 CurveRenderer::bezier(float t, const glm::vec2& p0, const glm::vec2& p1,
                                const glm::vec2& p2, const glm::vec2& p3)
{
    float u = 1.0 - t;
    return (float)(u * u * u) * p0 +
           (float)(3.0 * u * u * t) * p1 +
           (float)(3.0 * u * t * t) * p2 +
           (float)(t * t * t) * p3;
}

CurveRenderer::CurveRenderer()
{
    m_models = new glm::mat4[m_graphics];
    for (int i = 0; i < m_graphics; i ++) {
        m_models[i] = glm::mat4(1.0f);
    }

    createVertices();
}

CurveRenderer::~CurveRenderer()
{
    deleteVertices();

    if (m_models) {
        delete [] m_models;
    }
}

int CurveRenderer::createBuffer()
{
    if (!m_rhi)
        return -1;

    size_t verticesSize = sizeof(float) *
                          m_vertexAttributeStrip * m_segments * 2;
    m_vectexBuffer.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                          QRhiBuffer::VertexBuffer,
                                          verticesSize));
    m_vectexBuffer->create();

    m_modelBuffer.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                         QRhiBuffer::VertexBuffer,
                                         m_graphics * sizeof(glm::mat4)));
    m_modelBuffer->create();

    return 0;
}


int CurveRenderer::createPipline()
{
    if (!m_rhi)
        return -1;

    m_pipeline.reset(m_rhi->newGraphicsPipeline());

    m_pipeline->setShaderStages({
        {
            QRhiShaderStage::Vertex,
            getShader(QLatin1String(":/AnymateLab/shaders/curve.vert.qsb"))
        },
        {
            QRhiShaderStage::Fragment,
            getShader(QLatin1String(":/AnymateLab/shaders/curve.frag.qsb"))
        }
    });

    QRhiVertexInputLayout inputLayout;
    inputLayout.setBindings(
        {
         { 2 * sizeof(float), QRhiVertexInputBinding::PerVertex },
         { 64, QRhiVertexInputBinding::PerInstance },
         });

    inputLayout.setAttributes(
        {
            // binding0
            { 0, 0, QRhiVertexInputAttribute::Float2, 0                 },
            // binding1
            { 1, 3, QRhiVertexInputAttribute::Float4, 0 },
            { 1, 4, QRhiVertexInputAttribute::Float4, 4 * sizeof(float) },
            { 1, 5, QRhiVertexInputAttribute::Float4, 8 * sizeof(float) },
            { 1, 6, QRhiVertexInputAttribute::Float4, 12 * sizeof(float) },
         });

    int blockSize = sizeof(glm::mat4) +
                    sizeof(glm::mat4)
        ;
    int bufferSize = m_rhi->ubufAligned(blockSize) * m_uniformBufferBlockCount;
    m_uniformBuffer.reset(m_rhi->newBuffer(QRhiBuffer::Dynamic,
                                           QRhiBuffer::UniformBuffer,
                                           bufferSize));
    m_uniformBuffer->create();

    m_srb.reset(m_rhi->newShaderResourceBindings());
    m_srb->setBindings({
        QRhiShaderResourceBinding::uniformBufferWithDynamicOffset(
            0,
            m_shaderResourceStages,
            m_uniformBuffer.get(),
            bufferSize),
    });
    m_srb->create();

    m_pipeline->setVertexInputLayout(inputLayout);
    m_pipeline->setSampleCount(m_sampleCount);
    m_pipeline->setShaderResourceBindings(m_srb.get());
    m_pipeline->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    m_pipeline->setDepthTest(true);
    m_pipeline->setDepthWrite(true);
    m_pipeline->setTopology(QRhiGraphicsPipeline::TriangleStrip);
    // m_pipeline->setTopology(QRhiGraphicsPipeline::LineStrip);

    QList<QRhiGraphicsPipeline::TargetBlend> targetBlends(1);
    targetBlends[0].enable = true;
    targetBlends[0].srcColor = QRhiGraphicsPipeline::SrcAlpha;
    targetBlends[0].dstColor = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opColor = QRhiGraphicsPipeline::Add;
    targetBlends[0].srcAlpha = QRhiGraphicsPipeline::One;
    targetBlends[0].dstAlpha = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opAlpha = QRhiGraphicsPipeline::Add;
    m_pipeline->setTargetBlends(targetBlends.begin(), targetBlends.end());
    m_pipeline->create();

    return 1;
}


void CurveRenderer::initialize(QRhiCommandBuffer *cb)
{
    if (m_rhi != rhi()) {
        m_rhi = rhi();
        // ShowFreatures(m_rhi);
        createBuffer();
        m_pipeline.reset();
    }

    if (m_sampleCount != renderTarget()->sampleCount()) {
        m_sampleCount = renderTarget()->sampleCount();
        m_pipeline.reset();
    }

    QRhiTexture *finalTex = m_sampleCount > 1 ? resolveTexture() : colorTexture();
    if (m_textureFormat != finalTex->format()) {
        m_textureFormat = finalTex->format();
        m_pipeline.reset();
    }

    if (!m_pipeline) {
        createPipline();
    }


    QRhiResourceUpdateBatch *batch = m_rhi->nextResourceUpdateBatch();

    batch->uploadStaticBuffer(m_vectexBuffer.get(), m_vertices);
    batch->uploadStaticBuffer(m_modelBuffer.get(), m_models);

    cb->resourceUpdate(batch);

}

void CurveRenderer::render(QRhiCommandBuffer *cb)
{
    const QSize outputSize = renderTarget()->pixelSize();
    m_projection = m_rhi->clipSpaceCorrMatrix();

    m_projection.perspective(45.0f,
                             outputSize.width() / (float) outputSize.height(),
                             10.0f,
                             1000000.0f);

    QVector3D cameraPos(m_focus.rx(), m_focus.ry(), m_zoom);
    QVector3D cameraTarget(m_focus.rx(), m_focus.ry(), 0.0f);
    QVector3D cameraUp(0.0f, 1.0f, 0.0f);
    m_view.setToIdentity();
    m_view.lookAt(cameraPos, cameraTarget, cameraUp);

    QRhiResourceUpdateBatch *batch = m_rhi->nextResourceUpdateBatch();

    const QColor clearColor = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f);
    cb->beginPass(renderTarget(), clearColor, { 1.0f, 0 }, batch);

    cb->setViewport(QRhiViewport(0, 0, outputSize.width(), outputSize.height()));

    size_t offset = 0;

    offset = align(0, STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(m_uniformBuffer.get(),
                               offset,
                               sizeof(glm::mat4),
                               m_view.constData());

    offset = align(sizeof(glm::mat4), STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(m_uniformBuffer.get(),
                               offset,
                               sizeof(glm::mat4),
                               m_projection.constData());


    for (int i = 0; i < m_graphics; i ++) {
        glm::mat4 model = m_models[i];
        // model = glm::rotate(model, qDegreesToRadians(m_angle),
        //                     glm::vec3(0.0f, 1.0f, 1.0f));
        model = glm::scale(model, glm::vec3(2.0, 2.0, 0.0));
        model = glm::translate(model,glm::vec3(0.0f, 0.0f, 0.0f));

        batch->uploadStaticBuffer(m_modelBuffer.get(),
                                  i * sizeof(glm::mat4),
                                  sizeof(glm::mat4),
                                  &model);
    }

    cb->resourceUpdate(batch);

    cb->setGraphicsPipeline(m_pipeline.get());
    cb->setShaderResources(m_srb.get());
    const QRhiCommandBuffer::VertexInput inputBindings4[] = {
        { m_vectexBuffer.get(), 0 },
        { m_modelBuffer.get(), 0 }
    };
    cb->setVertexInput(0, 2, inputBindings4);
    cb->draw(m_segments*2, m_graphics);

    cb->endPass();
}

void CurveRenderer::synchronize(QQuickRhiItem *rhiItem)
{
    Curve *item = static_cast<Curve *>(rhiItem);
    if (item->angle() != m_angle)
        m_angle = item->angle();

    m_orthoX = item->getOrthoX();
    m_orthoY = item->getOrthoY();
    m_zoom = item->getZoom();
    m_focus = item->getFocus();
}

Curve::Curve()
{
    setFocusPolicy(Qt::ClickFocus);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(ItemAcceptsInputMethod, true);
}

void Curve::hoverMoveEvent(QHoverEvent *event)
{
    if (m_spaceButtonDown) {

        int offsetX = (int)event->position().x() -
                      m_mosePosition0.x() - this->width()/2;
        int offsetY = this->height()/2 -
                      (int)event->position().y() - m_mosePosition0.y();

        m_focus.setX(offsetX);
        m_focus.setY(offsetY);

        // qDebug() << m_focus << m_mosePosition0 << event->position();
    }
    return QQuickRhiItem::hoverMoveEvent(event);
}

void Curve::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!m_leftButtonDown) {
            m_leftButtonDown = true;
            qDebug("m_leftButtonDown true");
        }
    }
    return QQuickRhiItem::mousePressEvent(event);
}


void Curve::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (m_leftButtonDown) {
            m_leftButtonDown = false;
            qDebug("m_leftButtonDown false");
        }
    }
    return QQuickRhiItem::mouseReleaseEvent(event);
}

void Curve::wheelEvent(QWheelEvent *event)
{
    // qDebug() << "Mouse wheel delta: " << event->angleDelta();
    if (event->angleDelta().y() > 0) {
        m_zoom += 10.0;
    }
    else if (event->angleDelta().y() < 0) {
        if (m_zoom - 10.0 >= 10.0)
            m_zoom -= 10.0;
    }
    return QQuickRhiItem::wheelEvent(event);
}


void Curve::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up) {
        m_orthoY -= 100.0;
    }
    else if (event->key() == Qt::Key_Down) {
        m_orthoY += 100.0;
    }
    else if (event->key() == Qt::Key_Left) {
        m_orthoX += 100.0;
    }
    else if (event->key() == Qt::Key_Right) {
        m_orthoX -= 100.0;
    }
    else if (event->key() == Qt::Key_Space) {
        if (!m_spaceButtonDown) {
            m_spaceButtonDown = true;
            qDebug("m_spaceButtonDown true");
        }
    }

    return QQuickRhiItem::keyPressEvent(event);
}

void Curve::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        if (m_spaceButtonDown) {
            m_spaceButtonDown = false;
            qDebug("m_spaceButtonDown false");
        }
    }

    return QQuickRhiItem::keyReleaseEvent(event);
}

QQuickRhiItemRenderer* Curve::createRenderer()
{
    return new CurveRenderer();
}


float Curve::angle() const
{
    return m_angle;
}

void Curve::setAngle(float a)
{
    if (m_angle == a)
        return;

    m_angle = a;
    emit angleChanged();
    update();
}

float Curve::getOrthoX()
{
    return m_orthoX;
}

float Curve::getOrthoY()
{
    return m_orthoY;
}

float Curve::getZoom()
{
    return m_zoom;
}

QPointF& Curve::getFocus()
{
    return m_focus;
}
