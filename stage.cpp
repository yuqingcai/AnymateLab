#include "stage.h"
#include <QFile>
#include <QCursor>
#include <QRandomGenerator>
#include <QtMath>

static QShader getShader(const QString &name)
{
    QFile f(name);
    return f.open(QIODevice::ReadOnly) ? QShader::fromSerialized(f.readAll()) : QShader();
}

float vertexData1[] = {
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


float vertexData2[] = {

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
    m_models1 = new glm::mat4[m_instances1];
    for (int i = 0; i < m_instances1; i ++) {
        double ns = QRandomGenerator::global()->bounded(1, 2) * 0.5;
        double ntX = QRandomGenerator::global()->bounded(-16000, 16000);
        double ntY = QRandomGenerator::global()->bounded(-16000, 16000);
        m_models1[i] = glm::mat4(1.0f);
        m_models1[i] = glm::scale(m_models1[i], glm::vec3(ns, ns, ns));
        m_models1[i] = glm::translate(m_models1[i],
                                      glm::vec3(ntX, ntY, 0));
    }

    m_models2 = new glm::mat4[m_instances2];
    for (int i = 0; i < m_instances2; i ++) {
        double ns = QRandomGenerator::global()->bounded(1, 2) * 0.5;
        double ntX = QRandomGenerator::global()->bounded(-16000, 16000);
        double ntY = QRandomGenerator::global()->bounded(-16000, 16000);
        m_models2[i] = glm::mat4(1.0f);
        m_models2[i] = glm::scale(m_models2[i], glm::vec3(ns, ns, ns));
        m_models2[i] = glm::translate(m_models2[i],
                                      glm::vec3(ntX, ntY, 0));
    }
}

StageRenderer::~StageRenderer()
{
    if (m_models1) {
        delete m_models1;
    }
    if (m_models2) {
        delete m_models2;
    }
}

void StageRenderer::initialize(QRhiCommandBuffer *cb)
{
    if (m_rhi != rhi()) {
        m_rhi = rhi();

        m_pipeline.reset();

        qDebug("pipline reset");
        qDebug("Backend: %s",                       m_rhi->backendName());
        qDebug("TextureSizeMin: %d",                m_rhi->resourceLimit(QRhi::TextureSizeMin));
        qDebug("TextureSizeMax: %d",                m_rhi->resourceLimit(QRhi::TextureSizeMax));
        qDebug("MaxColorAttachments: %d",           m_rhi->resourceLimit(QRhi::MaxColorAttachments));
        qDebug("FramesInFlight: %d",                m_rhi->resourceLimit(QRhi::FramesInFlight));
        qDebug("MaxAsyncReadbackFrames: %d",        m_rhi->resourceLimit(QRhi::MaxAsyncReadbackFrames));
        qDebug("MaxThreadGroupsPerDimension: %d",   m_rhi->resourceLimit(QRhi::MaxThreadGroupsPerDimension));
        qDebug("MaxThreadsPerThreadGroup: %d",      m_rhi->resourceLimit(QRhi::MaxThreadsPerThreadGroup));
        qDebug("MaxThreadGroupX: %d",               m_rhi->resourceLimit(QRhi::MaxThreadGroupX));
        qDebug("MaxThreadGroupY: %d",               m_rhi->resourceLimit(QRhi::MaxThreadGroupY));
        qDebug("MaxThreadGroupZ: %d",               m_rhi->resourceLimit(QRhi::MaxThreadGroupZ));
        qDebug("TextureArraySizeMax: %d",           m_rhi->resourceLimit(QRhi::TextureArraySizeMax));
        qDebug("MaxUniformBufferRange: %d",         m_rhi->resourceLimit(QRhi::MaxUniformBufferRange));
        qDebug("MaxVertexInputs: %d",               m_rhi->resourceLimit(QRhi::MaxVertexInputs));
        qDebug("MaxVertexOutputs: %d",              m_rhi->resourceLimit(QRhi::MaxVertexOutputs));

        if (m_rhi->isFeatureSupported(QRhi::MultisampleTexture)) {
            qDebug("MultisampleTexture supported");
        }
        else {
            qDebug("MultisampleTexture not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::MultisampleRenderBuffer)) {
            qDebug("MultisampleRenderBuffer supported");
        }
        else {
            qDebug("MultisampleRenderBuffer not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::DebugMarkers)) {
            qDebug("DebugMarkers supported");
        }
        else {
            qDebug("DebugMarkers not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::Timestamps)) {
            qDebug("Timestamps supported");
        }
        else {
            qDebug("Timestamps not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::Instancing)) {
            qDebug("Instancing supported");
        }
        else {
            qDebug("Instancing not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::CustomInstanceStepRate)) {
            qDebug("CustomInstanceStepRate supported");
        }
        else {
            qDebug("CustomInstanceStepRate not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::PrimitiveRestart)) {
            qDebug("PrimitiveRestart supported");
        }
        else {
            qDebug("PrimitiveRestart not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::NonDynamicUniformBuffers)) {
            qDebug("NonDynamicUniformBuffers supported");
        }
        else {
            qDebug("NonDynamicUniformBuffers not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::NonFourAlignedEffectiveIndexBufferOffset)) {
            qDebug("NonFourAlignedEffectiveIndexBufferOffset supported");
        }
        else {
            qDebug("NonFourAlignedEffectiveIndexBufferOffset not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::NPOTTextureRepeat)) {
            qDebug("NPOTTextureRepeat supported");
        }
        else {
            qDebug("NPOTTextureRepeat not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::RedOrAlpha8IsRed)) {
            qDebug("RedOrAlpha8IsRed supported");
        }
        else {
            qDebug("RedOrAlpha8IsRed not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::ElementIndexUint)) {
            qDebug("ElementIndexUint supported");
        }
        else {
            qDebug("ElementIndexUint not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::Compute)) {
            qDebug("Compute supported");
        }
        else {
            qDebug("Compute not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::WideLines)) {
            qDebug("WideLines supported");
        }
        else {
            qDebug("WideLines not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::VertexShaderPointSize)) {
            qDebug("VertexShaderPointSize supported");
        }
        else {
            qDebug("VertexShaderPointSize not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::BaseVertex)) {
            qDebug("BaseVertex supported");
        }
        else {
            qDebug("BaseVertex not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::BaseInstance)) {
            qDebug("BaseInstance supported");
        }
        else {
            qDebug("BaseInstance not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::TriangleFanTopology)) {
            qDebug("TriangleFanTopology supported");
        }
        else {
            qDebug("TriangleFanTopology not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::ReadBackNonUniformBuffer)) {
            qDebug("ReadBackNonUniformBuffer supported");
        }
        else {
            qDebug("ReadBackNonUniformBuffer not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::ReadBackNonBaseMipLevel)) {
            qDebug("ReadBackNonBaseMipLevel supported");
        }
        else {
            qDebug("ReadBackNonBaseMipLevel not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::TexelFetch)) {
            qDebug("TexelFetch supported");
        }
        else {
            qDebug("TexelFetch not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::RenderToNonBaseMipLevel)) {
            qDebug("RenderToNonBaseMipLevel supported");
        }
        else {
            qDebug("RenderToNonBaseMipLevel not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::IntAttributes)) {
            qDebug("IntAttributes supported");
        }
        else {
            qDebug("IntAttributes not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::ScreenSpaceDerivatives)) {
            qDebug("ScreenSpaceDerivatives supported");
        }
        else {
            qDebug("ScreenSpaceDerivatives not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::ReadBackAnyTextureFormat)) {
            qDebug("ReadBackAnyTextureFormat supported");
        }
        else {
            qDebug("ReadBackAnyTextureFormat not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::PipelineCacheDataLoadSave)) {
            qDebug("PipelineCacheDataLoadSave supported");
        }
        else {
            qDebug("PipelineCacheDataLoadSave not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::ImageDataStride)) {
            qDebug("ImageDataStride supported");
        }
        else {
            qDebug("ImageDataStride not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::RenderBufferImport)) {
            qDebug("RenderBufferImport supported");
        }
        else {
            qDebug("RenderBufferImport not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::ThreeDimensionalTextures)) {
            qDebug("ThreeDimensionalTextures supported");
        }
        else {
            qDebug("ThreeDimensionalTextures not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::RenderTo3DTextureSlice)) {
            qDebug("RenderTo3DTextureSlice supported");
        }
        else {
            qDebug("RenderTo3DTextureSlice not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::TextureArrays)) {
            qDebug("TextureArrays supported");
        }
        else {
            qDebug("TextureArrays not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::Tessellation)) {
            qDebug("Tessellation supported");
        }
        else {
            qDebug("Tessellation not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::GeometryShader)) {
            qDebug("GeometryShader supported");
        }
        else {
            qDebug("GeometryShader not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::TextureArrayRange)) {
            qDebug("TextureArrayRange supported");
        }
        else {
            qDebug("TextureArrayRange not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::NonFillPolygonMode)) {
            qDebug("NonFillPolygonMode supported");
        }
        else {
            qDebug("NonFillPolygonMode not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::OneDimensionalTextures)) {
            qDebug("OneDimensionalTextures supported");
        }
        else {
            qDebug("OneDimensionalTextures not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::OneDimensionalTextureMipmaps)) {
            qDebug("OneDimensionalTextureMipmaps supported");
        }
        else {
            qDebug("OneDimensionalTextureMipmaps not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::HalfAttributes)) {
            qDebug("HalfAttributes supported");
        }
        else {
            qDebug("HalfAttributes not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::RenderToOneDimensionalTexture)) {
            qDebug("RenderToOneDimensionalTexture supported");
        }
        else {
            qDebug("RenderToOneDimensionalTexture not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::ThreeDimensionalTextureMipmaps)) {
            qDebug("ThreeDimensionalTextureMipmaps supported");
        }
        else {
            qDebug("ThreeDimensionalTextureMipmaps not supported");
        }
        if (m_rhi->isFeatureSupported(QRhi::MultiView)) {
            qDebug("MultiView supported");
        }
        else {
            qDebug("MultiView not supported");
        }
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
        m_pipeline.reset(m_rhi->newGraphicsPipeline());

        m_vectexBuffer1.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                              QRhiBuffer::VertexBuffer,
                                              sizeof(vertexData1)));
        m_vectexBuffer1->create();


        m_vectexBuffer2.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                               QRhiBuffer::VertexBuffer,
                                               sizeof(vertexData2)));
        m_vectexBuffer2->create();

        m_modelBuffer1.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                             QRhiBuffer::VertexBuffer,
                                             m_instances1*sizeof(glm::mat4)));
        m_modelBuffer1->create();

        m_modelBuffer2.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                              QRhiBuffer::VertexBuffer,
                                              m_instances2*sizeof(glm::mat4)));
        m_modelBuffer2->create();

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

        m_pipeline->setShaderStages({
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
        inputLayout.setBindings({
                                 // 顶点着色模式（PerVertex Mode）：顶点属性将为每个顶点更新。这是常见
                                 // 的模式，用于正常的非实例化渲染，每个顶点属性会随着顶点的变化而变化。
                                 // 实例化着色模式（PerInstance Mode）：顶点属性将为每个实例更新一次。
                                 // 这种模式在实例化渲染中很有用，适用于每个实例使用相同的顶点属性，但是
                                 // 每个实例之间的属性可能不同。

                                 // PerVertex 表示该顶点属性的索引是以“顶点”进行递增的
                                 { 6 * sizeof(float), QRhiVertexInputBinding::PerVertex },
                                 // PerInstance 表示该顶点属性的索引是以“实例”进行递增的
                                 { 64, QRhiVertexInputBinding::PerInstance },
                                 });

        inputLayout.setAttributes({
                                   // binding0
                                   { 0, 0, QRhiVertexInputAttribute::Float3, 0 },
                                   { 0, 1, QRhiVertexInputAttribute::Float3, 3 * sizeof(float) },
                                   // binding1
                                   // model矩阵由4个 vec4构成，每个vec4代表一列，从location2开始，每个列对应一个location。
                                   { 1, 2, QRhiVertexInputAttribute::Float4, 0 },
                                   { 1, 3, QRhiVertexInputAttribute::Float4, 4 * sizeof(float) },
                                   { 1, 4, QRhiVertexInputAttribute::Float4, 8 * sizeof(float) },
                                   { 1, 5, QRhiVertexInputAttribute::Float4, 12 * sizeof(float) },
                                   });
        m_pipeline->setVertexInputLayout(inputLayout);

        m_pipeline->setSampleCount(m_sampleCount);
        m_pipeline->setShaderResourceBindings(m_srb.get());
        m_pipeline->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
        m_pipeline->setDepthTest(true);
        m_pipeline->setDepthWrite(true);
        // 绘制线条模式
        // m_pipeline->setTopology(QRhiGraphicsPipeline::Lines);
        m_pipeline->create();

        QRhiResourceUpdateBatch *batch = m_rhi->nextResourceUpdateBatch();
        batch->uploadStaticBuffer(m_vectexBuffer1.get(), vertexData1);
        batch->uploadStaticBuffer(m_vectexBuffer2.get(), vertexData2);
        batch->uploadStaticBuffer(m_modelBuffer1.get(), m_models1);
        batch->uploadStaticBuffer(m_modelBuffer2.get(), m_models2);

        cb->resourceUpdate(batch);

    }

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

    cb->setGraphicsPipeline(m_pipeline.get());
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

    for (int i = 0; i < m_instances1; i ++) {
        glm::mat4 model = glm::rotate(m_models1[i],
                                      qDegreesToRadians(m_angle),
                                      glm::vec3(1.0f, 1.0f, 0.0f));

        batch->uploadStaticBuffer(m_modelBuffer1.get(),
                                  i * sizeof(glm::mat4),
                                  sizeof(glm::mat4),
                                  &model);
    }

    for (int i = 0; i < m_instances2; i ++) {
        glm::mat4 model = glm::rotate(m_models2[i],
                                      qDegreesToRadians(m_angle),
                                      glm::vec3(0.0f, 1.0f, 1.0f));
        batch->uploadStaticBuffer(m_modelBuffer2.get(),
                                  i * sizeof(glm::mat4),
                                  sizeof(glm::mat4),
                                  &model);
    }

    // // 更新
    cb->resourceUpdate(batch);
    cb->setShaderResources(m_srb.get());

    const QRhiCommandBuffer::VertexInput vbufBindings1[] = {
        { m_vectexBuffer1.get(), 0 },
        { m_modelBuffer1.get(), 0 }
    };
    cb->setVertexInput(0, 2, vbufBindings1);
    // 绘制实例，由6个顶点构成，顶点属性数据从偏移量0开始，实例id为0。这里需要注意，实例
    // id是很重要的一个参数，它用于着色器索引 model 矩阵的数据。在本例中 model 矩阵数据
    // 是以顶点属性数组绑定到着色器，并且被声明为以PerInstance模式进行绑定，意思是每绘制
    // 完一个实例（而不是每绘制完一个顶点），model顶点属性的索引才会进行一次递增，这样就能
    // 做到每个绘制的一个实例对应 model 顶点属性数组里的一个 model 矩阵。
    cb->draw(36, m_instances1);

    const QRhiCommandBuffer::VertexInput vbufBindings2[] = {
        { m_vectexBuffer2.get(), 0 },
        { m_modelBuffer2.get(), 0 }
    };
    cb->setVertexInput(0, 2, vbufBindings2);
    cb->draw(18, m_instances2);

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
