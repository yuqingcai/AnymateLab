#ifndef TEXTURE_H
#define TEXTURE_H

#include "sampleitem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class TextureRenderer : public QQuickRhiItemRenderer
{
public:
    TextureRenderer();
    ~TextureRenderer();
    void initialize(QRhiCommandBuffer *cb) override;
    void synchronize(QQuickRhiItem *item) override;
    void render(QRhiCommandBuffer *cb) override;

private:
    int createBuffer0();
    int createPipline0();
    void crateTextureImage();

    QRhi *_rhi = nullptr;
    int _sampleCount = 4;
    QRhiTexture::Format _textureFormat = QRhiTexture::RGBA8;

    std::unique_ptr<QRhiGraphicsPipeline> _pipeline0;
    std::unique_ptr<QRhiShaderResourceBindings> _srb0;
    std::unique_ptr<QRhiBuffer> _uniformBuffer0;
    std::unique_ptr<QRhiBuffer> _vectexBuffer0;
    std::unique_ptr<QRhiBuffer> _modelBuffer0;
    std::unique_ptr<QRhiSampler> _sampler;
    std::unique_ptr<QRhiTexture> _texture;
    QImage _textureImage;

    QMatrix4x4 _view;
    QMatrix4x4 _projection;
    int _rects;

    int _uniformBufferBlockCount = 1;
    static constexpr auto _shaderResourceStages =
        QRhiShaderResourceBinding::VertexStage |
        QRhiShaderResourceBinding::FragmentStage |
        QRhiShaderResourceBinding::TessellationControlStage |
        QRhiShaderResourceBinding::TessellationEvaluationStage |
        QRhiShaderResourceBinding::ComputeStage |
        QRhiShaderResourceBinding::GeometryStage;

    float _angle = 0.0f;
    float _orthoX = 0.0f;
    float _orthoY = 0.0f;
    float _zoom = 1.0f;
    QPointF _focus = {0.0f, 0.0f};
};

class Texture: public SampleItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Texture)

public:
    Texture();
    ~ Texture();
    QQuickRhiItemRenderer *createRenderer() override;

private:
};
#endif // TEXTURE_H
