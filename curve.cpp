#include "curve.h"
#include "features.h"
#include <QFile>
#include <QCursor>
#include <QRandomGenerator>
#include <QtMath>
#include <QPainter>
#include <graphic.h>

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


CurveRenderer::CurveRenderer()
{
    _model = new glm::mat4(1.0f);
}

CurveRenderer::~CurveRenderer()
{
    if (_model) {
        delete [] _model;
    }
}

int CurveRenderer::createBuffer()
{
    if (!_rhi)
        return -1;

    size_t verticesSize = sizeof(float) * 1024 * 1024;
    _vectexBuffer.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                        QRhiBuffer::VertexBuffer,
                                        verticesSize));
    _vectexBuffer->create();

    _modelBuffer.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                       QRhiBuffer::VertexBuffer,
                                       sizeof(glm::mat4) * 1024));
    _modelBuffer->create();

    return 0;
}


int CurveRenderer::createPipline()
{
    if (!_rhi)
        return -1;

    _pipeline.reset(_rhi->newGraphicsPipeline());

    _pipeline->setShaderStages({
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

    // view and projection uniform
    int blockSize = sizeof(glm::mat4) +
                    sizeof(glm::mat4)
        ;
    int bufferSize = _rhi->ubufAligned(blockSize);
    _uniformBuffer.reset(_rhi->newBuffer(QRhiBuffer::Dynamic,
                                         QRhiBuffer::UniformBuffer,
                                         bufferSize));
    _uniformBuffer->create();

    _srb.reset(_rhi->newShaderResourceBindings());
    _srb->setBindings({
        QRhiShaderResourceBinding::uniformBufferWithDynamicOffset(
            0,
            m_shaderResourceStages,
            _uniformBuffer.get(),
            bufferSize),
    });
    _srb->create();

    _pipeline->setVertexInputLayout(inputLayout);
    _pipeline->setSampleCount(_sampleCount);
    _pipeline->setShaderResourceBindings(_srb.get());
    _pipeline->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    _pipeline->setDepthTest(true);
    _pipeline->setDepthWrite(true);
    _pipeline->setTopology(QRhiGraphicsPipeline::TriangleStrip);

    QList<QRhiGraphicsPipeline::TargetBlend> targetBlends(1);
    targetBlends[0].enable = true;
    targetBlends[0].srcColor = QRhiGraphicsPipeline::SrcAlpha;
    targetBlends[0].dstColor = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opColor = QRhiGraphicsPipeline::Add;
    targetBlends[0].srcAlpha = QRhiGraphicsPipeline::One;
    targetBlends[0].dstAlpha = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opAlpha = QRhiGraphicsPipeline::Add;
    _pipeline->setTargetBlends(targetBlends.begin(), targetBlends.end());
    _pipeline->create();

    return 1;
}


void CurveRenderer::initialize(QRhiCommandBuffer *cb)
{
    if (_rhi != rhi()) {
        _rhi = rhi();
        // ShowFreatures(m_rhi);
        createBuffer();
        _pipeline.reset();
    }

    if (_sampleCount != renderTarget()->sampleCount()) {
        _sampleCount = renderTarget()->sampleCount();
        _pipeline.reset();
    }

    QRhiTexture *finalTex = _sampleCount > 1 ? resolveTexture() : colorTexture();
    if (_textureFormat != finalTex->format()) {
        _textureFormat = finalTex->format();
        _pipeline.reset();
    }

    if (!_pipeline) {
        createPipline();
    }

}

void CurveRenderer::render(QRhiCommandBuffer *cb)
{
    const QSize outputSize = renderTarget()->pixelSize();
    _projection = _rhi->clipSpaceCorrMatrix();

    _projection.perspective(45.0f,
                            outputSize.width() / (float) outputSize.height(),
                            10.0f,
                            1000000.0f);

    QVector3D cameraPos(_focus.rx(), _focus.ry(), _zoom);
    QVector3D cameraTarget(_focus.rx(), _focus.ry(), 0.0f);
    QVector3D cameraUp(0.0f, 1.0f, 0.0f);
    _view.setToIdentity();
    _view.lookAt(cameraPos, cameraTarget, cameraUp);

    QRhiResourceUpdateBatch *batch = _rhi->nextResourceUpdateBatch();

    const QColor clearColor = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f);
    cb->beginPass(renderTarget(), clearColor, { 1.0f, 0 }, batch);

    cb->setViewport(QRhiViewport(0, 0, outputSize.width(), outputSize.height()));

    size_t offset = 0;

    offset = align(0, STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(_uniformBuffer.get(),
                               offset,
                               sizeof(glm::mat4),
                               _view.constData());

    offset = align(sizeof(glm::mat4), STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(_uniformBuffer.get(),
                               offset,
                               sizeof(glm::mat4),
                               _projection.constData());

    // update VBO
    offset = 0;
    for (int i = 0; i < _shapes.size(); i ++) {
        batch->uploadStaticBuffer(_vectexBuffer.get(),
                                  offset,
                                  _shapes[i]->getVertexBufferSize(),
                                  _shapes[i]->getVertexBuffer());
        offset += _shapes[i]->getVertexBufferSize();

        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::rotate(model, qDegreesToRadians(_angle),
        //                     glm::vec3(0.0f, 1.0f, 1.0f));
        model = glm::scale(model, glm::vec3(1.0, 1.0, 0.0));
        model = glm::translate(model,glm::vec3(0.0f, 0.0f, 0.0f));
        batch->uploadStaticBuffer(_modelBuffer.get(),
                                  i * sizeof(glm::mat4),
                                  sizeof(glm::mat4),
                                  &model);
    }

    cb->resourceUpdate(batch);

    cb->setGraphicsPipeline(_pipeline.get());
    cb->setShaderResources(_srb.get());


    offset = 0;
    for (int i = 0; i < _shapes.size(); i ++) {

        const QRhiCommandBuffer::VertexInput inputBindings[] = {
            { _vectexBuffer.get(), offset },
            { _modelBuffer.get(), i * sizeof(glm::mat4) }
        };
        cb->setVertexInput(0, 2, inputBindings);
        cb->draw(_shapes[i]->getVertexCount());

        offset += _shapes[i]->getVertexBufferSize();
    }


    cb->endPass();
}

void CurveRenderer::synchronize(QQuickRhiItem *rhiItem)
{
    Curve *item = static_cast<Curve *>(rhiItem);
    if (item->angle() != _angle)
        _angle = item->angle();

    _shapes = item->getShapes();

    _orthoX = item->getOrthoX();
    _orthoY = item->getOrthoY();
    _zoom = item->getZoom();
    _focus = item->getFocus();
}

Curve::Curve()
{
    setFocusPolicy(Qt::ClickFocus);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(ItemAcceptsInputMethod, true);

    Anymate::Rectangle* rect0 = new Anymate::Rectangle();
    rect0->setX(0.0);
    rect0->setY(0.0);
    rect0->setWidth(100.0);
    rect0->setHeight(100.0);
    rect0->generateCurve();


    Anymate::Rectangle* rect1 = new Anymate::Rectangle();
    rect1->setX(100.0);
    rect1->setY(100.0);
    rect1->setWidth(50.0);
    rect1->setHeight(100.0);
    rect1->generateCurve();

    Anymate::Bezier* curve = new Anymate::Bezier({
        glm::vec2( -100.0, -100.0),
        glm::vec2(   0.0, -100.0),
        glm::vec2(   0.0,  100.0),
        glm::vec2( 100.0, 100.0)
    });
    curve->generateCurve();


    Anymate::Circle* circle = new Anymate::Circle(80.0);
    circle->generateCurve();

    _shapes.push_back(rect0);
    _shapes.push_back(rect1);
    _shapes.push_back(curve);
    _shapes.push_back(circle);

}

Curve::~Curve()
{
    for (auto ptr : _shapes) {
        delete ptr;
    }
    _shapes.clear();
}

std::vector<Anymate::GeometryShape*>& Curve::getShapes()
{
    return _shapes;
}

void Curve::hoverMoveEvent(QHoverEvent *event)
{
    if (_spaceButtonDown) {

        int offsetX = (int)event->position().x() -
                      _mosePosition0.x() - this->width()/2;
        int offsetY = this->height()/2 -
                      (int)event->position().y() - _mosePosition0.y();

        _focus.setX(offsetX);
        _focus.setY(offsetY);

        // qDebug() << m_focus << m_mosePosition0 << event->position();
    }
    return QQuickRhiItem::hoverMoveEvent(event);
}

void Curve::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!_leftButtonDown) {
            _leftButtonDown = true;
            qDebug("m_leftButtonDown true");
        }
    }
    return QQuickRhiItem::mousePressEvent(event);
}


void Curve::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (_leftButtonDown) {
            _leftButtonDown = false;
            qDebug("m_leftButtonDown false");
        }
    }
    return QQuickRhiItem::mouseReleaseEvent(event);
}

void Curve::wheelEvent(QWheelEvent *event)
{
    // qDebug() << "Mouse wheel delta: " << event->angleDelta();
    if (event->angleDelta().y() > 0) {
        _zoom += 10.0;
    }
    else if (event->angleDelta().y() < 0) {
        if (_zoom - 10.0 >= 10.0)
            _zoom -= 10.0;
    }
    return QQuickRhiItem::wheelEvent(event);
}


void Curve::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up) {
        _orthoY -= 100.0;
    }
    else if (event->key() == Qt::Key_Down) {
        _orthoY += 100.0;
    }
    else if (event->key() == Qt::Key_Left) {
        _orthoX += 100.0;
    }
    else if (event->key() == Qt::Key_Right) {
        _orthoX -= 100.0;
    }
    else if (event->key() == Qt::Key_Space) {
        if (!_spaceButtonDown) {
            _spaceButtonDown = true;
            qDebug("m_spaceButtonDown true");
        }
    }
    else if (event->key() == Qt::Key_R) {
        renderToTexture();
    }

    return QQuickRhiItem::keyPressEvent(event);
}

void Curve::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        if (_spaceButtonDown) {
            _spaceButtonDown = false;
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
    return _angle;
}

void Curve::setAngle(float a)
{
    if (_angle == a)
        return;

    _angle = a;
    emit angleChanged();
    update();
}

float Curve::getOrthoX()
{
    return _orthoX;
}

float Curve::getOrthoY()
{
    return _orthoY;
}

float Curve::getZoom()
{
    return _zoom;
}

QPointF& Curve::getFocus()
{
    return _focus;
}

void Curve::renderToTexture()
{
    qDebug("render!");

    std::unique_ptr<QRhi> rhi;

#if defined(Q_OS_WIN)
    QRhiD3D11InitParams params;
    params.enableDebugLayer = true;
    rhi.reset(QRhi::create(QRhi::D3D11, &params));
#elif defined(Q_OS_MACOS)
    QRhiMetalInitParams params;
    rhi.reset(QRhi::create(QRhi::Metal, &params));
#else
    QRhiVulkanInitParams params;
    params.inst = vulkanInstance();
    params.window = this;
    rhi.reset(QRhi::create(QRhi::Vulkan, &params));
#endif
    if (rhi)
        qDebug() << rhi->backendName() << rhi->driverInfo();
    else
        qFatal("Failed to initialize RHI");

    float rotation = 0.0f;
    float opacity = 1.0f;
    int opacityDir = 1;

    std::unique_ptr<QRhiTexture> tex(rhi->newTexture(QRhiTexture::RGBA8,
                                                     QSize(800, 600),
                                                     1,
                                                     QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
    tex->create();

    std::unique_ptr<QRhiTextureRenderTarget> rt(
        rhi->newTextureRenderTarget({ tex.get() }));
    std::unique_ptr<QRhiRenderPassDescriptor> rp(rt->newCompatibleRenderPassDescriptor());

    rt->setRenderPassDescriptor(rp.get());
    rt->create();

    QMatrix4x4 viewProjection = rhi->clipSpaceCorrMatrix();
    viewProjection.perspective(45.0f, 1280 / 720.f, 0.01f, 1000.0f);
    viewProjection.translate(0, 0, -4);

    static float vertexData[] = {
        // Y up, CCW
        0.0f,   0.5f,     1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,     0.0f, 1.0f, 0.0f,
        0.5f,  -0.5f,     0.0f, 0.0f, 1.0f,
    };

    std::unique_ptr<QRhiBuffer> vbuf(rhi->newBuffer(QRhiBuffer::Immutable,
                                                    QRhiBuffer::VertexBuffer,
                                                    sizeof(vertexData)));
    vbuf->create();

    std::unique_ptr<QRhiBuffer> ubuf(rhi->newBuffer(QRhiBuffer::Dynamic,
                                                    QRhiBuffer::UniformBuffer,
                                                    64 + 4));
    ubuf->create();

    std::unique_ptr<QRhiShaderResourceBindings> srb(rhi->newShaderResourceBindings());
    srb->setBindings({
        QRhiShaderResourceBinding::uniformBuffer(0,
                                                 QRhiShaderResourceBinding::VertexStage | QRhiShaderResourceBinding::FragmentStage,
                                                 ubuf.get())
    });
    srb->create();

    std::unique_ptr<QRhiGraphicsPipeline> ps(rhi->newGraphicsPipeline());
    QRhiGraphicsPipeline::TargetBlend premulAlphaBlend;
    premulAlphaBlend.enable = true;
    ps->setTargetBlends({ premulAlphaBlend });
    static auto getShader = [](const QString &name) {
        QFile f(name);
        return f.open(QIODevice::ReadOnly) ? QShader::fromSerialized(f.readAll()) : QShader();
    };
    ps->setShaderStages({
        { QRhiShaderStage::Vertex, getShader(QLatin1String(":/AnymateLab/shaders/color.vert.qsb")) },
        { QRhiShaderStage::Fragment, getShader(QLatin1String(":/AnymateLab/shaders/color.frag.qsb")) }
    });


    QRhiVertexInputLayout inputLayout;
    inputLayout.setBindings({
        { 5 * sizeof(float) }
    });
    inputLayout.setAttributes({
        { 0, 0, QRhiVertexInputAttribute::Float2, 0 },
        { 0, 1, QRhiVertexInputAttribute::Float3, 2 * sizeof(float) }
    });
    ps->setVertexInputLayout(inputLayout);
    ps->setShaderResourceBindings(srb.get());
    ps->setRenderPassDescriptor(rp.get());
    ps->create();

    QRhiCommandBuffer *cb;
    for (int frame = 0; frame < 20; ++frame) {
        rhi->beginOffscreenFrame(&cb);

        QRhiResourceUpdateBatch *u = rhi->nextResourceUpdateBatch();
        if (frame == 0)
            u->uploadStaticBuffer(vbuf.get(), vertexData);

        QMatrix4x4 mvp = viewProjection;
        mvp.rotate(rotation, 0, 1, 0);
        u->updateDynamicBuffer(ubuf.get(), 0, 64, mvp.constData());
        rotation += 5.0f;

        u->updateDynamicBuffer(ubuf.get(), 64, 4, &opacity);
        opacity += opacityDir * 0.2f;
        if (opacity < 0.0f || opacity > 1.0f) {
            opacityDir *= -1;
            opacity = qBound(0.0f, opacity, 1.0f);
        }

        const QColor clearColor = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f);
        cb->beginPass(rt.get(), clearColor, { 1.0f, 0 }, u);
        cb->setGraphicsPipeline(ps.get());
        cb->setViewport({ 0, 0, 1280, 720 });
        cb->setShaderResources();
        const QRhiCommandBuffer::VertexInput vbufBinding(vbuf.get(), 0);
        cb->setVertexInput(0, 1, &vbufBinding);
        cb->draw(3);
        QRhiReadbackResult readbackResult;
        u = rhi->nextResourceUpdateBatch();
        u->readBackTexture({ tex.get() }, &readbackResult);
        cb->endPass(u);

        rhi->endOffscreenFrame();

        QImage image(reinterpret_cast<const uchar *>(readbackResult.data.constData()),
                     readbackResult.pixelSize.width(),
                     readbackResult.pixelSize.height(),
                     QImage::Format_RGBA8888_Premultiplied);
        if (rhi->isYUpInFramebuffer())
            image = image.mirrored();
        image.save(QString::asprintf("frame%d.png", frame));
    }

}
