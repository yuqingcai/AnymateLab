#ifndef CURVE_H
#define CURVE_H

#include <QQuickRhiItem>
#include <rhi/qrhi.h>
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
    int createShaderResourceBinding0();
    int createBuffer1();
    int createShaderResourceBinding1();
    int createPipline0();
    int createPipline1();

    QRhi *_rhi = nullptr;
    int _sampleCount = 4;
    QRhiTexture::Format _textureFormat = QRhiTexture::RGBA8;


    std::unique_ptr<QRhiGraphicsPipeline> _pipeline0;
    std::unique_ptr<QRhiShaderResourceBindings> _srb0;
    std::unique_ptr<QRhiBuffer> _uniformBuffer0;
    std::unique_ptr<QRhiBuffer> _vectexBuffer0;
    std::unique_ptr<QRhiBuffer> _modelBuffer0;

    std::unique_ptr<QRhiGraphicsPipeline> _pipeline1;
    std::unique_ptr<QRhiShaderResourceBindings> _srb1;
    std::unique_ptr<QRhiBuffer> _uniformBuffer1;
    std::unique_ptr<QRhiBuffer> _vectexBuffer1;
    std::unique_ptr<QRhiBuffer> _modelBuffer1;

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


    std::vector<Anymate::GeometryShape*> _shapes;

};

class Curve: public QQuickRhiItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Curve)
    Q_PROPERTY(float angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)

public:
    Curve();
    virtual ~ Curve();
    QQuickRhiItemRenderer *createRenderer() override;

    float angle() const;
    void setAngle(float a);
    float scale() const;
    void setScale(float s);
    float getOrthoX();
    float getOrthoY();
    float getZoom();
    QPointF& getFocus();

    std::vector<Anymate::GeometryShape*>& getShapes();


protected:
    void renderToTexture();

    void hoverMoveEvent(QHoverEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;


signals:
    void angleChanged();

private:

    std::vector<Anymate::GeometryShape*> _shapes;

    float _angle = 0.0f;
    float _scale = 1.0f;
    float _orthoX = 0.0f;
    float _orthoY = 0.0f;
    float _zoom = 500.0f;
    QPointF _focus = {0.0f, 0.0f};
    bool _spaceButtonDown = false;
    bool _leftButtonDown = false;

    QPoint _mosePosition0 = {0, 0};
    QPoint _mosePosition1 = {0, 0};
};

#endif // CURVE_H
