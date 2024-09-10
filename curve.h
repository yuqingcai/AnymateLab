#ifndef CURVE_H
#define CURVE_H

#include <QQuickRhiItem>
#include <rhi/qrhi.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CurveRenderer : public QQuickRhiItemRenderer
{
public:
    CurveRenderer();
    ~CurveRenderer();
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
    int m_Rects = 1;
    glm::mat4* m_models;
    QImage m_textureImage;
    int m_pointCount = 4;
    glm::vec2* m_points;

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

class Curve: public QQuickRhiItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Curve)
    Q_PROPERTY(float angle READ angle WRITE setAngle NOTIFY angleChanged)

public:
    Curve();
    QQuickRhiItemRenderer *createRenderer() override;

    float angle() const;
    void setAngle(float a);
    float getOrthoX();
    float getOrthoY();
    float getZoom();
    QPointF& getFocus();

protected:
    void hoverMoveEvent(QHoverEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;


signals:
    void angleChanged();

private:
    float m_angle = 0.0f;
    float m_orthoX = 0.0f;
    float m_orthoY = 0.0f;
    float m_zoom = 1.0f;
    QPointF m_focus = {0.0f, 0.0f};
    bool m_spaceButtonDown = false;
    bool m_leftButtonDown = false;

    QPoint m_mosePosition0 = {0, 0};
    QPoint m_mosePosition1 = {0, 0};
};

#endif // CURVE_H
