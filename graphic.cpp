#include "graphic.h"

namespace Anymate {
Graphic::Graphic():
    _x(0.0),
    _y(0.0),
    _width(0.0),
    _height(0.0)
{

}

void Graphic::setX(float x)
{
    _x = x;
}

void Graphic::setY(float y)
{
    _y = y;
}

void Graphic::setWidth(float width)
{
    _width = width;
}

void Graphic::setHeight(float height)
{
    _height = height;
}

float Graphic::getX()
{
    return _x;
}

float Graphic::getY()
{
    return _y;
}

float Graphic::getWidth()
{
    return _width;
}

float Graphic::getHeight()
{
    return _height;
}

GeometryShape::GeometryShape():
    _borderType(None),
    _borderWidth(0.0),
    _curveSegments(0),
    _vertexBuffer(nullptr),
    _vertexCount(0),
    _vertexBufferSize(0)
{
}

GeometryShape::~ GeometryShape()
{
    if (_vertexBuffer) {
        delete [] _vertexBuffer;
    }
}

void GeometryShape::setBorderType(BorderType type)
{
    _borderType = type;
}

void GeometryShape::setBorderWidth(float borderWidth)
{
    _borderWidth = borderWidth;
}

GeometryShape::BorderType GeometryShape::getBorderType()
{
    return _borderType;
}

float GeometryShape::getBorderWidth()
{
    return _borderWidth;
}

void GeometryShape::setCurveSegments(uint32_t segments)
{
    _curveSegments = segments;
}

uint32_t GeometryShape::getCurveSegments()
{
    return _curveSegments;
}

float* GeometryShape::getVertexBuffer()
{
    return _vertexBuffer;
}

uint32_t GeometryShape::getVertexCount()
{
    return _vertexCount;
}

uint32_t GeometryShape::getVertexBufferSize()
{
    return _vertexBufferSize;
}

void GeometryShape::generateCurve()
{
    if (_vertexBuffer) {
        delete [] _vertexBuffer;
    }
}

Rectangle::Rectangle()
{
    _borderType = Solid;
    _borderWidth = 1.0;
}

Rectangle::~ Rectangle()
{

}

void Rectangle::generateCurve()
{
    GeometryShape::generateCurve();

    _vertexCount = 17;
    _vertexBufferSize = sizeof(float) * _vertexCount * 2;
    _vertexBuffer = new float[_vertexCount * 2];

    float hw = _width/2.0;
    float hh = _height/2.0;

    glm::vec2 p0(_x - hw, _y + hh);
    glm::vec2 p1(_x + hw, _y + hh);
    glm::vec2 p2(_x + hw, _y - hh);
    glm::vec2 p3(_x - hw, _y - hh);

    float w = _borderWidth / 2.0;

    // v0
    _vertexBuffer[0] = p0.x - w;
    _vertexBuffer[1] = p0.y + w;

    // v1
    _vertexBuffer[2] = p0.x - w;
    _vertexBuffer[3] = p0.y - w;

    // v2
    _vertexBuffer[4] = p1.x - w;
    _vertexBuffer[5] = p1.y + w;

    // v3
    _vertexBuffer[6] = p1.x - w;
    _vertexBuffer[7] = p1.y - w;

    // v4
    _vertexBuffer[8] = p1.x + w;
    _vertexBuffer[9] = p1.y + w;

    // v5
    _vertexBuffer[10] = p1.x - w;
    _vertexBuffer[11] = p1.y - w;

    // v6
    _vertexBuffer[12] = p2.x + w;
    _vertexBuffer[13] = p2.y + w;

    // v7
    _vertexBuffer[14] = p2.x - w;
    _vertexBuffer[15] = p2.y + w;

    // v8
    _vertexBuffer[16] = p2.x + w;
    _vertexBuffer[17] = p2.y - w;

    // v9
    _vertexBuffer[18] = p2.x - w;
    _vertexBuffer[19] = p2.y + w;

    // v10
    _vertexBuffer[20] = p3.x + w;
    _vertexBuffer[21] = p3.y - w;

    // v11
    _vertexBuffer[22] = p3.x + w;
    _vertexBuffer[23] = p3.y + w;

    // v12
    _vertexBuffer[24] = p3.x - w;
    _vertexBuffer[25] = p3.y - w;

    // v13
    _vertexBuffer[26] = p3.x - w;
    _vertexBuffer[27] = p3.y + w;

    // v14
    _vertexBuffer[28] = p3.x + w;
    _vertexBuffer[29] = p3.y + w;

    // v15
    _vertexBuffer[30] = p0.x - w;
    _vertexBuffer[31] = p0.y - w;

    // v16
    _vertexBuffer[32] = p0.x + w;
    _vertexBuffer[33] = p0.y - w;
}

Bezier::Bezier()
{
    _curveSegments = 100;
    _width = 1.0;
}

Bezier::Bezier(std::initializer_list<glm::vec2> list)
{
    _curveSegments = 100;
    _width = 1.0;

    for (auto elem : list) {
        _points.push_back(elem);
    }
}

Bezier:: ~ Bezier()
{

}

void Bezier::appendPoint(glm::vec2 point)
{
    _points.push_back(point);
}

glm::vec2 Bezier::bezier(float t, const glm::vec2& p0, const glm::vec2& p1,
                            const glm::vec2& p2, const glm::vec2& p3)
{
    float u = 1.0 - t;
    return (float)(u * u * u) * p0 +
           (float)(3.0 * u * u * t) * p1 +
           (float)(3.0 * u * t * t) * p2 +
           (float)(t * t * t) * p3;
}

void Bezier::generateCurve()
{
    GeometryShape::generateCurve();

    _vertexCount = _curveSegments * 2;
    _vertexBufferSize = sizeof(float) * _vertexCount * 2;

    _vertexBuffer = new float[_vertexCount * 2];

    for (int i = 0; i < _curveSegments; i ++) {

        float t = i / (float)_curveSegments;
        float delta = 1.0f / _curveSegments;

        glm::vec2 currentPoint = bezier(t,
                                        _points[0],
                                        _points[1],
                                        _points[2],
                                        _points[3]);
        glm::vec2 nextPoint = bezier(std::min(t + delta, 1.0f),
                                     _points[0],
                                     _points[1],
                                     _points[2],
                                     _points[3]);

        glm::vec2 tangent = glm::normalize((nextPoint - currentPoint));
        glm::vec2 normal = glm::vec2(-tangent.y, tangent.x);
        glm::vec2 left = currentPoint + (normal * (float)(_width / 2.0));
        glm::vec2 right = currentPoint - (normal * (float)(_width / 2.0));

        _vertexBuffer[i * 2 * 2 + 0] = left.x;
        _vertexBuffer[i * 2 * 2 + 1] = left.y;
        _vertexBuffer[i * 2 * 2 + 2] = right.x;
        _vertexBuffer[i * 2 * 2 + 3] = right.y;
    }
}


Circle::Circle()
{
    _radius = 0;
    _borderType = Solid;
    _borderWidth = 1.0;
    _curveSegments = 100;
}

Circle::Circle(float radius)
{
    _radius = radius;
    _borderType = Solid;
    _borderWidth = 1.0;
    _curveSegments = 100;
}

Circle:: ~ Circle()
{

}

void Circle::setRadius(float radius)
{
    _radius = radius;
}

float Circle::getRadius()
{
    return _radius;
}

void Circle::generateCurve()
{
    GeometryShape::generateCurve();

    _vertexCount = (_curveSegments + 1) * 2;
    _vertexBufferSize = sizeof(float) * _vertexCount * 2;

    _vertexBuffer = new float[_vertexCount * 2];

    for (int i = 0; i <= _curveSegments; i ++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(_curveSegments);
        float delta = 1.0f / _curveSegments;

        glm::vec2 currentPoint = glm::vec2(_radius * cosf(theta),
                                           _radius * sinf(theta));
        glm::vec2 nextPoint = glm::vec2(_radius * cosf(theta+delta),
                                        _radius * sinf(theta+delta));

        glm::vec2 tangent = glm::normalize((nextPoint - currentPoint));
        glm::vec2 normal = glm::vec2(-tangent.y, tangent.x);
        glm::vec2 left = currentPoint + (normal * (float)(_borderWidth / 2.0));
        glm::vec2 right = currentPoint - (normal * (float)(_borderWidth / 2.0));

        _vertexBuffer[i * 2 * 2 + 0] = left.x;
        _vertexBuffer[i * 2 * 2 + 1] = left.y;
        _vertexBuffer[i * 2 * 2 + 2] = right.x;
        _vertexBuffer[i * 2 * 2 + 3] = right.y;
    }
}

}
