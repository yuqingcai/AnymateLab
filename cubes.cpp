#include "cubes.h"
#include "features.h"
#include <QFile>
#include <QCursor>
#include <QRandomGenerator>
#include <QtMath>
#include <QPainter>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

static const QSize TEXTURE_SIZE(512, 512);


static float vertexCube[] = {
    //---- Position------       -----Color-----
    // X       Y       Z        R     G     B

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

static float vertexPyramid[] = {

    //---- Position------      -----Color-----
    // X       Y       Z       R     G     B

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


static float vertexShape[] = {
    //---- Position------      -----Color-----
    // X       Y       Z       R     G     B
   -100.0f,  -100.0f,  100.0f,   1.0f, 0.0f, 0.0f,
    100.0f,  -100.0f,  100.0f,   1.0f, 0.0f, 0.0f,
   -100.0f,   100.0f,  100.0f,   1.0f, 0.0f, 0.0f,
   -100.0f,   100.0f,  100.0f,   1.0f, 0.0f, 0.0f,
    100.0f,  -100.0f,  100.0f,   1.0f, 0.0f, 0.0f,
    100.0f,   100.0f,  100.0f,   1.0f, 0.0f, 0.0f,
};


static float vertexRect[] = {
    //---- Position------       -----Color-----     ---UV----
    // X       Y       Z        R     G     B       U    V
   -100.0f,  -100.0f,  0.0f,   1.0f, 0.0f, 0.0f,    0.0, 0.0,
    100.0f,  -100.0f,  0.0f,   0.0f, 1.0f, 0.0f,    1.0, 0.0,
   -100.0f,   100.0f,  0.0f,   0.0f, 0.0f, 1.0f,    0.0, 1.0,
   -100.0f,   100.0f,  0.0f,   0.0f, 0.0f, 1.0f,    0.0, 1.0,
    100.0f,  -100.0f,  0.0f,   0.0f, 1.0f, 0.0f,    1.0, 0.0,
    100.0f,   100.0f,  0.0f,   1.0f, 0.0f, 1.0f,    1.0, 1.0,
};


CubesRenderer::CubesRenderer()
{
    m_modelCubes = new glm::mat4[m_Cubes];
    for (int i = 0; i < m_Cubes; i ++) {
        double ns = QRandomGenerator::global()->bounded(1, 2) * 0.5;
        double ntX = QRandomGenerator::global()->bounded(-16000, 16000);
        double ntY = QRandomGenerator::global()->bounded(-16000, 16000);
        m_modelCubes[i] = glm::mat4(1.0f);
        m_modelCubes[i] = glm::scale(m_modelCubes[i],
                                     glm::vec3(ns, ns, ns));
        m_modelCubes[i] = glm::translate(m_modelCubes[i],
                                         glm::vec3(ntX, ntY, 0));
    }

    m_modelPyramids = new glm::mat4[m_Pyramids];
    for (int i = 0; i < m_Pyramids; i ++) {
        double ns = QRandomGenerator::global()->bounded(1, 2) * 0.5;
        double ntX = QRandomGenerator::global()->bounded(-16000, 16000);
        double ntY = QRandomGenerator::global()->bounded(-16000, 16000);
        m_modelPyramids[i] = glm::mat4(1.0f);
        m_modelPyramids[i] = glm::scale(m_modelPyramids[i],
                                        glm::vec3(ns, ns, ns));
        m_modelPyramids[i] = glm::translate(m_modelPyramids[i],
                                            glm::vec3(ntX, ntY, 0));
    }

    m_modelShapes = new glm::mat4[m_Shapes];
    for (int i = 0; i < m_Shapes; i ++) {
        m_modelShapes[i] = glm::mat4(1.0f);
        m_modelShapes[i] = glm::translate(m_modelShapes[i],
                                          glm::vec3(0.0f, 0.0f, 100.0f));
    }

    m_modelRects = new glm::mat4[m_Rects];
    for (int i = 0; i < m_Rects; i ++) {
        m_modelRects[i] = glm::mat4(1.0f);
        m_modelRects[i] = glm::translate(m_modelRects[i],
                                        glm::vec3(-100.0f, 0.0f, 100.0f));
    }

    crateTextureImage();
}

CubesRenderer::~CubesRenderer()
{
    if (m_modelRects) {
        delete m_modelRects;
    }

    if (m_modelShapes) {
        delete m_modelShapes;
    }

    if (m_modelPyramids) {
        delete m_modelPyramids;
    }
    if (m_modelCubes) {
        delete m_modelCubes;
    }
}


int CubesRenderer::createCubeBuffer()
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

int CubesRenderer::createPyramidBuffer()
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

int CubesRenderer::createBezierBuffer()
{
    if (!m_rhi)
        return -1;

    m_vectexBufferShape.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                               QRhiBuffer::VertexBuffer,
                                               sizeof(vertexShape)));
    m_vectexBufferShape->create();

    m_modelBufferShape.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                              QRhiBuffer::VertexBuffer,
                                              m_Shapes*sizeof(glm::mat4)));
    m_modelBufferShape->create();

    return 0;
}

int CubesRenderer::createRectBuffer()
{
    if (!m_rhi)
        return -1;

    m_vectexBufferRect.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                                QRhiBuffer::VertexBuffer,
                                                sizeof(vertexRect)));
    m_vectexBufferRect->create();

    m_modelBufferRect.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                             QRhiBuffer::VertexBuffer,
                                             m_Rects*sizeof(glm::mat4)));
    m_modelBufferRect->create();

    return 0;
}


int CubesRenderer::createPipline1()
{
    if (!m_rhi)
        return -1;

    m_pipeline1.reset(m_rhi->newGraphicsPipeline());

    m_pipeline1->setShaderStages({
        {
            QRhiShaderStage::Vertex,
            getShader(QLatin1String(":/AnymateLab/shaders/cube.vert.qsb"))
        },
        {
            QRhiShaderStage::Fragment,
            getShader(QLatin1String(":/AnymateLab/shaders/cube.frag.qsb"))
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
            m_shaderResourceStages,
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

int CubesRenderer::createPipline2()
{
    if (!m_rhi)
        return -1;

    m_pipeline2.reset(m_rhi->newGraphicsPipeline());

    m_pipeline2->setShaderStages({
        {
            QRhiShaderStage::Vertex,
            getShader(QLatin1String(":/AnymateLab/shaders/cube.vert.qsb"))
        },
        {
            QRhiShaderStage::Fragment,
            getShader(QLatin1String(":/AnymateLab/shaders/cube.frag.qsb"))
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
            m_shaderResourceStages,
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

int CubesRenderer::createPipline3()
{
    if (!m_rhi)
        return -1;

    m_pipeline3.reset(m_rhi->newGraphicsPipeline());

    m_pipeline3->setShaderStages({
        {
            QRhiShaderStage::Vertex,
            getShader(QLatin1String(":/AnymateLab/shaders/shape.vert.qsb"))
        },
        {
            QRhiShaderStage::Fragment,
            getShader(QLatin1String(":/AnymateLab/shaders/shape.frag.qsb"))
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

    int blockSize = sizeof(glm::mat4) +
                    sizeof(glm::mat4) +
                    sizeof(int) +
                    sizeof(float) +
                    sizeof(glm::vec2) +
                    sizeof(float) +
                    sizeof(float) +
                    sizeof(float)
        ;
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
            m_shaderResourceStages,
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

    QList<QRhiGraphicsPipeline::TargetBlend> targetBlends(1);
    targetBlends[0].enable = true;
    targetBlends[0].srcColor = QRhiGraphicsPipeline::SrcAlpha;
    targetBlends[0].dstColor = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opColor = QRhiGraphicsPipeline::Add;
    targetBlends[0].srcAlpha = QRhiGraphicsPipeline::One;
    targetBlends[0].dstAlpha = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opAlpha = QRhiGraphicsPipeline::Add;
    // targetBlends[0].colorWrite = QRhiGraphicsPipeline::ColorMask(0xF);
    m_pipeline3->setTargetBlends(targetBlends.begin(), targetBlends.end());
    // m_pipeline3->setTopology(QRhiGraphicsPipeline::LineStrip);
    m_pipeline3->create();

    return 1;
}


int CubesRenderer::createPipline4()
{
    if (!m_rhi)
        return -1;

    m_pipeline4.reset(m_rhi->newGraphicsPipeline());

    m_pipeline4->setShaderStages({
        {
            QRhiShaderStage::Vertex,
            getShader(QLatin1String(":/AnymateLab/shaders/recttexture.vert.qsb"))
        },
        {
            QRhiShaderStage::Fragment,
            getShader(QLatin1String(":/AnymateLab/shaders/recttexture.frag.qsb"))
        }
    });

    QRhiVertexInputLayout inputLayout;
    inputLayout.setBindings(
        {
         { 8 * sizeof(float), QRhiVertexInputBinding::PerVertex },
         { 64, QRhiVertexInputBinding::PerInstance },
         });

    inputLayout.setAttributes(
        {
         // binding0
         { 0, 0, QRhiVertexInputAttribute::Float3, 0                 },
         { 0, 1, QRhiVertexInputAttribute::Float3, 3 * sizeof(float) },
         { 0, 2, QRhiVertexInputAttribute::Float2, 6 * sizeof(float) },
         // binding1
         { 1, 3, QRhiVertexInputAttribute::Float4, 0 },
         { 1, 4, QRhiVertexInputAttribute::Float4, 4 * sizeof(float) },
         { 1, 5, QRhiVertexInputAttribute::Float4, 8 * sizeof(float) },
         { 1, 6, QRhiVertexInputAttribute::Float4, 12 * sizeof(float) },
         });

    int blockSize = sizeof(glm::mat4) +
                    sizeof(glm::mat4)
        ;
    int bufferSize = 0;
    bufferSize = m_rhi->ubufAligned(blockSize) * m_uniformBufferBlockCount;
    m_uniformBuffer4.reset(m_rhi->newBuffer(QRhiBuffer::Dynamic,
                                            QRhiBuffer::UniformBuffer,
                                            bufferSize));
    m_uniformBuffer4->create();


    // sampler
    m_sampler.reset(m_rhi->newSampler(QRhiSampler::Linear,
                                      QRhiSampler::Linear,
                                      QRhiSampler::None,
                                      QRhiSampler::ClampToEdge,
                                      QRhiSampler::ClampToEdge));
    m_sampler->create();

    // texture
    m_texture.reset(m_rhi->newTexture(QRhiTexture::RGBA8, TEXTURE_SIZE));
    m_texture->create();

    m_srb4.reset(m_rhi->newShaderResourceBindings());
    m_srb4->setBindings({
        QRhiShaderResourceBinding::uniformBufferWithDynamicOffset(
            0,
            m_shaderResourceStages,
            m_uniformBuffer4.get(),
            bufferSize),
        QRhiShaderResourceBinding::sampledTexture(
            1,
            m_shaderResourceStages,
            m_texture.get(),
            m_sampler.get()),
    });
    m_srb4->create();

    m_pipeline4->setVertexInputLayout(inputLayout);
    m_pipeline4->setSampleCount(m_sampleCount);
    m_pipeline4->setShaderResourceBindings(m_srb4.get());
    m_pipeline4->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    m_pipeline4->setDepthTest(true);
    m_pipeline4->setDepthWrite(true);

    QList<QRhiGraphicsPipeline::TargetBlend> targetBlends(1);
    targetBlends[0].enable = true;
    targetBlends[0].srcColor = QRhiGraphicsPipeline::SrcAlpha;
    targetBlends[0].dstColor = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opColor = QRhiGraphicsPipeline::Add;
    targetBlends[0].srcAlpha = QRhiGraphicsPipeline::One;
    targetBlends[0].dstAlpha = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opAlpha = QRhiGraphicsPipeline::Add;
    m_pipeline4->setTargetBlends(targetBlends.begin(), targetBlends.end());
    m_pipeline4->create();

    return 1;
}

void CubesRenderer::crateTextureImage()
{
    m_textureImage = QImage(TEXTURE_SIZE, QImage::Format_RGBA8888);
    const QRect r(QPoint(0, 0), TEXTURE_SIZE);
    QPainter p(&m_textureImage);
    p.fillRect(r, QGradient::AmourAmour);
    QFont font;
    font.setPointSize(24);
    p.setFont(font);
    p.drawText(r, QString("Hello World"));
    p.end();
    m_textureImage = m_textureImage.mirrored(false, true);
}

void CubesRenderer::initialize(QRhiCommandBuffer *cb)
{
    if (m_rhi != rhi()) {
        m_rhi = rhi();
        // ShowFreatures(m_rhi);

        createCubeBuffer();
        createPyramidBuffer();
        createBezierBuffer();
        createRectBuffer();

        m_pipeline1.reset();
        m_pipeline2.reset();
        m_pipeline3.reset();
        m_pipeline4.reset();

    }

    ShowFreatures(m_rhi);

    if (m_sampleCount != renderTarget()->sampleCount()) {
        m_sampleCount = renderTarget()->sampleCount();
        m_pipeline1.reset();
        m_pipeline2.reset();
        m_pipeline3.reset();
        m_pipeline4.reset();
    }

    QRhiTexture *finalTex = m_sampleCount > 1 ? resolveTexture() : colorTexture();
    if (m_textureFormat != finalTex->format()) {
        m_textureFormat = finalTex->format();
        m_pipeline1.reset();
        m_pipeline2.reset();
        m_pipeline3.reset();
        m_pipeline4.reset();
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

    if (!m_pipeline4) {
        createPipline4();
    }

    QRhiResourceUpdateBatch *batch = m_rhi->nextResourceUpdateBatch();

    batch->uploadStaticBuffer(m_vectexBufferCube.get(), vertexCube);
    batch->uploadStaticBuffer(m_modelBufferCube.get(), m_modelCubes);

    batch->uploadStaticBuffer(m_vectexBufferPyramid.get(), vertexPyramid);
    batch->uploadStaticBuffer(m_modelBufferPyramid.get(), m_modelPyramids);

    batch->uploadStaticBuffer(m_vectexBufferShape.get(), vertexShape);
    batch->uploadStaticBuffer(m_modelBufferShape.get(), m_modelShapes);

    batch->uploadStaticBuffer(m_vectexBufferRect.get(), vertexRect);
    batch->uploadStaticBuffer(m_modelBufferRect.get(), m_modelRects);

    batch->uploadTexture(m_texture.get(), m_textureImage);

    cb->resourceUpdate(batch);

}

void CubesRenderer::render(QRhiCommandBuffer *cb)
{
    const QSize outputSize = renderTarget()->pixelSize();
    m_projection = m_rhi->clipSpaceCorrMatrix();

    // 所谓投影是指场景里被显示在Viewport里的内容，投影区域有“大小”和“中心点”这
    // 两个属性，从人的视觉角度上说，这两个属性决定了看到什么物体以及物体的大小。
    // 使用正交投影，left + right 和 bottom + top决定投影的大小 ，相应的，
    // left和right 决定了投影的水平中心, bottom 和 top 则决定了投影的垂直中心。
    //
    // m_projection.ortho(-outputSize.width()/(m_zoom/20) + m_orthoX,
    //                    outputSize.width()/(m_zoom/20) + m_orthoX,
    //                    -outputSize.height()/(m_zoom/20) + m_orthoY,
    //                    outputSize.height()/(m_zoom/20) + m_orthoY,
    //                    -200.0f, 10000.0f);

    m_projection.perspective(45.0f,
                             outputSize.width() / (float) outputSize.height(),
                             10.0f,
                             1000000.0f);


    // 使用透视投影，相机的z轴位置决定了场景投影范围的“大小”，相机的目标则决定了
    // 投影“中心点”。
    QVector3D cameraPos(m_focus.rx(), m_focus.ry(), 800.0f + m_zoom);
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

    // 更新 model cubes 顶点缓冲区
    for (int i = 0; i < m_Cubes; i ++) {
        glm::mat4 model = glm::rotate(m_modelCubes[i],
                                      qDegreesToRadians(m_angle),
                                      glm::vec3(1.0f, 1.0f, 0.0f));
        batch->uploadStaticBuffer(m_modelBufferCube.get(),
                                  i * sizeof(glm::mat4),
                                  sizeof(glm::mat4),
                                  &model);
    }


    // 更新 uniform buffer2 缓冲区
    batch->updateDynamicBuffer(m_uniformBuffer2.get(),
                               0,
                               64,
                               m_view.constData());
    batch->updateDynamicBuffer(m_uniformBuffer2.get(),
                               64,
                               64,
                               m_projection.constData());

    // 更新 model pyramids 顶点缓冲区
    for (int i = 0; i < m_Pyramids; i ++) {
        glm::mat4 model = glm::rotate(m_modelPyramids[i],
                                      qDegreesToRadians(m_angle),
                                      glm::vec3(0.0f, 1.0f, 1.0f));
        batch->uploadStaticBuffer(m_modelBufferPyramid.get(),
                                  i * sizeof(glm::mat4),
                                  sizeof(glm::mat4),
                                  &model);
    }

    // 更新 uniform buffer3 缓冲区
    size_t offset = 0;
    int shapeType = 2;
    float radius = 80.0;
    glm::vec2 circleCenter(0.0, 0.0);
    float thickness = 10.0;
    float smoothness = 1.0;
    float angle = qDegreesToRadians(m_angle);

    offset = align(0, STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(m_uniformBuffer3.get(),
                               offset,
                               sizeof(glm::mat4),
                               m_view.constData());

    offset = align(sizeof(glm::mat4), STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(m_uniformBuffer3.get(),
                               offset,
                               sizeof(glm::mat4),
                               m_projection.constData());

    offset = align(sizeof(glm::mat4) + sizeof(glm::mat4), STD140_ALIGN_INT);
    batch->updateDynamicBuffer(m_uniformBuffer3.get(),
                               offset,
                               sizeof(int),
                               &shapeType);

    offset = align(sizeof(glm::mat4) + sizeof(glm::mat4) + sizeof(int),
        STD140_ALIGN_FLOAT);
    batch->updateDynamicBuffer(m_uniformBuffer3.get(),
                               offset,
                               sizeof(float),
                               &radius);

    offset = align(sizeof(glm::mat4) + sizeof(glm::mat4) + sizeof(int) +
                    sizeof(float), STD140_ALIGN_VEC2);
    batch->updateDynamicBuffer(m_uniformBuffer3.get(),
                               offset,
                               sizeof(glm::vec2),
                               &circleCenter);

    offset = align(sizeof(glm::mat4) + sizeof(glm::mat4) + sizeof(int) +
                       sizeof(float) + sizeof(glm::vec2), STD140_ALIGN_FLOAT);
    batch->updateDynamicBuffer(m_uniformBuffer3.get(),
                               offset,
                               sizeof(float),
                               &thickness);

    offset = align(sizeof(glm::mat4) + sizeof(glm::mat4) + sizeof(int) +
                       sizeof(float) + sizeof(glm::vec2) + sizeof(float),
                   STD140_ALIGN_FLOAT);
    batch->updateDynamicBuffer(m_uniformBuffer3.get(),
                               offset,
                               sizeof(float),
                               &smoothness);

    offset = align(sizeof(glm::mat4) + sizeof(glm::mat4) + sizeof(int) +
                       sizeof(float) + sizeof(glm::vec2) + sizeof(float) +
                       sizeof(float),
                   STD140_ALIGN_FLOAT);
    batch->updateDynamicBuffer(m_uniformBuffer3.get(),
                               offset,
                               sizeof(float),
                               &angle);

    // 更新 model shapes 顶点缓冲区
    for (int i = 0; i < m_Shapes; i ++) {
        glm::mat4 model = glm::rotate(m_modelShapes[i],
                                      qDegreesToRadians(m_angle),
                                      glm::vec3(0.0f, 1.0f, 1.0f));
        batch->uploadStaticBuffer(m_modelBufferShape.get(),
                                  i * sizeof(glm::mat4),
                                  sizeof(glm::mat4),
                                  &model);
    }

    // 更新 uniform buffer4 缓冲区
    offset = align(0, STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(m_uniformBuffer4.get(),
                               offset,
                               sizeof(glm::mat4),
                               m_view.constData());

    offset = align(sizeof(glm::mat4), STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(m_uniformBuffer4.get(),
                               offset,
                               sizeof(glm::mat4),
                               m_projection.constData());

    // 更新 model rects 顶点缓冲区
    for (int i = 0; i < m_Rects; i ++) {
        glm::mat4 model = glm::rotate(m_modelRects[i],
                                      qDegreesToRadians(m_angle),
                                      glm::vec3(0.0f, 0.0f, 1.0f));
        batch->uploadStaticBuffer(m_modelBufferRect.get(),
                                  i * sizeof(glm::mat4),
                                  sizeof(glm::mat4),
                                  &model);
    }

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
        { m_vectexBufferShape.get(), 0 },
        { m_modelBufferShape.get(), 0 }
    };
    cb->setVertexInput(0, 2, inputBindings3);
    cb->draw(6, m_Shapes);


    // 使用4号管线绘制贴图矩形
    cb->setGraphicsPipeline(m_pipeline4.get());
    cb->setShaderResources(m_srb4.get());
    // 绑定顶点属性缓冲区
    const QRhiCommandBuffer::VertexInput inputBindings4[] = {
        { m_vectexBufferRect.get(), 0 },
        { m_modelBufferRect.get(), 0 }
    };
    cb->setVertexInput(0, 2, inputBindings4);
    cb->draw(6, m_Rects);

    cb->endPass();
}

void CubesRenderer::synchronize(QQuickRhiItem *rhiItem)
{
    Cubes *item = static_cast<Cubes *>(rhiItem);
    if (item->angle() != m_angle)
        m_angle = item->angle();

    m_orthoX = item->getOrthoX();
    m_orthoY = item->getOrthoY();
    m_zoom = item->getZoom();
    m_focus = item->getFocus();
}

Cubes::Cubes()
{
    setFocusPolicy(Qt::ClickFocus);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(ItemAcceptsInputMethod, true);
}

Cubes:: ~ Cubes()
{

}

QQuickRhiItemRenderer* Cubes::createRenderer()
{
    return new CubesRenderer();
}

