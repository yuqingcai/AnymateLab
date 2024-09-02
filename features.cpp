#include "features.h"

void ShowFreatures(QRhi *rhi)
{
    qDebug("pipline reset");
    qDebug("Backend: %s",                       rhi->backendName());
    qDebug("TextureSizeMin: %d",                rhi->resourceLimit(QRhi::TextureSizeMin));
    qDebug("TextureSizeMax: %d",                rhi->resourceLimit(QRhi::TextureSizeMax));
    qDebug("MaxColorAttachments: %d",           rhi->resourceLimit(QRhi::MaxColorAttachments));
    qDebug("FramesInFlight: %d",                rhi->resourceLimit(QRhi::FramesInFlight));
    qDebug("MaxAsyncReadbackFrames: %d",        rhi->resourceLimit(QRhi::MaxAsyncReadbackFrames));
    qDebug("MaxThreadGroupsPerDimension: %d",   rhi->resourceLimit(QRhi::MaxThreadGroupsPerDimension));
    qDebug("MaxThreadsPerThreadGroup: %d",      rhi->resourceLimit(QRhi::MaxThreadsPerThreadGroup));
    qDebug("MaxThreadGroupX: %d",               rhi->resourceLimit(QRhi::MaxThreadGroupX));
    qDebug("MaxThreadGroupY: %d",               rhi->resourceLimit(QRhi::MaxThreadGroupY));
    qDebug("MaxThreadGroupZ: %d",               rhi->resourceLimit(QRhi::MaxThreadGroupZ));
    qDebug("TextureArraySizeMax: %d",           rhi->resourceLimit(QRhi::TextureArraySizeMax));
    qDebug("MaxUniformBufferRange: %d",         rhi->resourceLimit(QRhi::MaxUniformBufferRange));
    qDebug("MaxVertexInputs: %d",               rhi->resourceLimit(QRhi::MaxVertexInputs));
    qDebug("MaxVertexOutputs: %d",              rhi->resourceLimit(QRhi::MaxVertexOutputs));

    if (rhi->isFeatureSupported(QRhi::MultisampleTexture)) {
        qDebug("MultisampleTexture supported");
    }
    else {
        qDebug("MultisampleTexture not supported");
    }
    if (rhi->isFeatureSupported(QRhi::MultisampleRenderBuffer)) {
        qDebug("MultisampleRenderBuffer supported");
    }
    else {
        qDebug("MultisampleRenderBuffer not supported");
    }
    if (rhi->isFeatureSupported(QRhi::DebugMarkers)) {
        qDebug("DebugMarkers supported");
    }
    else {
        qDebug("DebugMarkers not supported");
    }
    if (rhi->isFeatureSupported(QRhi::Timestamps)) {
        qDebug("Timestamps supported");
    }
    else {
        qDebug("Timestamps not supported");
    }
    if (rhi->isFeatureSupported(QRhi::Instancing)) {
        qDebug("Instancing supported");
    }
    else {
        qDebug("Instancing not supported");
    }
    if (rhi->isFeatureSupported(QRhi::CustomInstanceStepRate)) {
        qDebug("CustomInstanceStepRate supported");
    }
    else {
        qDebug("CustomInstanceStepRate not supported");
    }
    if (rhi->isFeatureSupported(QRhi::PrimitiveRestart)) {
        qDebug("PrimitiveRestart supported");
    }
    else {
        qDebug("PrimitiveRestart not supported");
    }
    if (rhi->isFeatureSupported(QRhi::NonDynamicUniformBuffers)) {
        qDebug("NonDynamicUniformBuffers supported");
    }
    else {
        qDebug("NonDynamicUniformBuffers not supported");
    }
    if (rhi->isFeatureSupported(QRhi::NonFourAlignedEffectiveIndexBufferOffset)) {
        qDebug("NonFourAlignedEffectiveIndexBufferOffset supported");
    }
    else {
        qDebug("NonFourAlignedEffectiveIndexBufferOffset not supported");
    }
    if (rhi->isFeatureSupported(QRhi::NPOTTextureRepeat)) {
        qDebug("NPOTTextureRepeat supported");
    }
    else {
        qDebug("NPOTTextureRepeat not supported");
    }
    if (rhi->isFeatureSupported(QRhi::RedOrAlpha8IsRed)) {
        qDebug("RedOrAlpha8IsRed supported");
    }
    else {
        qDebug("RedOrAlpha8IsRed not supported");
    }
    if (rhi->isFeatureSupported(QRhi::ElementIndexUint)) {
        qDebug("ElementIndexUint supported");
    }
    else {
        qDebug("ElementIndexUint not supported");
    }
    if (rhi->isFeatureSupported(QRhi::Compute)) {
        qDebug("Compute supported");
    }
    else {
        qDebug("Compute not supported");
    }
    if (rhi->isFeatureSupported(QRhi::WideLines)) {
        qDebug("WideLines supported");
    }
    else {
        qDebug("WideLines not supported");
    }
    if (rhi->isFeatureSupported(QRhi::VertexShaderPointSize)) {
        qDebug("VertexShaderPointSize supported");
    }
    else {
        qDebug("VertexShaderPointSize not supported");
    }
    if (rhi->isFeatureSupported(QRhi::BaseVertex)) {
        qDebug("BaseVertex supported");
    }
    else {
        qDebug("BaseVertex not supported");
    }
    if (rhi->isFeatureSupported(QRhi::BaseInstance)) {
        qDebug("BaseInstance supported");
    }
    else {
        qDebug("BaseInstance not supported");
    }
    if (rhi->isFeatureSupported(QRhi::TriangleFanTopology)) {
        qDebug("TriangleFanTopology supported");
    }
    else {
        qDebug("TriangleFanTopology not supported");
    }
    if (rhi->isFeatureSupported(QRhi::ReadBackNonUniformBuffer)) {
        qDebug("ReadBackNonUniformBuffer supported");
    }
    else {
        qDebug("ReadBackNonUniformBuffer not supported");
    }
    if (rhi->isFeatureSupported(QRhi::ReadBackNonBaseMipLevel)) {
        qDebug("ReadBackNonBaseMipLevel supported");
    }
    else {
        qDebug("ReadBackNonBaseMipLevel not supported");
    }
    if (rhi->isFeatureSupported(QRhi::TexelFetch)) {
        qDebug("TexelFetch supported");
    }
    else {
        qDebug("TexelFetch not supported");
    }
    if (rhi->isFeatureSupported(QRhi::RenderToNonBaseMipLevel)) {
        qDebug("RenderToNonBaseMipLevel supported");
    }
    else {
        qDebug("RenderToNonBaseMipLevel not supported");
    }
    if (rhi->isFeatureSupported(QRhi::IntAttributes)) {
        qDebug("IntAttributes supported");
    }
    else {
        qDebug("IntAttributes not supported");
    }
    if (rhi->isFeatureSupported(QRhi::ScreenSpaceDerivatives)) {
        qDebug("ScreenSpaceDerivatives supported");
    }
    else {
        qDebug("ScreenSpaceDerivatives not supported");
    }
    if (rhi->isFeatureSupported(QRhi::ReadBackAnyTextureFormat)) {
        qDebug("ReadBackAnyTextureFormat supported");
    }
    else {
        qDebug("ReadBackAnyTextureFormat not supported");
    }
    if (rhi->isFeatureSupported(QRhi::PipelineCacheDataLoadSave)) {
        qDebug("PipelineCacheDataLoadSave supported");
    }
    else {
        qDebug("PipelineCacheDataLoadSave not supported");
    }
    if (rhi->isFeatureSupported(QRhi::ImageDataStride)) {
        qDebug("ImageDataStride supported");
    }
    else {
        qDebug("ImageDataStride not supported");
    }
    if (rhi->isFeatureSupported(QRhi::RenderBufferImport)) {
        qDebug("RenderBufferImport supported");
    }
    else {
        qDebug("RenderBufferImport not supported");
    }
    if (rhi->isFeatureSupported(QRhi::ThreeDimensionalTextures)) {
        qDebug("ThreeDimensionalTextures supported");
    }
    else {
        qDebug("ThreeDimensionalTextures not supported");
    }
    if (rhi->isFeatureSupported(QRhi::RenderTo3DTextureSlice)) {
        qDebug("RenderTo3DTextureSlice supported");
    }
    else {
        qDebug("RenderTo3DTextureSlice not supported");
    }
    if (rhi->isFeatureSupported(QRhi::TextureArrays)) {
        qDebug("TextureArrays supported");
    }
    else {
        qDebug("TextureArrays not supported");
    }
    if (rhi->isFeatureSupported(QRhi::Tessellation)) {
        qDebug("Tessellation supported");
    }
    else {
        qDebug("Tessellation not supported");
    }
    if (rhi->isFeatureSupported(QRhi::GeometryShader)) {
        qDebug("GeometryShader supported");
    }
    else {
        qDebug("GeometryShader not supported");
    }
    if (rhi->isFeatureSupported(QRhi::TextureArrayRange)) {
        qDebug("TextureArrayRange supported");
    }
    else {
        qDebug("TextureArrayRange not supported");
    }
    if (rhi->isFeatureSupported(QRhi::NonFillPolygonMode)) {
        qDebug("NonFillPolygonMode supported");
    }
    else {
        qDebug("NonFillPolygonMode not supported");
    }
    if (rhi->isFeatureSupported(QRhi::OneDimensionalTextures)) {
        qDebug("OneDimensionalTextures supported");
    }
    else {
        qDebug("OneDimensionalTextures not supported");
    }
    if (rhi->isFeatureSupported(QRhi::OneDimensionalTextureMipmaps)) {
        qDebug("OneDimensionalTextureMipmaps supported");
    }
    else {
        qDebug("OneDimensionalTextureMipmaps not supported");
    }
    if (rhi->isFeatureSupported(QRhi::HalfAttributes)) {
        qDebug("HalfAttributes supported");
    }
    else {
        qDebug("HalfAttributes not supported");
    }
    if (rhi->isFeatureSupported(QRhi::RenderToOneDimensionalTexture)) {
        qDebug("RenderToOneDimensionalTexture supported");
    }
    else {
        qDebug("RenderToOneDimensionalTexture not supported");
    }
    if (rhi->isFeatureSupported(QRhi::ThreeDimensionalTextureMipmaps)) {
        qDebug("ThreeDimensionalTextureMipmaps supported");
    }
    else {
        qDebug("ThreeDimensionalTextureMipmaps not supported");
    }
    if (rhi->isFeatureSupported(QRhi::MultiView)) {
        qDebug("MultiView supported");
    }
    else {
        qDebug("MultiView not supported");
    }
}
