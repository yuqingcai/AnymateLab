#ifndef SAMPLEITEM_H
#define SAMPLEITEM_H

#include <QFile>
#include <QQuickRhiItem>
#include <rhi/qrhi.h>

#define STD140_ALIGN_INT   4
#define STD140_ALIGN_FLOAT 4
#define STD140_ALIGN_VEC2  8
#define STD140_ALIGN_VEC3 16
#define STD140_ALIGN_VEC4 16
#define STD140_ALIGN_MAT4 64

size_t align(size_t offset, size_t alignment);
QShader getShader(const QString &name);

class SampleItem: public QQuickRhiItem
{
    Q_OBJECT
    Q_PROPERTY(float angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)

public:
    SampleItem();
    virtual ~ SampleItem();
    float angle() const;
    void setAngle(float angle);
    float scale() const;
    void setScale(float scale);
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

    void renderToTexture();

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

Q_SIGNALS:
    void angleChanged();
    void scaleChanged();

private:

};

#endif // SAMPLEITEM_H
