#ifndef CURVE_H
#define CURVE_H

#include "sampleitem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "graphic.h"

class CurveRenderer : public QQuickRhiItemRenderer
{
public:
    CurveRenderer();
    ~CurveRenderer();
    void initialize(QRhiCommandBuffer *cb) override;
    void synchronize(QQuickRhiItem *item) override;
    void render(QRhiCommandBuffer *cb) override;

private:
    int createBuffer0();
    int createBuffer1();
    int createBuffer2();
    int createShaderResourceBinding0();
    int createShaderResourceBinding1();
    int createShaderResourceBinding2();
    int createPipline0();
    int createPipline1();
    int createPipline2();

    QRhi *_rhi = nullptr;
    int _sampleCount = 4;
    QRhiTexture::Format _textureFormat = QRhiTexture::RGBA8;


    std::unique_ptr<QRhiGraphicsPipeline> _pipeline0;
    std::unique_ptr<QRhiShaderResourceBindings> _srb0;
    std::unique_ptr<QRhiBuffer> _uniformBuffer0;
    std::unique_ptr<QRhiBuffer> _vectexBuffer0;
    std::unique_ptr<QRhiBuffer> _indexBuffer0;
    std::unique_ptr<QRhiBuffer> _modelBuffer0;

    std::unique_ptr<QRhiGraphicsPipeline> _pipeline1;
    std::unique_ptr<QRhiShaderResourceBindings> _srb1;
    std::unique_ptr<QRhiBuffer> _uniformBuffer1;
    std::unique_ptr<QRhiBuffer> _vectexBuffer1;
    std::unique_ptr<QRhiBuffer> _indexBuffer1;
    std::unique_ptr<QRhiBuffer> _modelBuffer1;


    std::unique_ptr<QRhiGraphicsPipeline> _pipeline2;
    std::unique_ptr<QRhiShaderResourceBindings> _srb2;
    std::unique_ptr<QRhiBuffer> _uniformBuffer2;
    std::unique_ptr<QRhiBuffer> _vectexBuffer2;
    std::unique_ptr<QRhiBuffer> _indexBuffer2;
    std::unique_ptr<QRhiBuffer> _modelBuffer2;

    QMatrix4x4 _view;
    QMatrix4x4 _projection;
    glm::mat4* _model;

    static constexpr auto m_shaderResourceStages =
        QRhiShaderResourceBinding::VertexStage |
        QRhiShaderResourceBinding::GeometryStage |
        QRhiShaderResourceBinding::FragmentStage |
        QRhiShaderResourceBinding::TessellationControlStage |
        QRhiShaderResourceBinding::TessellationEvaluationStage |
        QRhiShaderResourceBinding::ComputeStage;

    float _angle = 0.0f;
    float _scale = 1.0f;
    float _orthoX = 0.0f;
    float _orthoY = 0.0f;
    float _zoom = 1.0f;
    QPointF _focus = {0.0f, 0.0f};

    std::vector<Vangoh::Shape*> _shapes;

};

class Curve: public SampleItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Curve)

public:
    Curve();
    virtual ~ Curve();
    QQuickRhiItemRenderer *createRenderer() override;
    std::vector<Vangoh::Shape*>& getShapes();

protected:

private:
    std::vector<Vangoh::Shape*> _shapes;
};

#endif // CURVE_H
