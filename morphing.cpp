#include "morphing.h".h"
#include <QFile>
#include <QtGui/qevent.h>
#include <cstdio>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

MorphingRenderer::MorphingRenderer()
{
}


MorphingRenderer::~MorphingRenderer()
{
}

int MorphingRenderer::createBuffer0()
{
    if (!_rhi)
        return -1;

    size_t verticesSize = sizeof(float) * 1024 * 1024 * 4;

    _vectexBuffer0.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                         QRhiBuffer::VertexBuffer,
                                         verticesSize));
    _vectexBuffer0->create();

    _indexBuffer0.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                        QRhiBuffer::IndexBuffer,
                                        verticesSize));
    _indexBuffer0->create();

    _modelBuffer0.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                        QRhiBuffer::VertexBuffer,
                                        sizeof(glm::mat4) * 1024));
    _modelBuffer0->create();

    return 0;
}

int MorphingRenderer::createShaderResourceBinding0()
{

    if (!_rhi)
        return -1;

    // view and projection uniform
    int blockSize = sizeof(glm::mat4) +
                    sizeof(glm::mat4) +
                    sizeof(glm::vec4)
        ;
    int bufferSize = _rhi->ubufAligned(blockSize);
    _uniformBuffer0.reset(_rhi->newBuffer(QRhiBuffer::Dynamic,
                                          QRhiBuffer::UniformBuffer,
                                          bufferSize));
    _uniformBuffer0->create();

    _srb0.reset(_rhi->newShaderResourceBindings());
    _srb0->setBindings({
        QRhiShaderResourceBinding::uniformBufferWithDynamicOffset(
            0,
            m_shaderResourceStages,
            _uniformBuffer0.get(),
            bufferSize),
    });
    _srb0->create();
    return 0;
}
int MorphingRenderer::createPipline0()
{
    if (!_rhi || !_srb0)
        return -1;

    _pipeline0.reset(_rhi->newGraphicsPipeline());

    _pipeline0->setShaderStages({
        {
            QRhiShaderStage::Vertex,
            getShader(QLatin1String(":/AnymateLab/shaders/curve.vert.qsb"))
        },
        {
            QRhiShaderStage::Fragment,
            getShader(QLatin1String(":/AnymateLab/shaders/curve.frag.qsb"))
        }
    });

    QRhiVertexInputLayout inputLayout;
    inputLayout.setBindings(
        {
         { 3 * sizeof(float), QRhiVertexInputBinding::PerVertex },
         { 64, QRhiVertexInputBinding::PerInstance },
         });

    inputLayout.setAttributes(
        {
         // binding0
         { 0, 0, QRhiVertexInputAttribute::Float3, 0                 },
         // binding1
         { 1, 1, QRhiVertexInputAttribute::Float4, 0 },
         { 1, 2, QRhiVertexInputAttribute::Float4, 4 * sizeof(float) },
         { 1, 3, QRhiVertexInputAttribute::Float4, 8 * sizeof(float) },
         { 1, 4, QRhiVertexInputAttribute::Float4, 12 * sizeof(float) },
         });


    _pipeline0->setVertexInputLayout(inputLayout);
    _pipeline0->setSampleCount(_sampleCount);
    _pipeline0->setShaderResourceBindings(_srb0.get());
    _pipeline0->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    _pipeline0->setDepthTest(true);
    _pipeline0->setDepthWrite(true);

    // _pipeline0->setTopology(QRhiGraphicsPipeline::Lines);
    // _pipeline0->setTopology(QRhiGraphicsPipeline::LineStrip);
    _pipeline0->setTopology(QRhiGraphicsPipeline::Triangles);
    QList<QRhiGraphicsPipeline::TargetBlend> targetBlends(1);
    targetBlends[0].enable = true;
    targetBlends[0].srcColor = QRhiGraphicsPipeline::SrcAlpha;
    targetBlends[0].dstColor = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opColor = QRhiGraphicsPipeline::Add;
    targetBlends[0].srcAlpha = QRhiGraphicsPipeline::One;
    targetBlends[0].dstAlpha = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opAlpha = QRhiGraphicsPipeline::Add;
    _pipeline0->setTargetBlends(targetBlends.begin(), targetBlends.end());
    _pipeline0->create();
    return 0;
}

void MorphingRenderer::initialize(QRhiCommandBuffer *cb)
{

    if (_rhi != rhi()) {
        _rhi = rhi();
        createBuffer0();
        _pipeline0.reset();
    }

    if (_sampleCount != renderTarget()->sampleCount()) {
        _sampleCount = renderTarget()->sampleCount();
        _pipeline0.reset();
    }

    QRhiTexture *finalTex = _sampleCount > 1 ? resolveTexture() : colorTexture();
    if (_textureFormat != finalTex->format()) {
        _textureFormat = finalTex->format();
        _pipeline0.reset();
    }

    if (!_srb0) {
        createShaderResourceBinding0();
    }

    if (!_pipeline0) {
        createPipline0();
    }
}

void MorphingRenderer::pushPointToVetices(Point_2& p, std::vector<float>& vectices)
{
    double x = CGAL::to_double(p.x());
    double y = CGAL::to_double(p.y());
    vectices.push_back(x);
    vectices.push_back(y);
    vectices.push_back(0.0);
}

void MorphingRenderer::render(QRhiCommandBuffer *cb)
{
    if (!_updateMophing) {
        // return;
    }

    const QSize outputSize = renderTarget()->pixelSize();
    _projection = _rhi->clipSpaceCorrMatrix();

    _projection.perspective(45.0f,
                            outputSize.width() / (float) outputSize.height(),
                            10.0f,
                            1000000.0f);

    QVector3D cameraPos(_focus.rx(), _focus.ry(), _zoom);
    QVector3D cameraTarget(_focus.rx(), _focus.ry(), 0.0f);
    QVector3D cameraUp(0.0f, 1.0f, 0.0f);
    _view.setToIdentity();
    _view.lookAt(cameraPos, cameraTarget, cameraUp);

    QRhiResourceUpdateBatch *batch = _rhi->nextResourceUpdateBatch();

    const QColor clearColor = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f);
    cb->beginPass(renderTarget(), clearColor, { 1.0f, 0 }, batch);
    cb->setViewport(QRhiViewport(0, 0, outputSize.width(), outputSize.height()));


    size_t offset = 0;
    glm::vec4 lineColor = glm::vec4(1.0, 0.0, 0.0, 1.0);

    offset = align(0, STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(_uniformBuffer0.get(),
                               offset,
                               sizeof(glm::mat4),
                               _view.constData());
    offset = align(sizeof(glm::mat4), STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(_uniformBuffer0.get(),
                               offset,
                               sizeof(glm::mat4),
                               _projection.constData());
    offset = align(sizeof(glm::mat4) + sizeof(glm::mat4),
                   STD140_ALIGN_VEC4);
    batch->updateDynamicBuffer(_uniformBuffer0.get(),
                               offset,
                               sizeof(glm::vec4),
                               &lineColor);

    offset = 0;
    for (int i = 0; i < _shapes.size(); i ++) {
        std::vector<glm::vec2>& vertices2D = _shapes[i]->getShapeVertices();
        std::vector<glm::vec3> vertices;
        for (glm::vec2 p : vertices2D) {
            vertices.push_back(glm::vec3(p.x, p.y, 0.0));
        }
        qint32 size = vertices.size() * sizeof(glm::vec3);
        batch->uploadStaticBuffer(_vectexBuffer0.get(), offset, size, vertices.data());
        offset += size;

        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::rotate(model, qDegreesToRadians(_angle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        batch->uploadStaticBuffer(_modelBuffer0.get(), i * sizeof(glm::mat4),
                                  sizeof(glm::mat4), &model);
    }

    cb->resourceUpdate(batch);

    cb->setGraphicsPipeline(_pipeline0.get());
    cb->setShaderResources(_srb0.get());
    offset = 0;
    for (int i = 0; i < _shapes.size(); i ++) {
        std::vector<glm::vec2>& vertices = _shapes[i]->getShapeVertices();
        const QRhiCommandBuffer::VertexInput inputBindings[] = {
            { _vectexBuffer0.get(), offset },
            { _modelBuffer0.get(), i * sizeof(glm::mat4) }
        };
        cb->setVertexInput(0, 2, inputBindings);
        cb->draw(vertices.size());
        offset += vertices.size() * sizeof(glm::vec3);
    }


    cb->endPass();

}

void MorphingRenderer::synchronize(QQuickRhiItem *rhiItem)
{
    Morphing *item = static_cast<Morphing *>(rhiItem);
    if (item->angle() != _angle)
        _angle = item->angle();

    if (item->morphing() != _morphing) {
        _morphing = item->morphing();
        _updateMophing = true;
    }
    else {
        _updateMophing = false;
    }

    _orthoX = item->getOrthoX();
    _orthoY = item->getOrthoY();
    _zoom = item->getZoom();
    _focus = item->getFocus();

    _shapes = item->getShapes();

}

Morphing::Morphing()
{
    Vangoh::Pen pen0(Vangoh::SolidLine, Vangoh::FlatCap, Vangoh::RoundJoin, 2);

    Vangoh::Polygon* polygon1 = new Vangoh::Polygon({
        glm::vec2(0, 0),
        glm::vec2(0, 8),
        glm::vec2(7, 8),
        glm::vec2(7, 0),
        glm::vec2(1, 4),
        glm::vec2(2, 5),
        glm::vec2(2, 4),
        glm::vec2(1, 6),
        glm::vec2(6, 6),

        glm::vec2(4, 4),
        glm::vec2(5, 3),
        glm::vec2(5, 5),
        glm::vec2(4, 4),

        glm::vec2(-4, -4),
        glm::vec2(0, -4),

        glm::vec2(0, 0),

    });
    // Vangoh::Polygon* polygon1 = new Vangoh::Polygon({
    //     glm::vec2(0, 0),
    //     glm::vec2(10, 0),
    //     glm::vec2(10, 10),
    //     glm::vec2(0, 10),
    //     glm::vec2(0, 0),
    // });


    // Vangoh::Polygon* polygon1 = new Vangoh::Polygon({
    //     glm::vec2(0, 0),
    //     glm::vec2(50, 0),
    //     glm::vec2(25, 75),
    //     glm::vec2(50, 75),
    //     glm::vec2(60, 70),
    //     glm::vec2(40, 70),
    //     glm::vec2(50, 75),
    //     glm::vec2(75, 75),
    //     glm::vec2(50, 0),
    //     glm::vec2(100, 0),
    //     glm::vec2(100, 100),
    //     glm::vec2(0, 100),
    //     glm::vec2(0, 0),
    // });

    polygon1->setPen(pen0);
    polygon1->createVertices();
    _shapes.push_back(polygon1);

}

Morphing:: ~ Morphing()
{
    for (auto ptr : _shapes) {
        delete ptr;
    }
    _shapes.clear();
}

QQuickRhiItemRenderer* Morphing::createRenderer()
{
    return new MorphingRenderer();
}

int Morphing::morphing() const
{
    return _morphing;
}

void Morphing::setMorphing(int morphing)
{
    _morphing = morphing;
}

std::vector<Vangoh::Shape*>& Morphing::getShapes()
{
    return _shapes;
}
