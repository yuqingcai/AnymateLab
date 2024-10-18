#include "squircles.h"
#include <QFile>
#include <QtGui/qevent.h>
#include <cstdio>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

SquirclesRenderer::SquirclesRenderer()
{
    _model = new glm::mat4(1.0f);
}


SquirclesRenderer::~SquirclesRenderer()
{
    if (_model) {
        delete [] _model;
    }
}

int SquirclesRenderer::createBuffer0()
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

int SquirclesRenderer::createShaderResourceBinding0()
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
int SquirclesRenderer::createPipline0()
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

    _pipeline0->setTopology(QRhiGraphicsPipeline::Lines);
    // _pipeline0->setTopology(QRhiGraphicsPipeline::LineStrip);
    // _pipeline0->setTopology(QRhiGraphicsPipeline::Triangles);
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

void SquirclesRenderer::initialize(QRhiCommandBuffer *cb)
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

void SquirclesRenderer::pushPointToVetices(Point& p, std::vector<float>& vectices)
{
    vectices.push_back(p.x());
    vectices.push_back(p.y());
    vectices.push_back(0.0);
}

void SquirclesRenderer::render(QRhiCommandBuffer *cb)
{
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


    std::vector<float> vertices;
    std::vector<std::pair<double, double>> points;
    double cx = 0.0, cy = 0.0, r = 50.0;
    double step = 1.0;
    double a = 50.0;
    double b = 50.0;
    double n = 5.0;
    for (int i = 0; i < 90; ++i) {
        double theta = i * M_PI / 180.0;
        double x = a * pow(cos(theta), 2.0 / n);
        double y = b * pow(sin(theta), 2.0 / n);
        points.emplace_back(x, y);
    }

    CDT cdt;
    std::vector<Vertex_handle> vs;

    for (int i = 0; i < points.size(); i ++) {
        Vertex_handle v = cdt.insert(Point(points[i].first, points[i].second));
        vs.push_back(v);
    }

    for (int i = points.size() - 1; i >= 0; i --) {
        Vertex_handle v = cdt.insert(Point(-points[i].first, points[i].second));
        vs.push_back(v);
    }

    for (int i = 0; i < points.size(); i ++) {
        Vertex_handle v = cdt.insert(Point(-points[i].first, -points[i].second));
        vs.push_back(v);
    }

    for (int i = points.size() - 1; i >= 0; i --) {
        Vertex_handle v = cdt.insert(Point(points[i].first, -points[i].second));
        vs.push_back(v);
    }

    for (int i = 0; i < vs.size(); i ++) {
        if (i < vs.size() - 1) {
            cdt.insert_constraint(vs[i], vs[i+1]);
        }
        else {
            cdt.insert_constraint(vs[i], vs[0]);
        }
    }

    std::list<Point> list_of_seeds;
    CGAL::refine_Delaunay_mesh_2(cdt, CGAL::parameters::seeds(list_of_seeds));

    for (auto fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); fit ++) {

            if(fit->is_in_domain()) {

            Point p0 = fit->vertex(0)->point();
            Point p1 = fit->vertex(1)->point();
            Point p2 = fit->vertex(2)->point();


            // pushPointToVetices(p0, vertices);
            // pushPointToVetices(p1, vertices);
            // pushPointToVetices(p2, vertices);

            pushPointToVetices(p0, vertices);
            pushPointToVetices(p1, vertices);
            pushPointToVetices(p1, vertices);
            pushPointToVetices(p2, vertices);
            pushPointToVetices(p2, vertices);
            pushPointToVetices(p0, vertices);
        }

    }
    batch->uploadStaticBuffer(_vectexBuffer0.get(),
                              0,
                              vertices.size() * sizeof(float),
                              vertices.data());


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, qDegreesToRadians(_angle), glm::vec3(0.0f, 1.0f, 0.0f));
    // model = glm::scale(model, glm::vec3(_scale, _scale, _scale));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    batch->uploadStaticBuffer(_modelBuffer0.get(), 0, sizeof(glm::mat4), &model);

    cb->resourceUpdate(batch);

    // draw indexed
    cb->setGraphicsPipeline(_pipeline0.get());
    cb->setShaderResources(_srb0.get());

    const QRhiCommandBuffer::VertexInput inputBindings[] = {
        { _vectexBuffer0.get(), 0 },
        { _modelBuffer0.get(), 0 }
    };
    cb->setVertexInput(0, 2, inputBindings, _indexBuffer0.get(), 0, QRhiCommandBuffer::IndexUInt32);
    // cb->drawIndexed(sizeof(indices));
    cb->draw(vertices.size()/3);

    cb->endPass();
}

void SquirclesRenderer::synchronize(QQuickRhiItem *rhiItem)
{
    Squircles *item = static_cast<Squircles *>(rhiItem);
    if (item->angle() != _angle)
        _angle = item->angle();


    if (item->scale() != _scale)
        _scale = item->scale();

    _orthoX = item->getOrthoX();
    _orthoY = item->getOrthoY();
    _zoom = item->getZoom();
    _focus = item->getFocus();
}

Squircles::Squircles()
{
}

Squircles:: ~ Squircles()
{

}

QQuickRhiItemRenderer* Squircles::createRenderer()
{
    return new SquirclesRenderer();
}

