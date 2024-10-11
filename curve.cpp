#include "curve.h"
#include "features.h"
#include <QFile>
#include <QCursor>
#include <QRandomGenerator>
#include <QtMath>
#include <QPainter>
#include <graphic.h>
#include "tools.h"

#define STD140_ALIGN_INT   4
#define STD140_ALIGN_FLOAT 4
#define STD140_ALIGN_VEC2  8
#define STD140_ALIGN_VEC3 16
#define STD140_ALIGN_VEC4 16
#define STD140_ALIGN_MAT4 64

using namespace Anymate;

static QShader getShader(const QString &name)
{
    QFile f(name);
    return f.open(QIODevice::ReadOnly) ?
               QShader::fromSerialized(f.readAll()) : QShader();
}

static size_t align(size_t offset, size_t alignment) {
    return (offset + alignment - 1) & ~(alignment - 1);
}


CurveRenderer::CurveRenderer()
{
    _model = new glm::mat4(1.0f);
}

CurveRenderer::~CurveRenderer()
{
    if (_model) {
        delete [] _model;
    }
}

int CurveRenderer::createBuffer0()
{
    if (!_rhi)
        return -1;

    size_t verticesSize = sizeof(float) * 1024 * 1024 * 4;

    _vectexBuffer0.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                        QRhiBuffer::VertexBuffer,
                                        verticesSize));
    _vectexBuffer0->create();

    _modelBuffer0.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                       QRhiBuffer::VertexBuffer,
                                       sizeof(glm::mat4) * 1024));
    _modelBuffer0->create();

    return 0;
}

int CurveRenderer::createBuffer1()
{
    if (!_rhi)
        return -1;

    size_t verticesSize = sizeof(float) * 1024 * 1024 * 4;

    _vectexBuffer1.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                         QRhiBuffer::VertexBuffer,
                                         verticesSize));
    _vectexBuffer1->create();

    _modelBuffer1.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                        QRhiBuffer::VertexBuffer,
                                        sizeof(glm::mat4) * 1024));
    _modelBuffer1->create();

    return 0;
}


int CurveRenderer::createBuffer2()
{
    if (!_rhi)
        return -1;

    size_t verticesSize = sizeof(float) * 1024 * 1024 * 4;

    _vectexBuffer2.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                         QRhiBuffer::VertexBuffer,
                                         verticesSize));
    _vectexBuffer2->create();

    _modelBuffer2.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                        QRhiBuffer::VertexBuffer,
                                        sizeof(glm::mat4) * 1024));
    _modelBuffer2->create();

    return 0;
}


int CurveRenderer::createShaderResourceBinding0()
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
    return 1;
}

int CurveRenderer::createShaderResourceBinding1()
{
    if (!_rhi)
        return -1;

    // view and projection uniform
    int blockSize = sizeof(glm::mat4) +
                    sizeof(glm::mat4) +
                    sizeof(glm::vec4)
        ;
    int bufferSize = _rhi->ubufAligned(blockSize);
    _uniformBuffer1.reset(_rhi->newBuffer(QRhiBuffer::Dynamic,
                                          QRhiBuffer::UniformBuffer,
                                          bufferSize));
    _uniformBuffer1->create();

    _srb1.reset(_rhi->newShaderResourceBindings());
    _srb1->setBindings({
        QRhiShaderResourceBinding::uniformBufferWithDynamicOffset(
            0,
            m_shaderResourceStages,
            _uniformBuffer1.get(),
            bufferSize),
    });
    _srb1->create();
    return 1;
}


int CurveRenderer::createShaderResourceBinding2()
{
    if (!_rhi)
        return -1;

    // view and projection uniform
    int blockSize = sizeof(glm::mat4) +
                    sizeof(glm::mat4) +
                    sizeof(glm::vec4)
        ;
    int bufferSize = _rhi->ubufAligned(blockSize);
    _uniformBuffer2.reset(_rhi->newBuffer(QRhiBuffer::Dynamic,
                                          QRhiBuffer::UniformBuffer,
                                          bufferSize));
    _uniformBuffer2->create();

    _srb2.reset(_rhi->newShaderResourceBindings());
    _srb2->setBindings({
        QRhiShaderResourceBinding::uniformBufferWithDynamicOffset(
            0,
            m_shaderResourceStages,
            _uniformBuffer2.get(),
            bufferSize),
    });
    _srb2->create();
    return 1;
}


int CurveRenderer::createPipline0()
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

    return 1;
}

int CurveRenderer::createPipline1()
{
    if (!_rhi || !_srb1)
        return -1;

    _pipeline1.reset(_rhi->newGraphicsPipeline());

    _pipeline1->setShaderStages({
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


    _pipeline1->setVertexInputLayout(inputLayout);
    _pipeline1->setSampleCount(_sampleCount);
    _pipeline1->setShaderResourceBindings(_srb1.get());
    _pipeline1->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    _pipeline1->setDepthTest(true);
    _pipeline1->setDepthWrite(true);
    _pipeline1->setTopology(QRhiGraphicsPipeline::Triangles);

    QList<QRhiGraphicsPipeline::TargetBlend> targetBlends(1);
    targetBlends[0].enable = true;
    targetBlends[0].srcColor = QRhiGraphicsPipeline::SrcAlpha;
    targetBlends[0].dstColor = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opColor = QRhiGraphicsPipeline::Add;
    targetBlends[0].srcAlpha = QRhiGraphicsPipeline::One;
    targetBlends[0].dstAlpha = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opAlpha = QRhiGraphicsPipeline::Add;
    _pipeline1->setTargetBlends(targetBlends.begin(), targetBlends.end());
    _pipeline1->create();

    return 1;
}


int CurveRenderer::createPipline2()
{
    if (!_rhi || !_srb1)
        return -1;

    _pipeline2.reset(_rhi->newGraphicsPipeline());

    _pipeline2->setShaderStages({
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


    _pipeline2->setVertexInputLayout(inputLayout);
    _pipeline2->setSampleCount(_sampleCount);
    _pipeline2->setShaderResourceBindings(_srb1.get());
    _pipeline2->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    _pipeline2->setDepthTest(true);
    _pipeline2->setDepthWrite(true);
    _pipeline2->setTopology(QRhiGraphicsPipeline::Lines);

    QList<QRhiGraphicsPipeline::TargetBlend> targetBlends(1);
    targetBlends[0].enable = true;
    targetBlends[0].srcColor = QRhiGraphicsPipeline::SrcAlpha;
    targetBlends[0].dstColor = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opColor = QRhiGraphicsPipeline::Add;
    targetBlends[0].srcAlpha = QRhiGraphicsPipeline::One;
    targetBlends[0].dstAlpha = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opAlpha = QRhiGraphicsPipeline::Add;
    _pipeline2->setTargetBlends(targetBlends.begin(), targetBlends.end());
    _pipeline2->create();

    return 1;
}

void CurveRenderer::initialize(QRhiCommandBuffer *cb)
{
    if (_rhi != rhi()) {
        _rhi = rhi();
        // ShowFreatures(m_rhi);
        createBuffer0();
        createBuffer1();
        createBuffer2();
        _pipeline0.reset();
        _pipeline1.reset();
        _pipeline2.reset();
    }

    if (_sampleCount != renderTarget()->sampleCount()) {
        _sampleCount = renderTarget()->sampleCount();
        _pipeline0.reset();
        _pipeline1.reset();
        _pipeline2.reset();
    }

    QRhiTexture *finalTex = _sampleCount > 1 ? resolveTexture() : colorTexture();
    if (_textureFormat != finalTex->format()) {
        _textureFormat = finalTex->format();
        _pipeline0.reset();
        _pipeline1.reset();
        _pipeline2.reset();
    }

    if (!_srb0) {
        createShaderResourceBinding0();
    }

    if (!_srb1) {
        createShaderResourceBinding1();
    }

    if (!_srb2) {
        createShaderResourceBinding2();
    }


    if (!_pipeline0) {
        createPipline0();
    }

    if (!_pipeline1) {
        createPipline1();
    }

    if (!_pipeline2) {
        createPipline2();
    }
}

void CurveRenderer::render(QRhiCommandBuffer *cb)
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

    glm::vec4 borderColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
    glm::vec4 guideLineColor = glm::vec4(0.0, 1.0, 0.0, 1.0);
    glm::vec4 fillColor = glm::vec4(1.0, 1.0, 0.0, 1.0);

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
                               &borderColor);


    offset = align(0, STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(_uniformBuffer1.get(),
                               offset,
                               sizeof(glm::mat4),
                               _view.constData());
    offset = align(sizeof(glm::mat4), STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(_uniformBuffer1.get(),
                               offset,
                               sizeof(glm::mat4),
                               _projection.constData());
    offset = align(sizeof(glm::mat4) + sizeof(glm::mat4),
                   STD140_ALIGN_VEC4);
    batch->updateDynamicBuffer(_uniformBuffer1.get(),
                               offset,
                               sizeof(glm::vec4),
                               &fillColor);


    offset = align(0, STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(_uniformBuffer2.get(),
                               offset,
                               sizeof(glm::mat4),
                               _view.constData());
    offset = align(sizeof(glm::mat4), STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(_uniformBuffer2.get(),
                               offset,
                               sizeof(glm::mat4),
                               _projection.constData());
    offset = align(sizeof(glm::mat4) + sizeof(glm::mat4),
                   STD140_ALIGN_VEC4);
    batch->updateDynamicBuffer(_uniformBuffer2.get(),
                               offset,
                               sizeof(glm::vec4),
                               &guideLineColor);


    // update outline VBO
    offset = 0;
    for (int i = 0; i < _shapes.size(); i ++) {
        std::vector<glm::vec3>& vertices = _shapes[i]->getOutlineVertices();
        qint32 size = vertices.size() * sizeof(glm::vec3);
        batch->uploadStaticBuffer(_vectexBuffer0.get(), offset, size, vertices.data());
        offset += size;

        // model vertices
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, qDegreesToRadians(_angle), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(_scale, _scale, _scale));
        model = glm::translate(model,glm::vec3(0.0f, 0.0f, 0.0f));
        batch->uploadStaticBuffer(_modelBuffer0.get(), i * sizeof(glm::mat4), sizeof(glm::mat4), &model);
    }

    // update shape VBO
    offset = 0;
    for (int i = 0; i < _shapes.size(); i ++) {
        // shape vertices
        std::vector<glm::vec3>& vertices = _shapes[i]->getShapeVertices();
        qint32 size = vertices.size() * sizeof(glm::vec3);
        batch->uploadStaticBuffer(_vectexBuffer1.get(), offset, size, vertices.data());
        offset += size;

        // model vertices
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, qDegreesToRadians(_angle), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(_scale, _scale, _scale));
        model = glm::translate(model,glm::vec3(0.0f, 0.0f, 0.0f));
        batch->uploadStaticBuffer(_modelBuffer1.get(), i * sizeof(glm::mat4), sizeof(glm::mat4), &model);
    }

    // update outline guide line VBO
    offset = 0;
    for (int i = 0; i < _shapes.size(); i ++) {
        std::vector<glm::vec3>& vertices = _shapes[i]->getGuideLineVertices();
        qint32 size = vertices.size() * sizeof(glm::vec3);
        batch->uploadStaticBuffer(_vectexBuffer2.get(), offset, size, vertices.data());
        offset += size;

        // model vertices
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, qDegreesToRadians(_angle), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(_scale, _scale, _scale));
        model = glm::translate(model,glm::vec3(0.0f, 0.0f, 0.0f));
        batch->uploadStaticBuffer(_modelBuffer2.get(), i * sizeof(glm::mat4), sizeof(glm::mat4), &model);
    }

    cb->resourceUpdate(batch);

    ///////////////////////////////////////////////////////////////////////////
    // draw border
    cb->setGraphicsPipeline(_pipeline0.get());
    cb->setShaderResources(_srb0.get());
    offset = 0;
    for (int i = 0; i < _shapes.size(); i ++) {
        std::vector<glm::vec3>& vertices = _shapes[i]->getOutlineVertices();
        const QRhiCommandBuffer::VertexInput inputBindings[] = {
            { _vectexBuffer0.get(), offset },
            { _modelBuffer0.get(), i * sizeof(glm::mat4) }
        };
        cb->setVertexInput(0, 2, inputBindings);
        cb->draw(vertices.size());
        offset += vertices.size() * sizeof(glm::vec3);
    }

    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // draw shape
    //
    cb->setGraphicsPipeline(_pipeline1.get());
    cb->setShaderResources(_srb1.get());
    offset = 0;
    for (int i = 0; i < _shapes.size(); i ++) {
        std::vector<glm::vec3>& vertices = _shapes[i]->getShapeVertices();

        const QRhiCommandBuffer::VertexInput inputBindings[] = {
            { _vectexBuffer1.get(), offset },
            { _modelBuffer1.get(), i * sizeof(glm::mat4) }
        };

        cb->setVertexInput(0, 2, inputBindings);
        cb->draw(vertices.size());
        offset += vertices.size() * sizeof(glm::vec3);
    }
    //
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // draw outline guide line
    //
    cb->setGraphicsPipeline(_pipeline2.get());
    cb->setShaderResources(_srb2.get());
    offset = 0;
    for (int i = 0; i < _shapes.size(); i ++) {
        std::vector<glm::vec3>& vertices = _shapes[i]->getGuideLineVertices();
        // for (int i = 0; i < vertices.size(); i ++) {
        //     printf("%0.1f ", vertices[i].z);
        // }

        const QRhiCommandBuffer::VertexInput inputBindings[] = {
            { _vectexBuffer2.get(), offset },
            { _modelBuffer2.get(), i * sizeof(glm::mat4) }
        };
        cb->setVertexInput(0, 2, inputBindings);
        cb->draw(vertices.size());
        offset += vertices.size() * sizeof(glm::vec3);
    }

    ///////////////////////////////////////////////////////////////////////////

    cb->endPass();
}

void CurveRenderer::synchronize(QQuickRhiItem *rhiItem)
{
    Curve *item = static_cast<Curve *>(rhiItem);
    if (item->angle() != _angle)
        _angle = item->angle();


    if (item->scale() != _scale)
        _scale = item->scale();

    _shapes = item->getShapes();

    _orthoX = item->getOrthoX();
    _orthoY = item->getOrthoY();
    _zoom = item->getZoom();
    _focus = item->getFocus();
}

Curve::Curve()
{
    setFocusPolicy(Qt::ClickFocus);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(ItemAcceptsInputMethod, true);

    Vangoh::Pen pen(Vangoh::SolidLine, Vangoh::FlatCap, Vangoh::RoundJoin, 2);

    Vangoh::Polygon* polygon1 = new Vangoh::Polygon({
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(100.0, 0.0, 0.0),
        glm::vec3(0, -100, 0.0),
        glm::vec3(100, -100.0, 0.0),
    });
    polygon1->setPen(pen);
    polygon1->draw();
    _shapes.push_back(polygon1);


    Vangoh::Polygon* polygon2 = new Vangoh::Polygon({
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0.0, 100.0, 0.0),
        glm::vec3(-100, 100, 0.0),
        glm::vec3(-100, 0.0, 0.0),
    });
    polygon2->setPen(pen);
    polygon2->draw();
    _shapes.push_back(polygon2);

    // line1 on X asix
    Vangoh::Line* line1 = new Vangoh::Line(
        glm::vec3(10.0, 0.0, 0.0),
        glm::vec3(40.0, 0.0, 0.0));
    line1->setPen(pen);
    line1->draw();
    _shapes.push_back(line1);

    // line2 on X asix
    Vangoh::Line* line2 = new Vangoh::Line(
        glm::vec3(20.0, 10.0, 0.0),
        glm::vec3(-40.0, 10.0, 0.0));
    line2->setPen(pen);
    line2->draw();
    _shapes.push_back(line2);


    // line3 on Y asix
    Vangoh::Line* line3 = new Vangoh::Line(
        glm::vec3(60.0, 0.0, 0.0),
        glm::vec3(60.0, 40.0, 0.0));
    line3->setPen(pen);
    line3->draw();
    _shapes.push_back(line3);

    // line4 on Y asix
    Vangoh::Line* line4 = new Vangoh::Line(
        glm::vec3(80.0, 0.0, 0.0),
        glm::vec3(80.0, -40.0, 0.0));
    line4->setPen(pen);
    line4->draw();
    _shapes.push_back(line4);


    // line5 on Z asix
    Vangoh::Line* line5 = new Vangoh::Line(
        glm::vec3(100.0, 0.0, 0.0),
        glm::vec3(100.0, 0.0, 40.0));
    line5->setPen(pen);
    line5->draw();
    _shapes.push_back(line5);

    // line6 on Z asix
    Vangoh::Line* line6 = new Vangoh::Line(
        glm::vec3(120.0, 0.0, 0.0),
        glm::vec3(120.0, 0.0, -40.0));
    line6->setPen(pen);
    line6->draw();
    _shapes.push_back(line6);


}

Curve::~Curve()
{
    for (auto ptr : _shapes) {
        delete ptr;
    }
    _shapes.clear();
}

std::vector<Vangoh::Shape*>& Curve::getShapes()
{
    return _shapes;
}

void Curve::hoverMoveEvent(QHoverEvent *event)
{
    if (_spaceButtonDown) {

        int offsetX = (int)event->position().x() -
                      _mosePosition0.x() - this->width()/2;
        int offsetY = this->height()/2 -
                      (int)event->position().y() - _mosePosition0.y();

        _focus.setX(offsetX);
        _focus.setY(offsetY);

        // qDebug() << m_focus << m_mosePosition0 << event->position();
    }
    return QQuickRhiItem::hoverMoveEvent(event);
}

void Curve::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!_leftButtonDown) {
            _leftButtonDown = true;
            qDebug("m_leftButtonDown true");
        }
    }
    return QQuickRhiItem::mousePressEvent(event);
}


void Curve::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (_leftButtonDown) {
            _leftButtonDown = false;
            qDebug("m_leftButtonDown false");
        }
    }
    return QQuickRhiItem::mouseReleaseEvent(event);
}

void Curve::wheelEvent(QWheelEvent *event)
{
    // qDebug() << "Mouse wheel delta: " << event->angleDelta();
    if (event->angleDelta().y() > 0) {
        _zoom += 10.0;
    }
    else if (event->angleDelta().y() < 0) {
        if (_zoom - 10.0 >= 10.0)
            _zoom -= 10.0;
    }
    return QQuickRhiItem::wheelEvent(event);
}


void Curve::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up) {
        _orthoY -= 100.0;
    }
    else if (event->key() == Qt::Key_Down) {
        _orthoY += 100.0;
    }
    else if (event->key() == Qt::Key_Left) {
        _orthoX += 100.0;
    }
    else if (event->key() == Qt::Key_Right) {
        _orthoX -= 100.0;
    }
    else if (event->key() == Qt::Key_Space) {
        if (!_spaceButtonDown) {
            _spaceButtonDown = true;
            qDebug("m_spaceButtonDown true");
        }
    }
    else if (event->key() == Qt::Key_R) {
        renderToTexture();
    }

    return QQuickRhiItem::keyPressEvent(event);
}

void Curve::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        if (_spaceButtonDown) {
            _spaceButtonDown = false;
            qDebug("m_spaceButtonDown false");
        }
    }

    return QQuickRhiItem::keyReleaseEvent(event);
}

QQuickRhiItemRenderer* Curve::createRenderer()
{
    return new CurveRenderer();
}


float Curve::angle() const
{
    return _angle;
}

void Curve::setAngle(float a)
{
    if (_angle == a)
        return;

    _angle = a;
    emit angleChanged();
    update();
}

float Curve::scale() const
{
    return _scale;
}

void Curve::setScale(float s)
{
    if (_scale == s)
        return;

    _scale = s;
    emit angleChanged();
    update();
}

float Curve::getOrthoX()
{
    return _orthoX;
}

float Curve::getOrthoY()
{
    return _orthoY;
}

float Curve::getZoom()
{
    return _zoom;
}

QPointF& Curve::getFocus()
{
    return _focus;
}

void Curve::renderToTexture()
{
    qDebug("render!");

    std::unique_ptr<QRhi> rhi;

#if defined(Q_OS_WIN)
    QRhiD3D11InitParams params;
    params.enableDebugLayer = true;
    rhi.reset(QRhi::create(QRhi::D3D11, &params));
#elif defined(Q_OS_MACOS)
    QRhiMetalInitParams params;
    rhi.reset(QRhi::create(QRhi::Metal, &params));
#else
    QRhiVulkanInitParams params;
    params.inst = vulkanInstance();
    params.window = this;
    rhi.reset(QRhi::create(QRhi::Vulkan, &params));
#endif
    if (rhi)
        qDebug() << rhi->backendName() << rhi->driverInfo();
    else
        qFatal("Failed to initialize RHI");

    float rotation = 0.0f;
    float opacity = 1.0f;
    int opacityDir = 1;

    std::unique_ptr<QRhiTexture> tex(rhi->newTexture(QRhiTexture::RGBA8,
                                                     QSize(800, 600),
                                                     1,
                                                     QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
    tex->create();

    std::unique_ptr<QRhiTextureRenderTarget> rt(
        rhi->newTextureRenderTarget({ tex.get() }));
    std::unique_ptr<QRhiRenderPassDescriptor> rp(rt->newCompatibleRenderPassDescriptor());

    rt->setRenderPassDescriptor(rp.get());
    rt->create();

    QMatrix4x4 viewProjection = rhi->clipSpaceCorrMatrix();
    viewProjection.perspective(45.0f, 1280 / 720.f, 0.01f, 1000.0f);
    viewProjection.translate(0, 0, -4);

    static float vertexData[] = {
        // Y up, CCW
        0.0f,   0.5f,     1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,     0.0f, 1.0f, 0.0f,
        0.5f,  -0.5f,     0.0f, 0.0f, 1.0f,
    };

    std::unique_ptr<QRhiBuffer> vbuf(rhi->newBuffer(QRhiBuffer::Immutable,
                                                    QRhiBuffer::VertexBuffer,
                                                    sizeof(vertexData)));
    vbuf->create();

    std::unique_ptr<QRhiBuffer> ubuf(rhi->newBuffer(QRhiBuffer::Dynamic,
                                                    QRhiBuffer::UniformBuffer,
                                                    64 + 4));
    ubuf->create();

    std::unique_ptr<QRhiShaderResourceBindings> srb(rhi->newShaderResourceBindings());
    srb->setBindings({
        QRhiShaderResourceBinding::uniformBuffer(0,
                                                 QRhiShaderResourceBinding::VertexStage | QRhiShaderResourceBinding::FragmentStage,
                                                 ubuf.get())
    });
    srb->create();

    std::unique_ptr<QRhiGraphicsPipeline> ps(rhi->newGraphicsPipeline());
    QRhiGraphicsPipeline::TargetBlend premulAlphaBlend;
    premulAlphaBlend.enable = true;
    ps->setTargetBlends({ premulAlphaBlend });
    static auto getShader = [](const QString &name) {
        QFile f(name);
        return f.open(QIODevice::ReadOnly) ? QShader::fromSerialized(f.readAll()) : QShader();
    };
    ps->setShaderStages({
        { QRhiShaderStage::Vertex, getShader(QLatin1String(":/AnymateLab/shaders/color.vert.qsb")) },
        { QRhiShaderStage::Fragment, getShader(QLatin1String(":/AnymateLab/shaders/color.frag.qsb")) }
    });


    QRhiVertexInputLayout inputLayout;
    inputLayout.setBindings({
        { 5 * sizeof(float) }
    });
    inputLayout.setAttributes({
        { 0, 0, QRhiVertexInputAttribute::Float2, 0 },
        { 0, 1, QRhiVertexInputAttribute::Float3, 2 * sizeof(float) }
    });
    ps->setVertexInputLayout(inputLayout);
    ps->setShaderResourceBindings(srb.get());
    ps->setRenderPassDescriptor(rp.get());
    ps->create();

    QRhiCommandBuffer *cb;
    for (int frame = 0; frame < 20; ++frame) {
        rhi->beginOffscreenFrame(&cb);

        QRhiResourceUpdateBatch *u = rhi->nextResourceUpdateBatch();
        if (frame == 0)
            u->uploadStaticBuffer(vbuf.get(), vertexData);

        QMatrix4x4 mvp = viewProjection;
        mvp.rotate(rotation, 0, 1, 0);
        u->updateDynamicBuffer(ubuf.get(), 0, 64, mvp.constData());
        rotation += 5.0f;

        u->updateDynamicBuffer(ubuf.get(), 64, 4, &opacity);
        opacity += opacityDir * 0.2f;
        if (opacity < 0.0f || opacity > 1.0f) {
            opacityDir *= -1;
            opacity = qBound(0.0f, opacity, 1.0f);
        }

        const QColor clearColor = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f);
        cb->beginPass(rt.get(), clearColor, { 1.0f, 0 }, u);
        cb->setGraphicsPipeline(ps.get());
        cb->setViewport({ 0, 0, 1280, 720 });
        cb->setShaderResources();
        const QRhiCommandBuffer::VertexInput vbufBinding(vbuf.get(), 0);
        cb->setVertexInput(0, 1, &vbufBinding);
        cb->draw(3);
        QRhiReadbackResult readbackResult;
        u = rhi->nextResourceUpdateBatch();
        u->readBackTexture({ tex.get() }, &readbackResult);
        cb->endPass(u);

        rhi->endOffscreenFrame();

        QImage image(reinterpret_cast<const uchar *>(readbackResult.data.constData()),
                     readbackResult.pixelSize.width(),
                     readbackResult.pixelSize.height(),
                     QImage::Format_RGBA8888_Premultiplied);
        if (rhi->isYUpInFramebuffer())
            image = image.mirrored();
        image.save(QString::asprintf("frame%d.png", frame));
    }

}
