#ifndef DRAWER_H
#define DRAWER_H

#include "graphic.h"
#include "sampleitem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Drawer;

class DrawerRenderer : public QQuickRhiItemRenderer
{
public:
    DrawerRenderer();
    ~DrawerRenderer();
    void initialize(QRhiCommandBuffer *cb) override;
    void synchronize(QQuickRhiItem *item) override;
    void render(QRhiCommandBuffer *cb) override;

private:
    int createBuffer0();
    int createShaderResourceBinding0();
    int createPipline0();

    int createBuffer1();
    int createShaderResourceBinding1();
    int createPipline1();

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

    QMatrix4x4 _viewMatrix;
    QMatrix4x4 _projectionMatrix;

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
    Drawer* _drawerRef;
    std::vector<Vangoh::Shape *> _shapes;
};

class Drawer : public SampleItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Drawer)

public:
    Drawer();
    virtual ~Drawer();
    QQuickRhiItemRenderer *createRenderer() override;
    std::vector<Vangoh::Shape *> &getShapes();
    void createRectangle(glm::vec2& position, float width, float height);
    void createBezier(glm::vec2& position, float width, float height);
    void setProjectionMatrix(QMatrix4x4& matrix);
    void setViewProjectMatrix(QMatrix4x4& matrix);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    QVector2D mousePositionToWordPosition(QVector2D& position);
    bool pointInRange(glm::vec2 p0, glm::vec2 p1);

private:
    QPointF _graphicOrigin = {0.0f, 0.0f};
    std::vector<Vangoh::Shape *> _shapes;
    QMatrix4x4 _viewMatrix;
    QMatrix4x4 _projectionMatrix;

    QPointF _lastMousePosition = { 0.0, 0.0 };
    Vangoh::BezierCurve* _currentCurve;
    Vangoh::Rectangle* _p0;
    Vangoh::Rectangle* _p1;
    Vangoh::Rectangle* _p2;
    Vangoh::Rectangle* _p3;
    Vangoh::Line* _l0;
    Vangoh::Line* _l1;

    int _activedPointIndex;
};

#endif // DRAWER_H
