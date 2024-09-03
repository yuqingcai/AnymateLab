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

    -100.0f,  100.0f,  100.0f,  0.0f, 0.0f, 1.0f,
    -100.0f,  100.0f, -100.0f,  0.0f, 0.0f, 1.0f,
    -100.0f, -100.0f, -100.0f,  0.0f, 0.0f, 1.0f,
    -100.0f, -100.0f, -100.0f,  0.0f, 0.0f, 1.0f,
    -100.0f, -100.0f,  100.0f,  0.0f, 0.0f, 1.0f,
    -100.0f,  100.0f,  100.0f,  0.0f, 0.0f, 1.0f,

    100.0f,  100.0f,  100.0f,   1.0f, 1.0f, 0.0f,
    100.0f,  100.0f, -100.0f,   1.0f, 1.0f, 0.0f,
    100.0f, -100.0f, -100.0f,   1.0f, 1.0f, 0.0f,
    100.0f, -100.0f, -100.0f,   1.0f, 1.0f, 0.0f,
    100.0f, -100.0f,  100.0f,   1.0f, 1.0f, 0.0f,
    100.0f,  100.0f,  100.0f,   1.0f, 1.0f, 0.0f,

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


float vertexBezier[] = {
    //---- Position------   -----Color-----
    // X       Y       Z    R     G     B
      0.0f,    0.0f,  200.0f,   1.0f, 0.0f, 0.0f,
    200.0f,    0.0f,  200.0f,   1.0f, 0.0f, 0.0f,
      0.0f,  100.0f,  200.0f,   1.0f, 0.0f, 0.0f,
      0.0f,  100.0f,  200.0f,   1.0f, 0.0f, 0.0f,
    200.0f,    0.0f,  200.0f,   1.0f, 0.0f, 0.0f,
    200.0f,  100.0f,  200.0f,   1.0f, 0.0f, 0.0f,
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

    m_modelBeziers = new glm::mat4[m_Beziers];
    for (int i = 0; i < m_Beziers; i ++) {
        m_modelBeziers[i] = glm::mat4(1.0f);
    }
}

StageRenderer::~StageRenderer()
{
    if (m_modelBeziers) {
        delete m_modelBeziers;
    }

    if (m_modelPyramids) {
        delete m_modelPyramids;
    }
    if (m_modelCubes) {
        delete m_modelCubes;
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

    return 0;
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

    return 0;
}

int StageRenderer::createBezierBuffer()
{
    if (!m_rhi)
        return -1;

    m_vectexBufferBezier.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                                QRhiBuffer::VertexBuffer,
                                                sizeof(vertexBezier)));
    m_vectexBufferBezier->create();

    m_modelBufferBezier.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                               QRhiBuffer::VertexBuffer,
                                               m_Beziers*sizeof(glm::mat4)));
    m_modelBufferBezier->create();

    return 0;
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
            // 顶点着色模式（PerVertex Mode）：顶点属性将为每个顶点更新。这是常见
            // 的模式，用于正常的非实例化渲染，每个顶点属性会随着顶点的变化而变化。
            // 实例化着色模式（PerInstance Mode）：顶点属性将为每个实例更新一次。
            // 这种模式在实例化渲染中很有用，适用于每个实例使用相同的顶点属性，但是
            // 每个实例之间的属性可能不同。
            { 64, QRhiVertexInputBinding::PerInstance },
        });

    inputLayout.setAttributes(
        {
            // binding0
            { 0, 0, QRhiVertexInputAttribute::Float3, 0 },
            { 0, 1, QRhiVertexInputAttribute::Float3, 3 * sizeof(float) },
            // binding1, 从location2开始的4个vec4，每个vec4对应model矩阵的一个列
            { 1, 2, QRhiVertexInputAttribute::Float4, 0 },
            { 1, 3, QRhiVertexInputAttribute::Float4, 4 * sizeof(float) },
            { 1, 4, QRhiVertexInputAttribute::Float4, 8 * sizeof(float) },
            { 1, 5, QRhiVertexInputAttribute::Float4, 12 * sizeof(float) },
        });

    // uniform buffer 的每个block包含两个矩阵，view matrix 和 projection matrix
    // 每个block必须根据硬件进行对齐。“对齐”是为了在绘图的时候可以通过字节偏移量动
    // 态的把缓冲区里的某个block值映射到 Shader 里的 uniform block，而偏移量必须
    // 是“对齐字节数”的整数倍。
    int blockSize = 64*2;
    int bufferSize = 0;
    bufferSize = m_rhi->ubufAligned(blockSize) * m_uniformBufferBlockCount;
    m_uniformBuffer1.reset(m_rhi->newBuffer(QRhiBuffer::Dynamic,
                                           QRhiBuffer::UniformBuffer,
                                           bufferSize));
    m_uniformBuffer1->create();

    m_srb1.reset(m_rhi->newShaderResourceBindings());

    // uniform 缓冲区使用 uniformBufferWithDynamicOffset 函数声明绑定
    m_srb1->setBindings({
        QRhiShaderResourceBinding::uniformBufferWithDynamicOffset(
            0,
            QRhiShaderResourceBinding::VertexStage,
            m_uniformBuffer1.get(),
            bufferSize),
    });
    m_srb1->create();

    m_pipeline1->setSampleCount(m_sampleCount);
    m_pipeline1->setVertexInputLayout(inputLayout);
    m_pipeline1->setShaderResourceBindings(m_srb1.get());
    m_pipeline1->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    m_pipeline1->setDepthTest(true);
    m_pipeline1->setDepthWrite(true);

    m_pipeline1->create();

    return 0;
}

int StageRenderer::createPipline2()
{
    if (!m_rhi)
        return -1;

    m_pipeline2.reset(m_rhi->newGraphicsPipeline());

    m_pipeline2->setShaderStages({
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
            { 6 * sizeof(float), QRhiVertexInputBinding::PerVertex },
            { 64, QRhiVertexInputBinding::PerInstance },
        });

    inputLayout.setAttributes(
        {
            // binding0
            { 0, 0, QRhiVertexInputAttribute::Float3, 0 },
            { 0, 1, QRhiVertexInputAttribute::Float3, 3 * sizeof(float) },
            // binding1
            { 1, 2, QRhiVertexInputAttribute::Float4, 0 },
            { 1, 3, QRhiVertexInputAttribute::Float4, 4 * sizeof(float) },
            { 1, 4, QRhiVertexInputAttribute::Float4, 8 * sizeof(float) },
            { 1, 5, QRhiVertexInputAttribute::Float4, 12 * sizeof(float) },
        });

    int blockSize = 64*2;
    int bufferSize = 0;
    bufferSize = m_rhi->ubufAligned(blockSize) * m_uniformBufferBlockCount;
    m_uniformBuffer2.reset(m_rhi->newBuffer(QRhiBuffer::Dynamic,
                                            QRhiBuffer::UniformBuffer,
                                            bufferSize));
    m_uniformBuffer2->create();

    m_srb2.reset(m_rhi->newShaderResourceBindings());
    m_srb2->setBindings({
        QRhiShaderResourceBinding::uniformBufferWithDynamicOffset(
            0,
            QRhiShaderResourceBinding::VertexStage,
            m_uniformBuffer2.get(),
            bufferSize),
    });
    m_srb2->create();

    m_pipeline2->setVertexInputLayout(inputLayout);
    m_pipeline2->setSampleCount(m_sampleCount);
    m_pipeline2->setShaderResourceBindings(m_srb2.get());
    m_pipeline2->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    m_pipeline2->setDepthTest(true);
    m_pipeline2->setDepthWrite(true);
    // 绘制线条模式
    m_pipeline2->setTopology(QRhiGraphicsPipeline::LineStrip);
    m_pipeline2->create();

    return 1;
}

int StageRenderer::createPipline3()
{
    if (!m_rhi)
        return -1;

    m_pipeline3.reset(m_rhi->newGraphicsPipeline());

    m_pipeline3->setShaderStages({
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
         { 6 * sizeof(float), QRhiVertexInputBinding::PerVertex },
         { 64, QRhiVertexInputBinding::PerInstance },
         });

    inputLayout.setAttributes(
        {
         // binding0
         { 0, 0, QRhiVertexInputAttribute::Float3, 0 },
         { 0, 1, QRhiVertexInputAttribute::Float3, 3 * sizeof(float) },
         // binding1
         { 1, 2, QRhiVertexInputAttribute::Float4, 0 },
         { 1, 3, QRhiVertexInputAttribute::Float4, 4 * sizeof(float) },
         { 1, 4, QRhiVertexInputAttribute::Float4, 8 * sizeof(float) },
         { 1, 5, QRhiVertexInputAttribute::Float4, 12 * sizeof(float) },
         });

    int blockSize = 64*2 + sizeof(glm::vec2) * 4;
    int bufferSize = 0;
    bufferSize = m_rhi->ubufAligned(blockSize) * m_uniformBufferBlockCount;
    m_uniformBuffer3.reset(m_rhi->newBuffer(QRhiBuffer::Dynamic,
                                            QRhiBuffer::UniformBuffer,
                                            bufferSize));
    m_uniformBuffer3->create();

    m_srb3.reset(m_rhi->newShaderResourceBindings());
    m_srb3->setBindings({
        QRhiShaderResourceBinding::uniformBufferWithDynamicOffset(
            0,
            QRhiShaderResourceBinding::VertexStage,
            m_uniformBuffer3.get(),
            bufferSize),
    });
    m_srb3->create();

    m_pipeline3->setVertexInputLayout(inputLayout);
    m_pipeline3->setSampleCount(m_sampleCount);
    m_pipeline3->setShaderResourceBindings(m_srb3.get());
    m_pipeline3->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    m_pipeline3->setDepthTest(true);
    m_pipeline3->setDepthWrite(true);
    // m_pipeline3->setTopology(QRhiGraphicsPipeline::LineStrip);
    m_pipeline3->create();

    return 1;
}

void StageRenderer::initialize(QRhiCommandBuffer *cb)
{
    if (m_rhi != rhi()) {
        m_rhi = rhi();
        ShowFreatures(m_rhi);

        createCubeBuffer();
        createPyramidBuffer();
        createBezierBuffer();

        m_pipeline1.reset();
        m_pipeline2.reset();
        m_pipeline3.reset();
    }

    if (m_sampleCount != renderTarget()->sampleCount()) {
        m_sampleCount = renderTarget()->sampleCount();
        m_pipeline1.reset();
        m_pipeline2.reset();
        m_pipeline3.reset();
    }

    QRhiTexture *finalTex = m_sampleCount > 1 ? resolveTexture() : colorTexture();
    if (m_textureFormat != finalTex->format()) {
        m_textureFormat = finalTex->format();
        m_pipeline1.reset();
        m_pipeline2.reset();
        m_pipeline3.reset();
    }

    if (!m_pipeline1) {
        createPipline1();
    }

    if (!m_pipeline2) {
        createPipline2();
    }

    if (!m_pipeline3) {
        createPipline3();
    }

    QRhiResourceUpdateBatch *batch = m_rhi->nextResourceUpdateBatch();

    batch->uploadStaticBuffer(m_vectexBufferCube.get(), vertexCube);
    batch->uploadStaticBuffer(m_modelBufferCube.get(), m_modelCubes);

    batch->uploadStaticBuffer(m_vectexBufferPyramid.get(), vertexPyramid);
    batch->uploadStaticBuffer(m_modelBufferPyramid.get(), m_modelPyramids);

    batch->uploadStaticBuffer(m_vectexBufferBezier.get(), vertexBezier);
    batch->uploadStaticBuffer(m_modelBufferBezier.get(), m_modelBeziers);

    cb->resourceUpdate(batch);

}

void StageRenderer::render(QRhiCommandBuffer *cb)
{
    const QSize outputSize = renderTarget()->pixelSize();
    m_projection = m_rhi->clipSpaceCorrMatrix();

    // 所谓投影是指场景里被显示在Viewport里的内容，投影区域有“大小”和“中心点”这
    // 两个属性，从人的视觉角度上说，这两个属性决定了看到什么物体以及物体的大小。
    // 使用正交投影，left + right 和 bottom + top决定投影的大小 ，相应的，
    // left和right 决定了投影的水平中心, bottom 和 top 则决定了投影的垂直中心。
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


    // 使用透视投影，相机的z轴位置决定了场景投影范围的“大小”，相机的目标则决定了
    // 投影“中心点”。
    QVector3D cameraPos(0.0f, 0.0f, 800.0f + m_zoom);
    QVector3D cameraTarget(m_focus.rx(), m_focus.ry(), 0.0f);
    QVector3D cameraUp(0.0f, 1.0f, 0.0f);
    m_view.setToIdentity();
    m_view.lookAt(cameraPos, cameraTarget, cameraUp);

    QRhiResourceUpdateBatch *batch = m_rhi->nextResourceUpdateBatch();

    const QColor clearColor = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f);
    cb->beginPass(renderTarget(), clearColor, { 1.0f, 0 }, batch);

    cb->setViewport(QRhiViewport(0, 0, outputSize.width(), outputSize.height()));

    // 更新 uniform buffer1 缓冲区
    batch->updateDynamicBuffer(m_uniformBuffer1.get(),
                               0,
                               64,
                               m_view.constData());
    batch->updateDynamicBuffer(m_uniformBuffer1.get(),
                               64,
                               64,
                               m_projection.constData());

    // 更新 uniform buffer2 缓冲区
    batch->updateDynamicBuffer(m_uniformBuffer2.get(),
                               0,
                               64,
                               m_view.constData());
    batch->updateDynamicBuffer(m_uniformBuffer2.get(),
                               64,
                               64,
                               m_projection.constData());


    // 更新 uniform buffer3 缓冲区
    batch->updateDynamicBuffer(m_uniformBuffer3.get(),
                               0,
                               64,
                               m_view.constData());
    batch->updateDynamicBuffer(m_uniformBuffer3.get(),
                               64,
                               64,
                               m_projection.constData());

    glm::vec2 resolution(800.0, 600.0);
    glm::vec2 p0(0.0, 0.0);
    glm::vec2 p1(100.0, 50.0);
    glm::vec2 p2(200.0, 0.0);
    batch->updateDynamicBuffer(m_uniformBuffer3.get(),
                               64*2,
                               sizeof(glm::vec2),
                               &resolution);
    batch->updateDynamicBuffer(m_uniformBuffer3.get(),
                               64*2 + sizeof(glm::vec2),
                               sizeof(glm::vec2),
                               &p0);
    batch->updateDynamicBuffer(m_uniformBuffer3.get(),
                               64*2 + sizeof(glm::vec2)*2,
                               sizeof(glm::vec2),
                               &p1);
    batch->updateDynamicBuffer(m_uniformBuffer3.get(),
                               64*2 + sizeof(glm::vec2)*3,
                               sizeof(glm::vec2),
                               &p2);

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

    // for (int i = 0; i < m_Beziers; i ++) {
    //     glm::mat4 model = glm::rotate(m_modelBeziers[i],
    //                                   qDegreesToRadians(m_angle),
    //                                   glm::vec3(0.0f, 1.0f, 1.0f));
    //     batch->uploadStaticBuffer(m_modelBufferBezier.get(),
    //                               i * sizeof(glm::mat4),
    //                               sizeof(glm::mat4),
    //                               &model);
    // }

    cb->resourceUpdate(batch);

    // // 使用管线1绘制立方体
    cb->setGraphicsPipeline(m_pipeline1.get());
    cb->setShaderResources(m_srb1.get());
    // 绑定顶点属性缓冲区
    const QRhiCommandBuffer::VertexInput inputBindings1[] = {
        { m_vectexBufferCube.get(), 0 },
        { m_modelBufferCube.get(), 0 }
    };
    cb->setVertexInput(0, 2, inputBindings1);
    cb->draw(36, m_Cubes);

    // 使用2号管线绘制线条图形
    cb->setGraphicsPipeline(m_pipeline2.get());
    cb->setShaderResources(m_srb2.get());
    // 绑定顶点属性缓冲区
    const QRhiCommandBuffer::VertexInput inputBindings2[] = {
        { m_vectexBufferPyramid.get(), 0 },
        { m_modelBufferPyramid.get(), 0 }
    };
    cb->setVertexInput(0, 2, inputBindings2);
    cb->draw(18, m_Pyramids);


    // 使用3号管线绘制贝塞尔曲线
    cb->setGraphicsPipeline(m_pipeline3.get());
    cb->setShaderResources(m_srb3.get());
    // 绑定顶点属性缓冲区
    const QRhiCommandBuffer::VertexInput inputBindings3[] = {
        { m_vectexBufferBezier.get(), 0 },
        { m_modelBufferBezier.get(), 0 }
    };
    cb->setVertexInput(0, 2, inputBindings3);
    cb->draw(6, m_Beziers);

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
