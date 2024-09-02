#include "stage.h"
#include "features.h"
#include <QFile>
#include <QCursor>
#include <QRandomGenerator>
#include <QtMath>

static QShader getShader(const QString &name)
{
    QFile f(name);
    return f.open(QIODevice::ReadOnly) ? QShader::fromSerialized(f.readAll()) : QShader();
}

float vertexCube[] = {
    //---- Position------   -----Color-----
    // X       Y       Z    R     G     B

    // Rectangle Vertices Attributes
   -100.0f, -100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    100.0f, -100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
   -100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
   -100.0f, -100.0f, -100.0f,   1.0f, 0.0f, 0.0f,

   -100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    100.0f,  100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    100.0f,  100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
   -100.0f,  100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
   -100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 0.0f,

    -100.0f,  100.0f,  100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f,  100.0f, -100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f, -100.0f, -100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f, -100.0f, -100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f, -100.0f,  100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f,  100.0f,  100.0f,   0.0f, 0.0f, 1.0f,

    100.0f,  100.0f,  100.0f,    1.0f, 1.0f, 0.0f,
    100.0f,  100.0f, -100.0f,    1.0f, 1.0f, 0.0f,
    100.0f, -100.0f, -100.0f,    1.0f, 1.0f, 0.0f,
    100.0f, -100.0f, -100.0f,    1.0f, 1.0f, 0.0f,
    100.0f, -100.0f,  100.0f,    1.0f, 1.0f, 0.0f,
    100.0f,  100.0f,  100.0f,    1.0f, 1.0f, 0.0f,

    -100.0f, -100.0f, -100.0f,   0.0f, 1.0f, 1.0f,
    100.0f, -100.0f, -100.0f,   0.0f, 1.0f, 1.0f,
    100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 1.0f,
    100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 1.0f,
    -100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 1.0f,
    -100.0f, -100.0f, -100.0f,   0.0f, 1.0f, 1.0f,

    -100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 1.0f,
    100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 1.0f,
    100.0f,  100.0f,  100.0f,   1.0f, 0.0f, 1.0f,
    100.0f,  100.0f,  100.0f,   1.0f, 0.0f, 1.0f,
    -100.0f,  100.0f,  100.0f,   1.0f, 0.0f, 1.0f,
    -100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 1.0f,
};

float vertexPyramid[] = {

    //---- Position------   -----Color-----
    // X       Y       Z    R     G     B

    // Triangle Vertices Attributes
   -100.0f, -100.0f,  100.0f,  1.0f, 0.0f, 0.0f,
    100.0f, -100.0f,  100.0f,  1.0f, 0.0f, 0.0f,
    0.0f,    100.0f,    0.0f,  1.0f, 0.0f, 0.0f,

    100.0f, -100.0f,  100.0f,  0.0f, 1.0f, 0.0f,
    100.0f, -100.0f,  -100.0f, 0.0f, 1.0f, 0.0f,
    0.0f,    100.0f,    0.0f,  0.0f, 1.0f, 0.0f,

    100.0f, -100.0f,  -100.0f, 0.0f, 0.0f, 1.0f,
   -100.0f, -100.0f, -100.0f,  0.0f, 0.0f, 1.0f,
    0.0f,    100.0f,    0.0f,  0.0f, 0.0f, 1.0f,

   -100.0f, -100.0f, -100.0f,  1.0f, 1.0f, 0.0f,
   -100.0f, -100.0f,  100.0f,  1.0f, 1.0f, 0.0f,
    0.0f,    100.0f,    0.0f,  1.0f, 1.0f, 0.0f,

   -100.0f, -100.0f, -100.0f,  1.0f, 0.0f, 1.0f,
    100.0f, -100.0f, -100.0f,  1.0f, 0.0f, 1.0f,
    100.0f, -100.0f,  100.0f,  1.0f, 0.0f, 1.0f,

    -100.0f, -100.0f, -100.0f, 0.0f, 1.0f, 1.0f,
     100.0f, -100.0f,  100.0f, 0.0f, 1.0f, 1.0f,
    -100.0f, -100.0f,  100.0f, 0.0f, 1.0f, 1.0f,

};


StageRenderer::StageRenderer()
{
    m_modelCubes = new glm::mat4[m_Cubes];
    for (int i = 0; i < m_Cubes; i ++) {
        double ns = QRandomGenerator::global()->bounded(1, 2) * 0.5;
        double ntX = QRandomGenerator::global()->bounded(-16000, 16000);
        double ntY = QRandomGenerator::global()->bounded(-16000, 16000);
        m_modelCubes[i] = glm::mat4(1.0f);
        m_modelCubes[i] = glm::scale(m_modelCubes[i], glm::vec3(ns, ns, ns));
        m_modelCubes[i] = glm::translate(m_modelCubes[i], glm::vec3(ntX, ntY, 0));
    }

    m_modelPyramids = new glm::mat4[m_Pyramids];
    for (int i = 0; i < m_Pyramids; i ++) {
        double ns = QRandomGenerator::global()->bounded(1, 2) * 0.5;
        double ntX = QRandomGenerator::global()->bounded(-16000, 16000);
        double ntY = QRandomGenerator::global()->bounded(-16000, 16000);
        m_modelPyramids[i] = glm::mat4(1.0f);
        m_modelPyramids[i] = glm::scale(m_modelPyramids[i], glm::vec3(ns, ns, ns));
        m_modelPyramids[i] = glm::translate(m_modelPyramids[i], glm::vec3(ntX, ntY, 0));
    }    
}

StageRenderer::~StageRenderer()
{
    if (m_modelCubes) {
        delete m_modelCubes;
    }
    if (m_modelPyramids) {
        delete m_modelPyramids;
    }
}


int StageRenderer::createCubeBuffer()
{
    if (!m_rhi)
        return -1;

    m_vectexBufferCube.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                              QRhiBuffer::VertexBuffer,
                                              sizeof(vertexCube)));
    m_vectexBufferCube->create();

    m_modelBufferCube.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                             QRhiBuffer::VertexBuffer,
                                             m_Cubes*sizeof(glm::mat4)));
    m_modelBufferCube->create();

    return m_Cubes*sizeof(glm::mat4);
}

int StageRenderer::createPyramidBuffer()
{
    if (!m_rhi)
        return -1;

    m_vectexBufferPyramid.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                                 QRhiBuffer::VertexBuffer,
                                                 sizeof(vertexPyramid)));
    m_vectexBufferPyramid->create();

    m_modelBufferPyramid.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                                QRhiBuffer::VertexBuffer,
                                                m_Pyramids*sizeof(glm::mat4)));
    m_modelBufferPyramid->create();

    return m_Pyramids*sizeof(glm::mat4);
}


int StageRenderer::createShaderResource()
{
    if (!m_rhi)
        return -1;

    // uniformbuffer1 的每个block包含两个矩阵，view matrix 和 projection matrix
    // 每个block必须根据硬件进行对齐。“对齐”是为了在绘图的时候可以通过字节偏移量动态的把
    // 缓冲区里的某个block值映射到 Shader 里的 uniform block，而偏移量必须是“对齐字节
    // 数”的整数倍。
    int blockSize = 64*2;
    int bufferSize = 0;
    m_uniformBufferBlockSize = m_rhi->ubufAligned(blockSize);
    bufferSize = m_uniformBufferBlockSize * m_uniformBufferBlockCount;
    m_uniformBuffer.reset(m_rhi->newBuffer(QRhiBuffer::Dynamic,
                                           QRhiBuffer::UniformBuffer,
                                           bufferSize));
    m_uniformBuffer->create();

    m_srb.reset(m_rhi->newShaderResourceBindings());

    // uniform 缓冲区使用 uniformBufferWithDynamicOffset 函数声明绑定
    m_srb->setBindings({
        QRhiShaderResourceBinding::uniformBufferWithDynamicOffset(
            0,
            QRhiShaderResourceBinding::VertexStage,
            m_uniformBuffer.get(),
            bufferSize),
    });
    m_srb->create();

    return bufferSize;
}

int StageRenderer::createPipline1()
{
    if (!m_rhi)
        return -1;

    m_pipeline1.reset(m_rhi->newGraphicsPipeline());

    m_pipeline1->setShaderStages({
        {
            QRhiShaderStage::Vertex,
            getShader(QLatin1String(":/AnymateLab/shaders/stage.vert.qsb"))
        },
        {
            QRhiShaderStage::Fragment,
            getShader(QLatin1String(":/AnymateLab/shaders/stage.frag.qsb"))
        }
    });

    QRhiVertexInputLayout inputLayout;
    inputLayout.setBindings(
        {
         // PerVertex 表示该顶点属性的索引是以“顶点”进行递增的
         { 6 * sizeof(float), QRhiVertexInputBinding::PerVertex },
         // PerInstance 表示该顶点属性的索引是以“实例”进行递增的
         { 64, QRhiVertexInputBinding::PerInstance },
         }
        );

    inputLayout.setAttributes(
        {
         // binding0
         { 0, 0, QRhiVertexInputAttribute::Float3, 0 },
         { 0, 1, QRhiVertexInputAttribute::Float3, 3 * sizeof(float) },
         // binding1, model矩阵由4个 vec4构成，每个vec4代表一列，从location2开始，
         // 每个列对应一个location。
         { 1, 2, QRhiVertexInputAttribute::Float4, 0 },
         { 1, 3, QRhiVertexInputAttribute::Float4, 4 * sizeof(float) },
         { 1, 4, QRhiVertexInputAttribute::Float4, 8 * sizeof(float) },
         { 1, 5, QRhiVertexInputAttribute::Float4, 12 * sizeof(float) },
         }
        );
    m_pipeline1->setVertexInputLayout(inputLayout);

    m_pipeline1->setSampleCount(m_sampleCount);
    m_pipeline1->setShaderResourceBindings(m_srb.get());
    m_pipeline1->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    m_pipeline1->setDepthTest(true);
    m_pipeline1->setDepthWrite(true);
    m_pipeline1->create();

    return 1;
}

int StageRenderer::createPipline2()
{
    if (!m_rhi)
        return -1;

    m_pipeline2.reset(m_rhi->newGraphicsPipeline());

    m_pipeline2->setShaderStages({
        {
            QRhiShaderStage::Vertex,
            getShader(QLatin1String(":/AnymateLab/shaders/bezier.vert.qsb"))
        },
        {
            QRhiShaderStage::Fragment,
            getShader(QLatin1String(":/AnymateLab/shaders/bezier.frag.qsb"))
        }
    });

    QRhiVertexInputLayout inputLayout;
    inputLayout.setBindings(
        {
         // 顶点着色模式（PerVertex Mode）：顶点属性将为每个顶点更新。这是常见
         // 的模式，用于正常的非实例化渲染，每个顶点属性会随着顶点的变化而变化。
         // 实例化着色模式（PerInstance Mode）：顶点属性将为每个实例更新一次。
         // 这种模式在实例化渲染中很有用，适用于每个实例使用相同的顶点属性，但是
         // 每个实例之间的属性可能不同。

         // PerVertex 表示该顶点属性的索引是以“顶点”进行递增的
         { 6 * sizeof(float), QRhiVertexInputBinding::PerVertex },
         // PerInstance 表示该顶点属性的索引是以“实例”进行递增的
         { 64, QRhiVertexInputBinding::PerInstance },
         }
        );

    inputLayout.setAttributes(
        {
         // binding0
         { 0, 0, QRhiVertexInputAttribute::Float3, 0 },
         { 0, 1, QRhiVertexInputAttribute::Float3, 3 * sizeof(float) },
         // binding1, model矩阵由4个 vec4构成，每个vec4代表一列，从location2开始，
         // 每个列对应一个location。
         { 1, 2, QRhiVertexInputAttribute::Float4, 0 },
         { 1, 3, QRhiVertexInputAttribute::Float4, 4 * sizeof(float) },
         { 1, 4, QRhiVertexInputAttribute::Float4, 8 * sizeof(float) },
         { 1, 5, QRhiVertexInputAttribute::Float4, 12 * sizeof(float) },
         }
        );
    m_pipeline2->setVertexInputLayout(inputLayout);

    m_pipeline2->setSampleCount(m_sampleCount);
    m_pipeline2->setShaderResourceBindings(m_srb.get());
    m_pipeline2->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    m_pipeline2->setDepthTest(true);
    m_pipeline2->setDepthWrite(true);
    // 绘制线条模式
    m_pipeline2->setTopology(QRhiGraphicsPipeline::Lines);
    m_pipeline2->create();

    return 1;
}

void StageRenderer::initialize(QRhiCommandBuffer *cb)
{
    if (m_rhi != rhi()) {
        m_rhi = rhi();
        ShowFreatures(m_rhi);

        createCubeBuffer();
        createPyramidBuffer();
        createShaderResource();

        m_pipeline1.reset();
        m_pipeline2.reset();
    }

    if (m_sampleCount != renderTarget()->sampleCount()) {
        m_sampleCount = renderTarget()->sampleCount();
        m_pipeline1.reset();
        m_pipeline2.reset();
    }

    QRhiTexture *finalTex = m_sampleCount > 1 ? resolveTexture() : colorTexture();
    if (m_textureFormat != finalTex->format()) {
        m_textureFormat = finalTex->format();
        m_pipeline1.reset();
        m_pipeline2.reset();
    }

    if (!m_pipeline1) {
        createPipline1();
    }

    if (!m_pipeline2) {
        createPipline2();
    }

    QRhiResourceUpdateBatch *batch = m_rhi->nextResourceUpdateBatch();
    batch->uploadStaticBuffer(m_vectexBufferCube.get(), vertexCube);
    batch->uploadStaticBuffer(m_vectexBufferPyramid.get(), vertexPyramid);
    batch->uploadStaticBuffer(m_modelBufferCube.get(), m_modelCubes);
    batch->uploadStaticBuffer(m_modelBufferPyramid.get(), m_modelPyramids);

    cb->resourceUpdate(batch);

}

void StageRenderer::render(QRhiCommandBuffer *cb)
{
    const QSize outputSize = renderTarget()->pixelSize();
    m_projection = m_rhi->clipSpaceCorrMatrix();

    // 所谓投影是指场景里被显示在Viewport里的内容，投影区域有“大小”和“中心点”这两个属性，
    // 从人的视觉角度上说，这两个属性决定了看到什么物体以及物体的大小。
    // 使用正交投影，left + right 和 bottom + top决定投影的大小 ，相应的，left和right
    // 决定了投影的水平中心, bottom 和 top 则决定了投影的垂直中心。
    //
    // m_projection.ortho(-outputSize.width()/2.0 + m_orthoX,
    //                    outputSize.width()/2.0 + m_orthoX,
    //                    -outputSize.height()/2.0 + m_orthoY,
    //                    outputSize.height()/2.0 + m_orthoY,
    //                    -200.0f, 10000.0f);

    m_projection.perspective(45.0f,
                             outputSize.width() / (float) outputSize.height(),
                             10.0f,
                             1000000.0f);


    // 使用透视投影，相机的z轴位置决定了场景投影范围的“大小”，相机的目标则决定了投影“中心点”。
    QVector3D cameraPos(0.0f, 0.0f, 800.0f + m_zoom);
    QVector3D cameraTarget(m_focus.rx(), m_focus.ry(), 0.0f);
    QVector3D cameraUp(0.0f, 1.0f, 0.0f);
    m_view.setToIdentity();
    m_view.lookAt(cameraPos, cameraTarget, cameraUp);

    QRhiResourceUpdateBatch *batch = m_rhi->nextResourceUpdateBatch();

    const QColor clearColor = QColor::fromRgbF(1.0f, 1.0f, 1.0f, 1.0f);
    cb->beginPass(renderTarget(), clearColor, { 1.0f, 0 }, batch);

    cb->setGraphicsPipeline(m_pipeline1.get());

    cb->setViewport(QRhiViewport(0, 0, outputSize.width(), outputSize.height()));

    // 批量更新 uniform 缓冲区
    batch->updateDynamicBuffer(m_uniformBuffer.get(),
                               0,
                               64,
                               m_view.constData());
    batch->updateDynamicBuffer(m_uniformBuffer.get(),
                               64,
                               64,
                               m_projection.constData());

    for (int i = 0; i < m_Cubes; i ++) {
        glm::mat4 model = glm::rotate(m_modelCubes[i],
                                      qDegreesToRadians(m_angle),
                                      glm::vec3(1.0f, 1.0f, 0.0f));

        batch->uploadStaticBuffer(m_modelBufferCube.get(),
                                  i * sizeof(glm::mat4),
                                  sizeof(glm::mat4),
                                  &model);
    }

    for (int i = 0; i < m_Pyramids; i ++) {
        glm::mat4 model = glm::rotate(m_modelPyramids[i],
                                      qDegreesToRadians(m_angle),
                                      glm::vec3(0.0f, 1.0f, 1.0f));
        batch->uploadStaticBuffer(m_modelBufferPyramid.get(),
                                  i * sizeof(glm::mat4),
                                  sizeof(glm::mat4),
                                  &model);
    }

    // 更新
    cb->resourceUpdate(batch);

    cb->setShaderResources(m_srb.get());

    const QRhiCommandBuffer::VertexInput vertexBufferBindings1[] = {
        { m_vectexBufferCube.get(), 0 },
        { m_modelBufferCube.get(), 0 }
    };
    cb->setVertexInput(0, 2, vertexBufferBindings1);
    // 绘制实例，由6个顶点构成，顶点属性数据从偏移量0开始，实例id为0。这里需要注意，实例
    // id是很重要的一个参数，它用于着色器索引 model 矩阵的数据。在本例中 model 矩阵数据
    // 是以顶点属性数组绑定到着色器，并且被声明为以PerInstance模式进行绑定，意思是每绘制
    // 完一个实例（而不是每绘制完一个顶点），model顶点属性的索引才会进行一次递增，这样就能
    // 做到每个绘制的一个实例对应 model 顶点属性数组里的一个 model 矩阵。
    cb->draw(36, m_Cubes);

    cb->setGraphicsPipeline(m_pipeline2.get());
    cb->setShaderResources(m_srb.get());
    const QRhiCommandBuffer::VertexInput vertexBufferBindings2[] = {
        { m_vectexBufferPyramid.get(), 0 },
        { m_modelBufferPyramid.get(), 0 }
    };
    cb->setVertexInput(0, 2, vertexBufferBindings2);
    cb->draw(18, m_Pyramids);

    cb->endPass();
}

void StageRenderer::synchronize(QQuickRhiItem *rhiItem)
{
    Stage *item = static_cast<Stage *>(rhiItem);
    if (item->angle() != m_angle)
        m_angle = item->angle();

    m_orthoX = item->getOrthoX();
    m_orthoY = item->getOrthoY();
    m_zoom = item->getZoom();
    m_focus = item->getFocus();
}

Stage::Stage()
{
    setFocusPolicy(Qt::ClickFocus);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(ItemAcceptsInputMethod, true);
}

void Stage::hoverMoveEvent(QHoverEvent *event)
{
    if (m_spaceButtonDown) {

        int offsetX = (int)event->position().x() - m_mosePosition0.x() - this->width()/2;
        int offsetY = this->height()/2 - (int)event->position().y() - m_mosePosition0.y();

        m_focus.setX(offsetX);
        m_focus.setY(offsetY);

        // qDebug() << m_focus << m_mosePosition0 << event->position();
    }
    return QQuickRhiItem::hoverMoveEvent(event);
}

void Stage::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!m_leftButtonDown) {
            m_leftButtonDown = true;
            qDebug("m_leftButtonDown true");
        }
    }
    return QQuickRhiItem::mousePressEvent(event);
}


void Stage::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (m_leftButtonDown) {
            m_leftButtonDown = false;
            qDebug("m_leftButtonDown false");
        }
    }
    return QQuickRhiItem::mouseReleaseEvent(event);
}

void Stage::wheelEvent(QWheelEvent *event)
{
    // qDebug() << "Mouse wheel delta: " << event->angleDelta();
    if (event->angleDelta().y() > 0) {
        m_zoom += 200.0;
    }
    else if (event->angleDelta().y() < 0) {
        m_zoom -= 200.0;
    }
    return QQuickRhiItem::wheelEvent(event);
}


void Stage::keyPressEvent(QKeyEvent *event)
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

void Stage::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        if (m_spaceButtonDown) {
            m_spaceButtonDown = false;
            qDebug("m_spaceButtonDown false");
        }
    }

    return QQuickRhiItem::keyReleaseEvent(event);
}

QQuickRhiItemRenderer* Stage::createRenderer()
{
    return new StageRenderer();
}


float Stage::angle() const
{
    return m_angle;
}

void Stage::setAngle(float a)
{
    if (m_angle == a)
        return;

    m_angle = a;
    emit angleChanged();
    update();
}


float Stage::getOrthoX()
{
    return m_orthoX;
}


float Stage::getOrthoY()
{
    return m_orthoY;
}

float Stage::getZoom()
{
    return m_zoom;
}

QPointF& Stage::getFocus()
{
    return m_focus;
}
