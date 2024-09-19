#include "graphic.h"
#include <cmath>
#include <cstring>
#include <iostream>

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
    _shapePrecision(GeometryShape::defaultShapePrecision)
{
}

GeometryShape::GeometryShape(float x, float y, float width, float height)
    :Graphic(x, y, width, height),
    _borderWidth(1.0),
    _borderStyle(SolidLineBorder),
    _borderColor(glm::vec4(0.0, 0.0, 0.0, 1.0)),
    _fillStyle(SolidFill),
    _fillSolidColor(glm::vec4(1.0, 1.0, 1.0, 1.0)),
    _shapePrecision(GeometryShape::defaultShapePrecision)
{

}

GeometryShape::~ GeometryShape()
{
    _shapeVertices.clear();
    _borderVertices.clear();
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

std::vector<glm::vec3>& GeometryShape::getVertices(VertexType type)
{
    if (type == ShapeVertex) {
        return _shapeVertices;
    }
    else if (type == BorderVertex) {
        return _borderVertices;
    }

    return _errorVertices;
}


void GeometryShape::createVertices()
{
    _borderVertices.clear();
    _shapeVertices.clear();
}

void GeometryShape::createOutlinePoints()
{
    _outlinePoints.clear();
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
                         float rightBottomRadius, float leftBottomRadius)
    :GeometryShape(x, y, width, height)
{
    float maxCornerRadius = std::min(_width, _height)/2.0;

    _leftTopRadius = leftTopRadius;
    _rightTopRadius = rightTopRadius;
    _rightBottomRadius = rightBottomRadius;
    _leftBottomRadius = leftBottomRadius;

    if (_leftTopRadius < 0 || std::fabs(_leftTopRadius) < FLT_EPSILON) {
        _leftTopRadius = std::numeric_limits<float>::min();
    }
    else if (_leftTopRadius > maxCornerRadius) {
        _leftTopRadius = maxCornerRadius;
    }

    if (_rightTopRadius < 0 || std::fabs(_rightTopRadius) < FLT_EPSILON) {
        _rightTopRadius = std::numeric_limits<float>::min();
    }
    else if (_rightTopRadius > maxCornerRadius) {
        _rightTopRadius = maxCornerRadius;
    }

    if (_rightBottomRadius < 0 || std::fabs(_rightBottomRadius) < FLT_EPSILON) {
        _rightBottomRadius = std::numeric_limits<float>::min();
    }
    else if (_rightBottomRadius > maxCornerRadius) {
        _rightBottomRadius = maxCornerRadius;
    }

    if (_leftBottomRadius < 0 || std::fabs(_leftBottomRadius) < FLT_EPSILON) {
        _leftBottomRadius = std::numeric_limits<float>::min();
    }
    else if (_leftBottomRadius > maxCornerRadius) {
        _leftBottomRadius = maxCornerRadius;
    }
}

RoundedRect::~ RoundedRect()
{

}

void RoundedRect::createVertices()
{
    GeometryShape::createVertices();
    createOutlinePoints();
    createBorderVierices();
    createShapeVierices();
}

void RoundedRect::createOutlinePoints()
{
    GeometryShape::createOutlinePoints();

    float topLineLength = _width - _rightTopRadius - _leftTopRadius;
    float leftLineLength = _height - _leftTopRadius - _leftBottomRadius;
    float bottomLineLength = _width - _leftBottomRadius - _rightBottomRadius;
    float rightLineLength = _height - _rightTopRadius - _rightBottomRadius;

    // separate 8 parts:
    // part0: right top corner
    // part1: top line
    // part2: left top corner
    // part3: left line
    // part4: bottom left corner
    // part5: bottom line
    // part6: bottom right corner
    // part7: right line
    int cornerPrecision = 45;
    float radianUnit = (M_PI/2.0)/float(cornerPrecision);
    float theta = 0.0;
    float x = 0.0;
    float y = 0.0;
    glm::vec3 p;
    glm::vec3 offset;
    float hw = _width / 2.0;
    float hh = _height / 2.0;

    // right top corner
    for (int i = 0; i < cornerPrecision; i ++) {
        theta = i * radianUnit;
        p = glm::vec3(_rightTopRadius * cosf(theta),
                      _rightTopRadius * sinf(theta),
                      0.0);
        offset = glm::vec3(hw - _rightTopRadius,
                           hh - _rightTopRadius,
                           0.0);
        p.x += offset.x;
        p.y += offset.y;
        _outlinePoints.push_back(p);
    }

    // top line
    x = hw - _rightTopRadius;
    y = hh;
    for (int i = 0; i < int(topLineLength); i ++) {
        p = glm::vec3(x - i, y, 0.0);
        _outlinePoints.push_back(p);
    }

    // left top corner
    for (int i = 0; i < cornerPrecision; i ++) {
        theta = (M_PI / 2.0) + (i * radianUnit);
        p = glm::vec3(_leftTopRadius * cosf(theta),
                      _leftTopRadius * sinf(theta),
                      0.0);
        offset = glm::vec3(hw - _leftTopRadius,
                           hh - _leftTopRadius,
                           0.0);
        p.x -= offset.x;
        p.y += offset.y;
        _outlinePoints.push_back(p);
    }

    // left line
    x = - hw;
    y = hh - _leftTopRadius;
    for (int i = 0; i < int(leftLineLength); i ++) {
        glm::vec3 p = glm::vec3(x, y - i, 0.0);
        _outlinePoints.push_back(p);
    }

    // left bottom corner
    for (int i = 0; i < cornerPrecision; i ++) {
        theta = (M_PI) + (i * radianUnit);
        p = glm::vec3(_leftBottomRadius * cosf(theta),
                      _leftBottomRadius * sinf(theta),
                      0.0);
        offset = glm::vec3(hw - _leftBottomRadius,
                           hh - _leftBottomRadius,
                           0.0);
        p.x -= offset.x;
        p.y -= offset.y;
        _outlinePoints.push_back(p);
    }

    // bottom line
    x = - hw + _leftBottomRadius;
    y = - hh;
    for (int i = 0; i < int(bottomLineLength); i ++) {
        glm::vec3 p = glm::vec3(x + i, y, 0.0);
        _outlinePoints.push_back(p);
    }

    // right bottom corner
    for (int i = 0; i < cornerPrecision; i ++) {
        theta = (M_PI / 2.0 * 3.0) + (i * radianUnit);
        p = glm::vec3(_rightBottomRadius * cosf(theta),
                      _rightBottomRadius * sinf(theta),
                      0.0);
        offset = glm::vec3(hw - _rightBottomRadius,
                           hh - _rightBottomRadius,
                           0.0);
        p.x += offset.x;
        p.y -= offset.y;
        _outlinePoints.push_back(p);
    }

    // right line
    x = hw;
    y = - hh + _rightBottomRadius;
    for (int i = 0; i < int(rightLineLength); i ++) {
        glm::vec3 p = glm::vec3(x, y + i, 0.0);
        _outlinePoints.push_back(p);
    }

}

void RoundedRect::createBorderVierices()
{
    if (!_outlinePoints.size()) {
        return ;
    }

    std::vector<glm::vec3> l0;
    std::vector<glm::vec3> l1;

    int n = _outlinePoints.size();

    for (int i = 0; i < n; i ++) {
        glm::vec3 p0;
        glm::vec3 p1;

        if (i != n - 1) {
            p0 = _outlinePoints[i];
            p1 = _outlinePoints[i + 1];
        }
        else {
            p0 = _outlinePoints[i];
            p1 = _outlinePoints[0];
        }

        glm::vec3 tangent = glm::normalize((p1 - p0));
        glm::vec3 normal = glm::vec3(-tangent.y, tangent.x, 0);

        glm::vec3 c0 = p0 + (normal * (float)(_borderWidth / 2.0));
        glm::vec3 c1 = p0 - (normal * (float)(_borderWidth / 2.0));

        l0.push_back(c0);
        l1.push_back(c1);
    }

    // border vertices
    for (int i = 0; i < n; i ++) {
        //
        // l1[i+1] +-------+ l0[i+1]
        //         |\      |
        //         | \     |
        //         |  \    |
        //         |   \   |
        //         |    \  |
        //         |     \ |
        //         |      \|
        //   l1[i] +-------+ l0[i]
        //
        if (i != n - 1) {
            _borderVertices.push_back(l0[i]);
            _borderVertices.push_back(l0[i+1]);
            _borderVertices.push_back(l1[i+1]);
            _borderVertices.push_back(l1[i+1]);
            _borderVertices.push_back(l1[i]);
            _borderVertices.push_back(l0[i]);
        }
        else {
            _borderVertices.push_back(l0[i]);
            _borderVertices.push_back(l0[0]);
            _borderVertices.push_back(l1[0]);
            _borderVertices.push_back(l1[0]);
            _borderVertices.push_back(l1[i]);
            _borderVertices.push_back(l0[i]);
        }
    }
}

void RoundedRect::createShapeVierices()
{
    if (!_outlinePoints.size()) {
        return ;
    }

    int n = _outlinePoints.size();

    for (int i = 0; i < n; i ++) {
        _shapeVertices.push_back(glm::vec3(0.0, 0.0, 2.0));
        glm::vec3 p0;
        glm::vec3 p1;

        if (i != n-1) {
            p0 = _outlinePoints[i];
            p1 = _outlinePoints[i+1];

        }
        else {
            p0 = _outlinePoints[i];
            p1 = _outlinePoints[0];
        }
        _shapeVertices.push_back(glm::vec3(p0.x, p0.y, 0.0));
        _shapeVertices.push_back(glm::vec3(p1.x, p1.y, 0.0));
    }
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
}

Bezier::Bezier()
{
}

Bezier::Bezier(std::initializer_list<glm::vec2> list)
{
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
