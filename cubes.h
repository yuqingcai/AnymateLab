#ifndef CUBES_H
#define CUBES_H

#include "sampleitem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CubesRenderer : public QQuickRhiItemRenderer
{
public:
    CubesRenderer();
    ~CubesRenderer();
    void initialize(QRhiCommandBuffer *cb) override;
    void synchronize(QQuickRhiItem *item) override;
    void render(QRhiCommandBuffer *cb) override;

private:
    int createCubeBuffer();
    int createPyramidBuffer();
    int createBezierBuffer();
    int createRectBuffer();

    int createPipline1();
    int createPipline2();
    int createPipline3();
    int createPipline4();

    void crateTextureImage();

    QRhi *m_rhi = nullptr;
    int m_sampleCount = 4;
    QRhiTexture::Format m_textureFormat = QRhiTexture::RGBA8;

    std::unique_ptr<QRhiGraphicsPipeline> m_pipeline1;
    std::unique_ptr<QRhiShaderResourceBindings> m_srb1;
    std::unique_ptr<QRhiBuffer> m_uniformBuffer1;
    std::unique_ptr<QRhiBuffer> m_vectexBufferCube;
    std::unique_ptr<QRhiBuffer> m_modelBufferCube;
    int m_Cubes = 4000;
    glm::mat4* m_modelCubes;

    std::unique_ptr<QRhiGraphicsPipeline> m_pipeline2;
    std::unique_ptr<QRhiShaderResourceBindings> m_srb2;
    std::unique_ptr<QRhiBuffer> m_uniformBuffer2;
    std::unique_ptr<QRhiBuffer> m_vectexBufferPyramid;
    std::unique_ptr<QRhiBuffer> m_modelBufferPyramid;
    int m_Pyramids = 4000;
    glm::mat4* m_modelPyramids;

    std::unique_ptr<QRhiGraphicsPipeline> m_pipeline3;
    std::unique_ptr<QRhiShaderResourceBindings> m_srb3;
    std::unique_ptr<QRhiBuffer> m_uniformBuffer3;
    std::unique_ptr<QRhiBuffer> m_vectexBufferShape;
    std::unique_ptr<QRhiBuffer> m_modelBufferShape;
    int m_Shapes = 1;
    glm::mat4* m_modelShapes;

    std::unique_ptr<QRhiGraphicsPipeline> m_pipeline4;
    std::unique_ptr<QRhiShaderResourceBindings> m_srb4;
    std::unique_ptr<QRhiBuffer> m_uniformBuffer4;
    std::unique_ptr<QRhiBuffer> m_vectexBufferRect;
    std::unique_ptr<QRhiBuffer> m_modelBufferRect;
    std::unique_ptr<QRhiSampler> m_sampler;
    std::unique_ptr<QRhiTexture> m_texture;
    int m_Rects = 1;
    glm::mat4* m_modelRects;
    QImage m_textureImage;

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

class Cubes: public SampleItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Cubes)

public:
    Cubes();
    ~ Cubes();
    QQuickRhiItemRenderer *createRenderer() override;

private:
};

#endif // CUBES_H
