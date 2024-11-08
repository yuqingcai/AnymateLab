#include "texture.h"
#include <QtMath>
#include <QPainter>
#include <QDebug>


static const QSize TEXTURE_SIZE(512, 512);


static float vertexRect[] = {
    //---- Position------       -----Color-----     ---UV----
    // X       Y       Z        R     G     B       U    V
    -100.0f,  -100.0f,  0.0f,   1.0f, 0.0f, 0.0f,   0.0, 0.0,
    100.0f,  -100.0f,  0.0f,   0.0f, 1.0f, 0.0f,    1.0, 0.0,
    -100.0f,   100.0f,  0.0f,   0.0f, 0.0f, 1.0f,   0.0, 1.0,
    -100.0f,   100.0f,  0.0f,   0.0f, 0.0f, 1.0f,   0.0, 1.0,
    100.0f,  -100.0f,  0.0f,   0.0f, 1.0f, 0.0f,    1.0, 0.0,
    100.0f,   100.0f,  0.0f,   1.0f, 0.0f, 1.0f,    1.0, 1.0,
};


TextureRenderer::TextureRenderer()
{
    _rects = 1;

}

TextureRenderer::~TextureRenderer()
{
}

void TextureRenderer::render(QRhiCommandBuffer *cb)
{
    const QSize outputSize = renderTarget()->pixelSize();
    _projection = _rhi->clipSpaceCorrMatrix();
    _projection.perspective(45.0f,
                            outputSize.width() / (float) outputSize.height(),
                            10.0f,
                            1000000.0f);

    QVector3D cameraPos(_focus.rx(), _focus.ry(), 800.0f + _zoom);
    QVector3D cameraTarget(_focus.rx(), _focus.ry(), 0.0f);
    QVector3D cameraUp(0.0f, 1.0f, 0.0f);
    _view.setToIdentity();
    _view.lookAt(cameraPos, cameraTarget, cameraUp);

    QRhiResourceUpdateBatch *batch = _rhi->nextResourceUpdateBatch();

    const QColor clearColor = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f);
    cb->beginPass(renderTarget(), clearColor, { 1.0f, 0 }, batch);

    cb->setViewport(QRhiViewport(0, 0, outputSize.width(), outputSize.height()));

    // 更新 uniform buffer 缓冲区（view 和 projection 矩阵）
    size_t offset = 0;
    offset = align(0, STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(_uniformBuffer0.get(),
                               offset,
                               sizeof(glm::mat4),
                               _view.constData());

    offset = align(sizeof(glm::mat4), STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(_uniformBuffer0.get(),
                               offset,
                               sizeof(glm::mat4),
                               _projection.constData());

    // 更新 model rects 顶点缓冲区
    for (int i = 0; i < _rects; i ++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, qDegreesToRadians(_angle),
                            glm::vec3(0.0f, 1.0f, 0.0f));
        batch->uploadStaticBuffer(_modelBuffer0.get(),
                                  i * sizeof(glm::mat4),
                                  sizeof(glm::mat4),
                                  &model);
    }

    // QImage textureImage;
    // batch->uploadTexture(_texture.get(), textureImage);

    cb->resourceUpdate(batch);

    // 使用4号管线绘制贴图矩形
    cb->setGraphicsPipeline(_pipeline0.get());
    cb->setShaderResources(_srb0.get());
    // 绑定顶点属性缓冲区
    const QRhiCommandBuffer::VertexInput inputBindings4[] = {
        { _vectexBuffer0.get(), 0 },
        { _modelBuffer0.get(), 0 }
    };
    cb->setVertexInput(0, 2, inputBindings4);
    cb->draw(6, _rects);

    cb->endPass();

}

int TextureRenderer::createBuffer0()
{
    if (!_rhi)
        return -1;

    _vectexBuffer0.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                         QRhiBuffer::VertexBuffer,
                                         sizeof(vertexRect)));
    _vectexBuffer0->create();

    _modelBuffer0.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                        QRhiBuffer::VertexBuffer,
                                        sizeof(glm::mat4)));
    _modelBuffer0->create();

    return 0;
}

int TextureRenderer::createPipline0()
{
    if (!_rhi)
        return -1;

    _pipeline0.reset(_rhi->newGraphicsPipeline());

    _pipeline0->setShaderStages({
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
    bufferSize = _rhi->ubufAligned(blockSize) * _uniformBufferBlockCount;
    _uniformBuffer0.reset(_rhi->newBuffer(QRhiBuffer::Dynamic,
                                          QRhiBuffer::UniformBuffer,
                                          bufferSize));
    _uniformBuffer0->create();


    // sampler
    _sampler.reset(_rhi->newSampler(QRhiSampler::Linear,
                                    QRhiSampler::Linear,
                                    QRhiSampler::None,
                                    QRhiSampler::ClampToEdge,
                                    QRhiSampler::ClampToEdge));
    _sampler->create();

    // texture
    _texture.reset(_rhi->newTexture(QRhiTexture::RGBA8,
                                    TEXTURE_SIZE,
                                    1,
                                    QRhiTexture::RenderTarget |
                                        QRhiTexture::UsedAsTransferSource));
    _texture->create();

    _srb0.reset(_rhi->newShaderResourceBindings());
    _srb0->setBindings({
        QRhiShaderResourceBinding::uniformBufferWithDynamicOffset(
            0,
            _shaderResourceStages,
            _uniformBuffer0.get(),
            bufferSize),
        QRhiShaderResourceBinding::sampledTexture(
            1,
            _shaderResourceStages,
            _texture.get(),
            _sampler.get()),
    });
    _srb0->create();

    _pipeline0->setVertexInputLayout(inputLayout);
    _pipeline0->setSampleCount(_sampleCount);
    _pipeline0->setShaderResourceBindings(_srb0.get());
    _pipeline0->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    _pipeline0->setDepthTest(true);
    _pipeline0->setDepthWrite(true);

    QList<QRhiGraphicsPipeline::TargetBlend> targetBlends(1);
    targetBlends[0].enable = true;
    targetBlends[0].srcColor = QRhiGraphicsPipeline::SrcAlpha;
    targetBlends[0].dstColor = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opColor = QRhiGraphicsPipeline::Add;
    targetBlends[0].srcAlpha = QRhiGraphicsPipeline::One;
    targetBlends[0].dstAlpha = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opAlpha = QRhiGraphicsPipeline::Add;
    _pipeline0->setTargetBlends(targetBlends.begin(), targetBlends.end());
    _pipeline0->create();

    return 1;
}

void TextureRenderer::crateTextureImage()
{
    _textureImage = QImage(TEXTURE_SIZE, QImage::Format_RGBA8888);
    const QRect r(QPoint(0, 0), TEXTURE_SIZE);
    QPainter p(&_textureImage);
    p.fillRect(r, QGradient::AmourAmour);
    QFont font;
    font.setPointSize(24);
    p.setFont(font);
    p.drawText(r, QString("Hello World"));
    p.end();
    _textureImage = _textureImage.mirrored(false, true);
}

void TextureRenderer::initialize(QRhiCommandBuffer *cb)
{

    if (_rhi != rhi()) {
        _rhi = rhi();
        createBuffer0();
        _pipeline0.reset();
    }

    if (_sampleCount != renderTarget()->sampleCount()) {
        _sampleCount = renderTarget()->sampleCount();
        _pipeline0.reset();
    }

    QRhiTexture *finalTex = _sampleCount > 1 ? resolveTexture() : colorTexture();
    if (_textureFormat != finalTex->format()) {
        _textureFormat = finalTex->format();
        _pipeline0.reset();
    }

    if (!_pipeline0) {
        createPipline0();
    }

    QRhiResourceUpdateBatch *batch = _rhi->nextResourceUpdateBatch();
    batch->uploadStaticBuffer(_vectexBuffer0.get(), vertexRect);

    crateTextureImage();
    batch->uploadTexture(_texture.get(), _textureImage);

    cb->resourceUpdate(batch);
}

void TextureRenderer::synchronize(QQuickRhiItem *rhiItem)
{
    Texture *item = static_cast<Texture *>(rhiItem);
    _angle = item->angle();
    _orthoX = item->getOrthoX();
    _orthoY = item->getOrthoY();
    _zoom = item->getZoom();
    _focus = item->getFocus();
}

Texture::Texture()
{
}

Texture::~Texture()
{
}

QQuickRhiItemRenderer* Texture::createRenderer()
{
    return new TextureRenderer();
}
