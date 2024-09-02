#ifndef STAGE_H
#define STAGE_H

#include <QQuickRhiItem>
#include <rhi/qrhi.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class StageRenderer : public QQuickRhiItemRenderer
{
public:
    StageRenderer();
    ~StageRenderer();
    void initialize(QRhiCommandBuffer *cb) override;
    void synchronize(QQuickRhiItem *item) override;
    void render(QRhiCommandBuffer *cb) override;

private:
    QRhi *m_rhi = nullptr;
    int m_sampleCount = 4;
    QRhiTexture::Format m_textureFormat = QRhiTexture::RGBA8;
    std::unique_ptr<QRhiGraphicsPipeline> m_pipeline;
    std::unique_ptr<QRhiBuffer> m_vectexBuffer1;
    std::unique_ptr<QRhiBuffer> m_modelBuffer1;
    std::unique_ptr<QRhiBuffer> m_vectexBuffer2;
    std::unique_ptr<QRhiBuffer> m_modelBuffer2;

    std::unique_ptr<QRhiBuffer> m_uniformBuffer;
    std::unique_ptr<QRhiShaderResourceBindings> m_srb;

    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    float m_angle = 0.0f;
    float m_alpha = 1.0f;
    float m_orthoX = 0.0f;
    float m_orthoY = 0.0f;
    float m_zoom = 1.0f;
    QPointF m_focus = {0.0f, 0.0f};

    int m_uniformBufferBlockSize = 0;
    int m_uniformBufferBlockCount = 1;

    int m_instances1 = 4000;
    int m_instances2 = 4000;
    glm::mat4* m_models1;
    glm::mat4* m_models2;
};

class Stage: public QQuickRhiItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Stage)
    Q_PROPERTY(float angle READ angle WRITE setAngle NOTIFY angleChanged)

public:
    Stage();
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

#endif // STAGE_H
