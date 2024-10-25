#ifndef BOOSTGEOMETRY_H
#define BOOSTGEOMETRY_H

#include "sampleitem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class BoostGeometryRenderer: public QQuickRhiItemRenderer
{
public:
    BoostGeometryRenderer();
    ~BoostGeometryRenderer();
    void initialize(QRhiCommandBuffer *cb) override;
    void synchronize(QQuickRhiItem *item) override;
    void render(QRhiCommandBuffer *cb) override;

private:
    int createBuffer0();
    int createShaderResourceBinding0();
    int createPipline0();
    void pushPointToVetices(Point_2& p, std::vector<float>& vectices);

    QRhi *_rhi = nullptr;
    int _sampleCount = 4;
    QRhiTexture::Format _textureFormat = QRhiTexture::RGBA8;

    std::unique_ptr<QRhiGraphicsPipeline> _pipeline0;
    std::unique_ptr<QRhiShaderResourceBindings> _srb0;
    std::unique_ptr<QRhiBuffer> _uniformBuffer0;
    std::unique_ptr<QRhiBuffer> _vectexBuffer0;
    std::unique_ptr<QRhiBuffer> _indexBuffer0;
    std::unique_ptr<QRhiBuffer> _modelBuffer0;

    QMatrix4x4 _view;
    QMatrix4x4 _projection;

    static constexpr auto m_shaderResourceStages =
        QRhiShaderResourceBinding::VertexStage |
        QRhiShaderResourceBinding::GeometryStage |
        QRhiShaderResourceBinding::FragmentStage |
        QRhiShaderResourceBinding::TessellationControlStage |
        QRhiShaderResourceBinding::TessellationEvaluationStage |
        QRhiShaderResourceBinding::ComputeStage;

    float _angle = 0.0f;
    float _orthoX = 0.0f;
    float _orthoY = 0.0f;
    float _zoom = 1.0f;
    QPointF _focus = {0.0f, 0.0f};

    // std::vector<Vangoh::Shape*> _shapes;
};

class BoostGeometry: public SampleItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(BoostGeometry)

public:
    BoostGeometry();
    virtual ~ BoostGeometry();
    QQuickRhiItemRenderer *createRenderer() override;
    // std::vector<Vangoh::Shape*>& getShapes();

private:
    // std::vector<Vangoh::Shape*> _shapes;

};

#endif // BOOSTGEOMETRY_H
