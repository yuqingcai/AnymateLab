#include "drawer.h"
#include <QFile>
#include <QtGui/qevent.h>
#include <cstdio>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

DrawerRenderer::DrawerRenderer()
{
    _drawerRef = nullptr;
}

DrawerRenderer::~DrawerRenderer()
{
}

int DrawerRenderer::createBuffer0()
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

int DrawerRenderer::createShaderResourceBinding0()
{

    if (!_rhi)
        return -1;

    // view and projection uniform
    int blockSize = sizeof(glm::mat4) +
                    sizeof(glm::mat4) +
                    sizeof(glm::vec4);
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

int DrawerRenderer::createPipline0()
{
    if (!_rhi || !_srb0)
        return -1;

    _pipeline0.reset(_rhi->newGraphicsPipeline());

    _pipeline0->setShaderStages({{QRhiShaderStage::Vertex,
                                  getShader(QLatin1String(":/AnymateLab/shaders/curve.vert.qsb"))},
                                 {QRhiShaderStage::Fragment,
                                  getShader(QLatin1String(":/AnymateLab/shaders/curve.frag.qsb"))}});

    QRhiVertexInputLayout inputLayout;
    inputLayout.setBindings(
        {
            {3 * sizeof(float), QRhiVertexInputBinding::PerVertex},
            {64, QRhiVertexInputBinding::PerInstance},
        });

    inputLayout.setAttributes(
        {
            // binding0
            {0, 0, QRhiVertexInputAttribute::Float3, 0},
            // binding1
            {1, 1, QRhiVertexInputAttribute::Float4, 0},
            {1, 2, QRhiVertexInputAttribute::Float4, 4 * sizeof(float)},
            {1, 3, QRhiVertexInputAttribute::Float4, 8 * sizeof(float)},
            {1, 4, QRhiVertexInputAttribute::Float4, 12 * sizeof(float)},
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

int DrawerRenderer::createBuffer1()
{
    if (!_rhi)
        return -1;

    size_t verticesSize = sizeof(float) * 1024 * 1024 * 4;

    _vectexBuffer1.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                         QRhiBuffer::VertexBuffer,
                                         verticesSize));
    _vectexBuffer1->create();

    _indexBuffer1.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                        QRhiBuffer::IndexBuffer,
                                        verticesSize));
    _indexBuffer1->create();

    _modelBuffer1.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                        QRhiBuffer::VertexBuffer,
                                        sizeof(glm::mat4) * 1024));
    _modelBuffer1->create();

    return 0;
}

int DrawerRenderer::createShaderResourceBinding1()
{

    if (!_rhi)
        return -1;

    // view and projection uniform
    int blockSize = sizeof(glm::mat4) +
                    sizeof(glm::mat4) +
                    sizeof(glm::vec4);
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
    return 0;
}

int DrawerRenderer::createPipline1()
{
    if (!_rhi || !_srb1)
        return -1;

    _pipeline1.reset(_rhi->newGraphicsPipeline());

    _pipeline1->setShaderStages({{QRhiShaderStage::Vertex,
                                  getShader(QLatin1String(":/AnymateLab/shaders/curve.vert.qsb"))},
                                 {QRhiShaderStage::Fragment,
                                  getShader(QLatin1String(":/AnymateLab/shaders/curve.frag.qsb"))}});

    QRhiVertexInputLayout inputLayout;
    inputLayout.setBindings(
        {
            {3 * sizeof(float), QRhiVertexInputBinding::PerVertex},
            {64, QRhiVertexInputBinding::PerInstance},
        });

    inputLayout.setAttributes(
        {
            // binding0
            {0, 0, QRhiVertexInputAttribute::Float3, 0},
            // binding1
            {1, 1, QRhiVertexInputAttribute::Float4, 0},
            {1, 2, QRhiVertexInputAttribute::Float4, 4 * sizeof(float)},
            {1, 3, QRhiVertexInputAttribute::Float4, 8 * sizeof(float)},
            {1, 4, QRhiVertexInputAttribute::Float4, 12 * sizeof(float)},
        });

    _pipeline1->setVertexInputLayout(inputLayout);
    _pipeline1->setSampleCount(_sampleCount);
    _pipeline1->setShaderResourceBindings(_srb0.get());
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
    return 0;
}

void DrawerRenderer::initialize(QRhiCommandBuffer *cb)
{

    if (_rhi != rhi())
    {
        _rhi = rhi();
        createBuffer0();
        _pipeline0.reset();

        createBuffer1();
        _pipeline1.reset();
    }

    if (_sampleCount != renderTarget()->sampleCount())
    {
        _sampleCount = renderTarget()->sampleCount();
        _pipeline0.reset();
        _pipeline1.reset();
    }

    QRhiTexture *finalTex = _sampleCount > 1 ? resolveTexture() : colorTexture();
    if (_textureFormat != finalTex->format())
    {
        _textureFormat = finalTex->format();
        _pipeline0.reset();
        _pipeline1.reset();
    }

    if (!_srb0)
    {
        createShaderResourceBinding0();
    }

    if (!_pipeline0)
    {
        createPipline0();
    }

    if (!_srb1)
    {
        createShaderResourceBinding1();
    }

    if (!_pipeline1)
    {
        createPipline1();
    }
}

void DrawerRenderer::render(QRhiCommandBuffer *cb)
{
    QVector3D cameraPos(_focus.rx(), _focus.ry(), 10.0);
    QVector3D cameraUp(0.0f, 1.0f, 0.0f);
    QVector3D cameraTarget(_focus.rx(), _focus.ry(), 0.0f);

    const QSize outputSize = renderTarget()->pixelSize();
    _projectionMatrix = _rhi->clipSpaceCorrMatrix();

    // 正交投影的视景体（视锥体）的near和far的距离是相对观察点而言的（相机），而不是
    // 相对零点的。举例：如果相机的z坐标 z=10，near=10，far=20，那么视景体的z范围
    // 是 [0, -10]，超过这个范围的物体都会被裁剪。
    _projectionMatrix.ortho(0.0 - outputSize.width()/4 ,
                            0.0 + outputSize.width()/4 ,
                            0.0 - outputSize.height()/4,
                            0.0 + outputSize.height()/4,
                            10.0f,
                            20.0f);
    // _projectionMatrix.perspective(45.0f,
    //                         outputSize.width() / (float)outputSize.height(),
    //                         10.0f,
    //                         200.0f);

    _viewMatrix.setToIdentity();
    _viewMatrix.lookAt(cameraPos, cameraTarget, cameraUp);

    if (_drawerRef) {
        _drawerRef->setProjectionMatrix(_projectionMatrix);
        _drawerRef->setViewProjectMatrix(_viewMatrix);
    }

    QRhiResourceUpdateBatch *batch = _rhi->nextResourceUpdateBatch();

    const QColor clearColor = QColor::fromRgbF(1.0f, 1.0f, 1.0f, 1.0f);
    cb->beginPass(renderTarget(), clearColor, {1.0f, 0}, batch);
    cb->setViewport(QRhiViewport(0, 0, outputSize.width(), outputSize.height()));

    size_t offset = 0;
    glm::vec4 borderColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
    glm::vec4 fillColor = glm::vec4(1.0, 1.0, 0.0, 1.0);

    offset = align(0, STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(_uniformBuffer0.get(),
                               offset,
                               sizeof(glm::mat4),
                               _viewMatrix.constData());
    offset = align(sizeof(glm::mat4), STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(_uniformBuffer0.get(),
                               offset,
                               sizeof(glm::mat4),
                               _projectionMatrix.constData());
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
                               _viewMatrix.constData());
    offset = align(sizeof(glm::mat4), STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(_uniformBuffer1.get(),
                               offset,
                               sizeof(glm::mat4),
                               _projectionMatrix.constData());

    offset = align(sizeof(glm::mat4) + sizeof(glm::mat4),
                   STD140_ALIGN_VEC4);
    batch->updateDynamicBuffer(_uniformBuffer1.get(),
                               offset,
                               sizeof(glm::vec4),
                               &fillColor);

    offset = 0;
    for (int i = 0; i < _shapes.size(); i++)
    {
        std::vector<glm::vec2> &vertices2D = _shapes[i]->getOutlineVertices();
        std::vector<glm::vec3> vertices;
        for (glm::vec2 p : vertices2D)
        {
            vertices.push_back(glm::vec3(p.x, p.y, 0.0));
        }
        qint32 size = vertices.size() * sizeof(glm::vec3);
        batch->uploadStaticBuffer(_vectexBuffer0.get(), offset, size, vertices.data());
        offset += size;

        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::rotate(model, qDegreesToRadians(_angle), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        batch->uploadStaticBuffer(_modelBuffer0.get(), i * sizeof(glm::mat4),
                                  sizeof(glm::mat4), &model);
    }

    offset = 0;
    for (int i = 0; i < _shapes.size(); i++)
    {
        std::vector<glm::vec2> &vertices2D = _shapes[i]->getShapeVertices();
        std::vector<glm::vec3> vertices;
        for (glm::vec2 p : vertices2D)
        {
            vertices.push_back(glm::vec3(p.x, p.y, 0.0));
        }

        qint32 size = vertices.size() * sizeof(glm::vec3);
        batch->uploadStaticBuffer(_vectexBuffer1.get(), offset,
                                  size, vertices.data());
        offset += size;

        // model vertices
        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::rotate(model, qDegreesToRadians(_angle), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(_scale, _scale, _scale));
        // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        batch->uploadStaticBuffer(_modelBuffer1.get(), i * sizeof(glm::mat4), sizeof(glm::mat4), &model);
    }

    cb->resourceUpdate(batch);

    cb->setGraphicsPipeline(_pipeline0.get());
    cb->setShaderResources(_srb0.get());
    offset = 0;
    for (int i = 0; i < _shapes.size(); i++)
    {
        std::vector<glm::vec2> &vertices = _shapes[i]->getOutlineVertices();
        const QRhiCommandBuffer::VertexInput inputBindings[] = {
            {_vectexBuffer0.get(), offset},
            {_modelBuffer0.get(), i * sizeof(glm::mat4)}};
        cb->setVertexInput(0, 2, inputBindings);
        cb->draw(vertices.size());
        offset += vertices.size() * sizeof(glm::vec3);
    }

    cb->setGraphicsPipeline(_pipeline1.get());
    cb->setShaderResources(_srb1.get());
    offset = 0;
    for (int i = 0; i < _shapes.size(); i++)
    {
        std::vector<glm::vec2> &vertices = _shapes[i]->getShapeVertices();
        const QRhiCommandBuffer::VertexInput inputBindings[] = {
            {_vectexBuffer1.get(), offset},
            {_modelBuffer1.get(), i * sizeof(glm::mat4)}};
        cb->setVertexInput(0, 2, inputBindings);
        cb->draw(vertices.size());
        offset += vertices.size() * sizeof(glm::vec3);
    }

    cb->endPass();
}

void DrawerRenderer::synchronize(QQuickRhiItem *rhiItem)
{
    Drawer *item = static_cast<Drawer *>(rhiItem);
    if (item->angle() != _angle)
        _angle = item->angle();

    _drawerRef = item;
    _orthoX = item->getOrthoX();
    _orthoY = item->getOrthoY();
    _zoom = item->getZoom();
    _focus = item->getFocus();

    _shapes = item->getShapes();
}

Drawer::Drawer()
{
    _viewMatrix.setToIdentity();
    _projectionMatrix.setToIdentity();
    _activedPointIndex = -1;

    glm::vec2 p(
        _graphicOrigin.x(),
        _graphicOrigin.y()

        );
    createBezier(p, 200, 200);
}

void Drawer::createRectangle(glm::vec2& position, float width, float height)
{
    float x = position.x;
    float y = position.y;
    float a = width / 2.0;
    float b = height / 2.0;
    Vangoh::Pen pen0(Vangoh::SolidLine, Vangoh::FlatCap, Vangoh::RoundJoin, 1.0);
    Vangoh::Polygon *rect = new Vangoh::Polygon({
        glm::vec2(x - a, y - b),
        glm::vec2(x + a, y - b),
        glm::vec2(x + a, y + b),
        glm::vec2(x - a, y + b),
    });

    rect->setPen(pen0);
    rect->createVertices();
    _shapes.push_back(rect);
}

void Drawer::createBezier(glm::vec2& position, float width, float height)
{
    float x = position.x;
    float y = position.y;
    float a = width / 2.0;
    float b = height / 2.0;

    Vangoh::Pen pen0(Vangoh::SolidLine, Vangoh::FlatCap, Vangoh::RoundJoin, 2.0);
    _currentCurve = new Vangoh::BezierCurve(
        glm::vec2(x - a,  y - b),
        glm::vec2(x, y - b),
        glm::vec2(x, y + b),
        glm::vec2(x + a, y + b)
        );
    _currentCurve->setPen(pen0);
    _currentCurve->createVertices();
    _shapes.push_back(_currentCurve);

    Vangoh::Pen pen1(Vangoh::SolidLine, Vangoh::FlatCap, Vangoh::RoundJoin, 1.0);
    _p0 = new Vangoh::Rectangle(_currentCurve->getPoint0(), 10.0, 10.0);
    _p0->setPen(pen1);
    _p0->createVertices();
    _shapes.push_back(_p0);

    _p1 = new Vangoh::Rectangle(_currentCurve->getPoint1(), 10.0, 10.0);
    _p1->setPen(pen1);
    _p1->createVertices();
    _shapes.push_back(_p1);

    _p2 = new Vangoh::Rectangle(_currentCurve->getPoint2(), 10.0, 10.0);
    _p2->setPen(pen1);
    _p2->createVertices();
    _shapes.push_back(_p2);

    _p3 = new Vangoh::Rectangle(_currentCurve->getPoint3(), 10.0, 10.0);
    _p3->setPen(pen1);
    _p3->createVertices();
    _shapes.push_back(_p3);

    _l0 = new Vangoh::Line(_currentCurve->getPoint0(), _currentCurve->getPoint1());
    _l0->setPen(pen1);
    _l0->createVertices();
    _shapes.push_back(_l0);

    _l1 = new Vangoh::Line(_currentCurve->getPoint2(), _currentCurve->getPoint3());
    _l1->setPen(pen1);
    _l1->createVertices();
    _shapes.push_back(_l1);

}

Drawer::~Drawer()
{
    for (auto ptr : _shapes)
    {
        delete ptr;
    }
    _shapes.clear();
}

QQuickRhiItemRenderer *Drawer::createRenderer()
{
    return new DrawerRenderer();
}

std::vector<Vangoh::Shape *>& Drawer::getShapes()
{
    return _shapes;
}

bool Drawer::pointInRange(glm::vec2 p0, glm::vec2 p1)
{
    return (p0.x >= p1.x - 10.0 && p0.x <= p1.x + 10 &&
            p0.y >= p1.y - 10.0 && p0.y <= p1.y + 10.0);
}

void Drawer::mouseMoveEvent(QMouseEvent *event) {
    if (!_currentCurve) {
        return;
    }

    QVector2D mousePosition = QVector2D(event->position().x(),
                                        event->position().y());
    QVector2D worldPosition = mousePositionToWordPosition(mousePosition);
    glm::vec2 p(worldPosition.x(), worldPosition.y());

    if (event->buttons() & Qt::LeftButton) {

        glm::vec2 point0 = _currentCurve->getPoint0();
        glm::vec2 point1 = _currentCurve->getPoint1();
        glm::vec2 point2 = _currentCurve->getPoint2();
        glm::vec2 point3 = _currentCurve->getPoint3();

        if (_activedPointIndex == -1) {
            if (pointInRange(p, point0)) {
                _activedPointIndex = 0;
            }
            else if (pointInRange(p, point1)) {
                _activedPointIndex = 1;
            }
            else if (pointInRange(p, point2)) {
                _activedPointIndex = 2;
            }
            else if (pointInRange(p, point3)) {
                _activedPointIndex = 3;
            }
        }

        QPointF currentPosition = event->position();
        float deltaX = currentPosition.x() - _lastMousePosition.x();
        float deltaY = currentPosition.y() - _lastMousePosition.y();
        _lastMousePosition = event->position();

        if (_activedPointIndex == 0) {
            point0.x += deltaX;
            point0.y -= deltaY;
            _currentCurve->setPoint0(point0);
            _p0->setCenter(point0);
            _l0->setPoint0(point0);
        }
        else if (_activedPointIndex == 1) {
            point1.x += deltaX;
            point1.y -= deltaY;
            _currentCurve->setPoint1(point1);
            _p1->setCenter(point1);
            _l0->setPoint1(point1);
        }
        else if (_activedPointIndex == 2) {
            point2.x += deltaX;
            point2.y -= deltaY;
            _currentCurve->setPoint2(point2);
            _p2->setCenter(point2);
            _l1->setPoint0(point2);
        }
        else if (_activedPointIndex == 3) {
            point3.x += deltaX;
            point3.y -= deltaY;
            _currentCurve->setPoint3(point3);
            _p3->setCenter(point3);
            _l1->setPoint1(point3);
        }

    }
    return;
}

void Drawer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        _lastMousePosition = event->position();
    }
    return;
}

void Drawer::mouseReleaseEvent(QMouseEvent *event)
{
    _activedPointIndex = -1;
    return ;
}

void Drawer::mouseDoubleClickEvent(QMouseEvent *event)
{
    QVector2D mousePosition = QVector2D(event->position().x(),
                                        event->position().y());

    QVector2D worldPosition = mousePositionToWordPosition(mousePosition);
    _graphicOrigin = QPointF(worldPosition.x(), worldPosition.y());
}

void Drawer::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        for (auto ptr : _shapes)
        {
            delete ptr;
        }
        _shapes.clear();
        _currentCurve = nullptr;
        _p0 = nullptr;
        _p1 = nullptr;
        _p2 = nullptr;
        _p3 = nullptr;
        _l0 = nullptr;
        _l1 = nullptr;
    }

    if (event->key() == Qt::Key_C) {
        glm::vec2 p(
            _graphicOrigin.x(),
            _graphicOrigin.y()
        );
        createBezier(p, 200, 200);
    }

    return  SampleItem::keyPressEvent(event);
}

void Drawer::setProjectionMatrix(QMatrix4x4& matrix)
{
    _projectionMatrix = matrix;
}

void Drawer::setViewProjectMatrix(QMatrix4x4& matrix)
{
    _viewMatrix = matrix;
}

QVector2D Drawer::mousePositionToWordPosition(QVector2D& position)
{
    QVector4D windowPos(
        position.x() ,
        this->height() - position.y(),
        0.0,
        1.0);
    QVector4D ndcPos(
        ((2.0 * windowPos.x()) / this->width()) - 1.0,
        ((2.0 * windowPos.y()) / this->height()) - 1.0,
        0.0,
        1.0);

    bool invertible;
    QMatrix4x4 inverseVP =
        (_projectionMatrix*_viewMatrix).inverted(&invertible);
    if (invertible) {
        QVector4D worldPos = inverseVP * ndcPos;
        worldPos = worldPos/worldPos.w();
        return QVector2D(worldPos.x(), worldPos.y());
    }
    return QVector2D(0.0, 0.0);

}
