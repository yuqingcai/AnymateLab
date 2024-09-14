#include "graphic.h"
#include <cmath>
#include <cstring>

namespace Anymate {
Graphic::Graphic():
    _x(0.0),
    _y(0.0),
    _width(0.0),
    _height(0.0)
{

}

Graphic::Graphic(float x, float y, float width, float height)
{
    _x = x;
    _y = y;
    _width = width;
    _height = height;
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
    _borderWidth(1.0),
    _borderStyle(SolidLineBorder),
    _borderColor(glm::vec4(0.0, 0.0, 0.0, 1.0)),
    _fillStyle(SolidFill),
    _fillSolidColor(glm::vec4(1.0, 1.0, 1.0, 1.0)),
    _shapePrecision(GeometryShape::defaultShapePrecision),
    _shapeVertices(nullptr),
    _shapeVertexCount(0),
    _shapeVertexBufferSize(0),
    _borderVertices(nullptr),
    _borderVertexCount(0),
    _borderVertexBufferSize(0)
{
}

GeometryShape::GeometryShape(float x, float y, float width, float height)
    :Graphic(x, y, width, height),
    _borderWidth(1.0),
    _borderStyle(SolidLineBorder),
    _borderColor(glm::vec4(0.0, 0.0, 0.0, 1.0)),
    _fillStyle(SolidFill),
    _fillSolidColor(glm::vec4(1.0, 1.0, 1.0, 1.0)),
    _shapePrecision(GeometryShape::defaultShapePrecision),
    _shapeVertices(nullptr),
    _shapeVertexCount(0),
    _shapeVertexBufferSize(0),
    _borderVertices(nullptr),
    _borderVertexCount(0),
    _borderVertexBufferSize(0)
{

}

GeometryShape::~ GeometryShape()
{
    if (_shapeVertices) {
        delete [] _shapeVertices;
    }

    if (_borderVertices) {
        delete [] _borderVertices;
    }
}

void GeometryShape::setBorderStyle(BorderStyle type)
{
    _borderStyle = type;
}

void GeometryShape::setBorderWidth(float borderWidth)
{
    _borderWidth = borderWidth;
}

GeometryShape::BorderStyle GeometryShape::getBorderStyle()
{
    return _borderStyle;
}

float GeometryShape::getBorderWidth()
{
    return _borderWidth;
}

void GeometryShape::setShapePrecision(uint32_t precision)
{
    _shapePrecision = precision;
}

uint32_t GeometryShape::getShapePrecision()
{
    return _shapePrecision;
}

glm::vec3* GeometryShape::getVertices(VertexType type)
{
    glm::vec3* vertices = nullptr;
    if (type == ShapeVertex) {
        vertices = _shapeVertices;
    }
    else if (type == BorderVertex) {
        vertices = _borderVertices;
    }
    return vertices;
}

uint32_t GeometryShape::getVertexCount(VertexType type)
{
    if (type == ShapeVertex) {
        return _shapeVertexCount;
    }
    else if (type == BorderVertex) {
        return _borderVertexCount;
    }
    return 0;
}

uint32_t GeometryShape::getVertexBufferSize(VertexType type)
{
    if (type == ShapeVertex) {
        return _shapeVertexBufferSize;
    }
    else if (type == BorderVertex) {
        return _borderVertexBufferSize;
    }
    return 0;
}

void GeometryShape::createVertices()
{
    if (_shapeVertices) {
        delete [] _shapeVertices;
        _shapeVertices = nullptr;
    }

    if (_borderVertices) {
        delete [] _borderVertices;
        _borderVertices = nullptr;
    }
}

RoundedRect::RoundedRect():
    _leftTopRadius(std::numeric_limits<float>::min()),
    _rightTopRadius(std::numeric_limits<float>::min()),
    _leftBottomRadius(std::numeric_limits<float>::min()),
    _rightBottomRadius(std::numeric_limits<float>::min())
{

}

RoundedRect::RoundedRect(float x, float y, float width, float height,
                         float leftTopRadius, float rightTopRadius,
                         float leftBottomRadius, float rightBottomRadius)
    :GeometryShape(x, y, width, height)
{
    float maxCornerRadius = std::min(_width, _height)/2.0;

    if (leftTopRadius < 0 || std::fabs(leftTopRadius) < FLT_EPSILON) {
        _leftTopRadius = std::numeric_limits<float>::min();
    }
    else {
        if (leftTopRadius > maxCornerRadius) {
            _leftTopRadius = maxCornerRadius;
        }
        else {
            _leftTopRadius = leftTopRadius;
        }
    }

    if (rightTopRadius < 0 || std::fabs(rightTopRadius) < FLT_EPSILON) {
        _rightTopRadius = std::numeric_limits<float>::min();
    }
    else {
        if (rightTopRadius > maxCornerRadius) {
            _rightTopRadius = maxCornerRadius;
        }
        else {
            _rightTopRadius = rightTopRadius;
        }
    }

    if (leftBottomRadius < 0 || std::fabs(leftBottomRadius) < FLT_EPSILON) {
        _leftBottomRadius = std::numeric_limits<float>::min();
    }
    else {
        if (leftBottomRadius > maxCornerRadius) {
            _leftBottomRadius = maxCornerRadius;
        }
        else {
            _leftBottomRadius = leftBottomRadius;
        }
    }

    if (rightBottomRadius < 0 || std::fabs(rightBottomRadius) < FLT_EPSILON) {
        _rightBottomRadius = std::numeric_limits<float>::min();
    }
    else {
        if (rightBottomRadius > maxCornerRadius) {
            _rightBottomRadius = maxCornerRadius;
        }
        else {
            _rightBottomRadius = rightBottomRadius;
        }
    }

}

RoundedRect::~ RoundedRect()
{

}

void RoundedRect::createVertices()
{
    GeometryShape::createVertices();
    createBorderVierices();
    createShapeVierices();

}

void RoundedRect::createBorderVierices()
{
    std::vector<glm::vec3> l0;
    std::vector<glm::vec3> l1;

    if (_borderStyle == SolidLineBorder) {
        _borderVertexCount = _shapePrecision * 6;
        _borderVertexBufferSize = sizeof(glm::vec3) * _borderVertexCount;
        _borderVertices = new glm::vec3[_borderVertexCount];
        memset(_borderVertices, 0, _borderVertexBufferSize);

        // separate 8 parts:
        // part0: left top corner
        // part1: top line
        // part2: right top corner
        // part3: right line
        // part4: bottom right corner
        // part5: bottom line
        // part6: bottom left corner
        // part7: left line
        int parts = 8;
        int steps = _shapePrecision / parts;

        // part0: left top corner
        float leftTopCrnerRadianPerStep = (M_PI / 2.0) / float(steps);

        for (int i = 0; i < steps; i ++) {
            float theta = i * leftTopCrnerRadianPerStep;
            glm::vec3 p0 = glm::vec3(_rightTopRadius * cosf(theta),
                                     _rightTopRadius * sinf(theta),
                                     0.0);
            glm::vec3 p1 = glm::vec3(_rightTopRadius * cosf(theta + leftTopCrnerRadianPerStep),
                                     _rightTopRadius * sinf(theta + leftTopCrnerRadianPerStep),
                                     0.0);

            p0.x += ((_width / 2.0) - _rightTopRadius);
            p0.y += ((_height / 2.0) - _rightTopRadius);
            p1.x += ((_width / 2.0) - _rightTopRadius);
            p1.y += ((_height / 2.0) - _rightTopRadius);

            glm::vec3 tangent = normalize((p1 - p0));
            glm::vec3 normal = glm::vec3(-tangent.y, tangent.x, 0);

            glm::vec3 cross0 = p0 + (normal * (float)(_borderWidth / 2.0));
            glm::vec3 cross1 = p0 - (normal * (float)(_borderWidth / 2.0));

            l0.push_back(cross0);
            l1.push_back(cross1);
        }

        for (int i = 0; i < steps; i ++) {
            float theta = (M_PI / 2.0) + i * leftTopCrnerRadianPerStep;
            glm::vec3 p0 = glm::vec3(_leftTopRadius * cosf(theta),
                                     _leftTopRadius * sinf(theta),
                                     0.0);
            glm::vec3 p1 = glm::vec3(_leftTopRadius * cosf(theta + leftTopCrnerRadianPerStep),
                                     _leftTopRadius * sinf(theta + leftTopCrnerRadianPerStep),
                                     0.0);

            p0.x -= ((_width / 2.0) - _leftTopRadius);
            p0.y += ((_height / 2.0) - _leftTopRadius);
            p1.x -= ((_width / 2.0) - _leftTopRadius);
            p1.y += ((_height / 2.0) - _leftTopRadius);

            glm::vec3 tangent = normalize((p1 - p0));
            glm::vec3 normal = glm::vec3(-tangent.y, tangent.x, 0);

            glm::vec3 cross0 = p0 + (normal * (float)(_borderWidth / 2.0));
            glm::vec3 cross1 = p0 - (normal * (float)(_borderWidth / 2.0));

            l0.push_back(cross0);
            l1.push_back(cross1);
        }

        for (int i = 0; i < steps; i ++) {
            float theta = M_PI + i * leftTopCrnerRadianPerStep;
            glm::vec3 p0 = glm::vec3(_leftBottomRadius * cosf(theta),
                                     _leftBottomRadius * sinf(theta),
                                     0.0);
            glm::vec3 p1 = glm::vec3(_leftBottomRadius * cosf(theta + leftTopCrnerRadianPerStep),
                                     _leftBottomRadius * sinf(theta + leftTopCrnerRadianPerStep),
                                     0.0);

            p0.x -= ((_width / 2.0) - _leftBottomRadius);
            p0.y -= ((_height / 2.0) - _leftBottomRadius);
            p1.x -= ((_width / 2.0) - _leftBottomRadius);
            p1.y -= ((_height / 2.0) - _leftBottomRadius);

            glm::vec3 tangent = normalize((p1 - p0));
            glm::vec3 normal = glm::vec3(-tangent.y, tangent.x, 0);

            glm::vec3 cross0 = p0 + (normal * (float)(_borderWidth / 2.0));
            glm::vec3 cross1 = p0 - (normal * (float)(_borderWidth / 2.0));

            l0.push_back(cross0);
            l1.push_back(cross1);
        }

        for (int i = 0; i < steps; i ++) {
            float theta = ((M_PI / 2.0) * 3.0) + i * leftTopCrnerRadianPerStep;
            glm::vec3 p0 = glm::vec3(_rightBottomRadius * cosf(theta),
                                     _rightBottomRadius * sinf(theta),
                                     0.0);
            glm::vec3 p1 = glm::vec3(_rightBottomRadius * cosf(theta + leftTopCrnerRadianPerStep),
                                     _rightBottomRadius * sinf(theta + leftTopCrnerRadianPerStep),
                                     0.0);

            p0.x += ((_width / 2.0) - _rightBottomRadius);
            p0.y -= ((_height / 2.0) - _rightBottomRadius);
            p1.x += ((_width / 2.0) - _rightBottomRadius);
            p1.y -= ((_height / 2.0) - _rightBottomRadius);

            glm::vec3 tangent = normalize((p1 - p0));
            glm::vec3 normal = glm::vec3(-tangent.y, tangent.x, 0);

            glm::vec3 cross0 = p0 + (normal * (float)(_borderWidth / 2.0));
            glm::vec3 cross1 = p0 - (normal * (float)(_borderWidth / 2.0));

            l0.push_back(cross0);
            l1.push_back(cross1);
        }


        // // part1: top line
        // float topLineLengthPerStep = (_width - _leftTopRadius - _rightTopRadius) / float(steps);
        // float x = _x - (_width/2.0) + _leftTopRadius;
        // float y = _y + (_height/2.0);
        // for (int i = 0; i < steps; i ++) {
        //     glm::vec3 p0 = glm::vec3(x+i*topLineLengthPerStep, y, 0.0);
        //     glm::vec3 cross0 = glm::vec3(p0.x, p0.y + (float)(_borderWidth / 2.0), 0.0);
        //     glm::vec3 cross1 = glm::vec3(p0.x, p0.y - (float)(_borderWidth / 2.0), 0.0);

        //     l0.push_back(cross0);
        //     l1.push_back(cross1);
        // }


        for (int i = 0; i < steps*4; i ++) {
            _borderVertices[i*6 + 0] = l0[i];
            _borderVertices[i*6 + 1] = l1[i];
            _borderVertices[i*6 + 2] = l0[i+1];
            _borderVertices[i*6 + 3] = l0[i+1];
            _borderVertices[i*6 + 4] = l1[i];
            _borderVertices[i*6 + 5] = l1[i+1];
        }

    }
    else if (_borderStyle == DashLineBorder) {
    }
    else if (_borderStyle == DotLineBorder) {
    }
    else if (_borderStyle == DashDotLineBorder) {
    }
    else if (_borderStyle == DashDotLineBorder) {
    }
}

void RoundedRect::createShapeVierices()
{

}


Oval::Oval()
{
}

Oval::Oval(float x, float y, float width, float height)
    :GeometryShape(x, y, width, height)
{

}

Oval:: ~ Oval()
{

}


void Oval::createVertices()
{
    GeometryShape::createVertices();

    // _vertexCount = (_segments + 1) * 2;
    // _vertexBufferSize = sizeof(float) * _vertexCount * 2;

    // _vertexBuffer = new float[_vertexCount * 2];

    // for (int i = 0; i <= _segments; i ++) {
    //     float theta = 2.0f * 3.1415926f * float(i) / float(_segments);
    //     float delta = 1.0f / _segments;

    //     glm::vec2 currentPoint = glm::vec2(_radius * cosf(theta),
    //                                        _radius * sinf(theta));
    //     glm::vec2 nextPoint = glm::vec2(_radius * cosf(theta+delta),
    //                                     _radius * sinf(theta+delta));

    //     glm::vec2 tangent = normalize((nextPoint - currentPoint));
    //     glm::vec2 normal = glm::vec2(-tangent.y, tangent.x);
    //     glm::vec2 left = currentPoint + (normal * (float)(_borderWidth / 2.0));
    //     glm::vec2 right = currentPoint - (normal * (float)(_borderWidth / 2.0));

    //     _vertexBuffer[i * 2 * 2 + 0] = left.x;
    //     _vertexBuffer[i * 2 * 2 + 1] = left.y;
    //     _vertexBuffer[i * 2 * 2 + 2] = right.x;
    //     _vertexBuffer[i * 2 * 2 + 3] = right.y;
    // }
}

Bezier::Bezier()
{
}

Bezier::Bezier(std::initializer_list<glm::vec2> list)
{
    // for (auto elem : list) {
    //     _points.push_back(elem);
    // }
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

void Bezier::createVertices()
{
    GeometryShape::createVertices();

    // _vertexCount = _segments * 2;
    // _vertexBufferSize = sizeof(float) * _vertexCount * 2;

    // _vertexBuffer = new float[_vertexCount * 2];

    // for (int i = 0; i < _segments; i ++) {

    //     float t = i / (float)_segments;
    //     float delta = 1.0f / _segments;

    //     glm::vec2 currentPoint = bezier(t,
    //                                     _points[0],
    //                                     _points[1],
    //                                     _points[2],
    //                                     _points[3]);
    //     glm::vec2 nextPoint = bezier(std::min(t + delta, 1.0f),
    //                                  _points[0],
    //                                  _points[1],
    //                                  _points[2],
    //                                  _points[3]);

    //     glm::vec2 tangent = glm::normalize((nextPoint - currentPoint));
    //     glm::vec2 normal = glm::vec2(-tangent.y, tangent.x);
    //     glm::vec2 left = currentPoint + (normal * (float)(_width / 2.0));
    //     glm::vec2 right = currentPoint - (normal * (float)(_width / 2.0));

    //     _vertexBuffer[i * 2 * 2 + 0] = left.x;
    //     _vertexBuffer[i * 2 * 2 + 1] = left.y;
    //     _vertexBuffer[i * 2 * 2 + 2] = right.x;
    //     _vertexBuffer[i * 2 * 2 + 3] = right.y;
    // }
}


LinearGradient::LinearGradient()
{

}

LinearGradient::LinearGradient(std::initializer_list<glm::vec3> colors)
{

}

LinearGradient::~ LinearGradient()
{

}


RadialGradient::RadialGradient()
{

}

RadialGradient::RadialGradient(std::initializer_list<glm::vec3> colors)
{

}

RadialGradient::~ RadialGradient()
{

}

}
