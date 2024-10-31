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


class WorkerThread : public QThread {
    Q_OBJECT

public:
    explicit WorkerThread(QObject *parent = nullptr) : QThread(parent) {}
    ~WorkerThread() override {}

Q_SIGNALS:
    void resultReady();

protected:
    void run() override {

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < 100; i ++) {
            Vangoh::Pen pen0(Vangoh::SolidLine, Vangoh::FlatCap, Vangoh::RoundJoin, 2);
            Vangoh::Polygon* polygon = new Vangoh::Polygon({
                glm::vec2(0, 0),
                glm::vec2(0, 80),
                glm::vec2(70, 80),
                glm::vec2(70, 0),
                glm::vec2(10, 40),
                glm::vec2(20, 50),
                glm::vec2(20, 40),
                glm::vec2(10, 60),
                glm::vec2(60, 60),

                glm::vec2(40, 40),
                glm::vec2(50, 50),
                glm::vec2(50, 30),
                glm::vec2(40, 40),

                glm::vec2(0, 0),

            });
            polygon->setPen(pen0);
            polygon->createVertices();
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "create polygon x100 spend: " << duration.count() << " ms" << std::endl;
        Q_EMIT resultReady();
    }
};

#endif // MORPHING_H
