#ifndef MORPHING_H
#define MORPHING_H

#include "graphic.h"
#include "sampleitem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MorphingRenderer: public QQuickRhiItemRenderer
{
public:
    MorphingRenderer();
    ~MorphingRenderer();
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
    int _morphing = 0;
    float _orthoX = 0.0f;
    float _orthoY = 0.0f;
    float _zoom = 1.0f;
    QPointF _focus = {0.0f, 0.0f};
    bool _updateMophing = false;

    std::vector<Vangoh::Shape*> _shapes;
};

class Morphing: public SampleItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Morphing)
    Q_PROPERTY(int morphing READ morphing WRITE setMorphing NOTIFY morphingChanged)

public:
    Morphing();
    virtual ~ Morphing();
    QQuickRhiItemRenderer *createRenderer() override;
    int morphing() const;
    void setMorphing(int morphing);
    std::vector<Vangoh::Shape*>& getShapes();

private:
    int _morphing = 0;
    std::vector<Vangoh::Shape*> _shapes;

Q_SIGNALS:
    void morphingChanged();
};

#endif // MORPHING_H
