#ifndef MOTION_H
#define MOTION_H

#include "sampleitem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MotionRenderer : public QQuickRhiItemRenderer
{
public:
    MotionRenderer();
    ~MotionRenderer();
    void initialize(QRhiCommandBuffer *cb) override;
    void synchronize(QQuickRhiItem *item) override;
    void render(QRhiCommandBuffer *cb) override;

private:
    int createBuffer();
    int createPipline();
    void crateTextureImage();

    QRhi *m_rhi = nullptr;
    int m_sampleCount = 4;
    QRhiTexture::Format m_textureFormat = QRhiTexture::RGBA8;

    std::unique_ptr<QRhiGraphicsPipeline> m_pipeline;
    std::unique_ptr<QRhiShaderResourceBindings> m_srb;
    std::unique_ptr<QRhiBuffer> m_uniformBuffer;
    std::unique_ptr<QRhiBuffer> m_vectexBuffer;
    std::unique_ptr<QRhiBuffer> m_modelBuffer;
    std::unique_ptr<QRhiSampler> m_sampler;
    std::unique_ptr<QRhiTexture> m_texture;
    int m_Rects = 1;
    glm::mat4* m_models;
    QImage m_textureImage;
    int m_step;
    glm::vec2 m_region[4];

    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    int m_uniformBufferBlockCount = 1;
    static constexpr auto m_shaderResourceStages =
        QRhiShaderResourceBinding::VertexStage |
        QRhiShaderResourceBinding::FragmentStage |
        QRhiShaderResourceBinding::TessellationControlStage |
        QRhiShaderResourceBinding::TessellationEvaluationStage |
        QRhiShaderResourceBinding::ComputeStage |
        QRhiShaderResourceBinding::GeometryStage;

    float m_angle = 0.0f;
    float m_orthoX = 0.0f;
    float m_orthoY = 0.0f;
    float m_zoom = 1.0f;
    QPointF m_focus = {0.0f, 0.0f};

};

class Motion: public SampleItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Motion)

public:
    Motion();
    virtual ~ Motion();
    QQuickRhiItemRenderer *createRenderer() override;

};

#endif // MOTION_H
