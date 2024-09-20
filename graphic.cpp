#include "graphic.h"
#include <cmath>
#include <cstring>
#include <iostream>

namespace Anymate {


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

GeometryShape::GeometryShape()
    :
    Graphic(0.0, 0.0, 0.0, 0.0),
    _borderWidth(1.0),
    _borderStyle(SolidLineBorder),
    _borderColor(glm::vec4(0.0, 0.0, 0.0, 1.0)),
    _fillStyle(SolidFill),
    _fillSolidColor(glm::vec4(1.0, 1.0, 1.0, 1.0))
{

}

GeometryShape::GeometryShape(float x, float y, float width, float height)
    :
    Graphic(x, y, width, height),
    _borderWidth(1.0),
    _borderStyle(SolidLineBorder),
    _borderColor(glm::vec4(0.0, 0.0, 0.0, 1.0)),
    _fillStyle(SolidFill),
    _fillSolidColor(glm::vec4(1.0, 1.0, 1.0, 1.0))
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


bool GeometryShape::isCuspIndex(int i)
{
    return std::find(_cuspIndexes.begin(), _cuspIndexes.end(), i)
           != _cuspIndexes.end();
}

glm::vec3 GeometryShape::orthogonal(glm::vec3& p0, glm::vec3& p1)
{
    glm::vec3 tangent = glm::normalize((p1 - p0));
    return glm::vec3(-tangent.y, tangent.x, 0);
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

    createOutlinePoints();
    createBorderVertices();
    createShapeVertices();
}

void GeometryShape::createOutlinePoints()
{
    _outlinePoints.clear();
    _cuspIndexes.clear();
}

void GeometryShape::createBorderVertices()
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

        l0.push_back(glm::vec3(c0.x, c0.y, borderVerticesZWeight));
        l1.push_back(glm::vec3(c1.x, c1.y, borderVerticesZWeight));
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

void GeometryShape::createShapeVertices()
{
    if (!_outlinePoints.size()) {
        return ;
    }

    _shapeVertices.clear();

    int n = _outlinePoints.size();

    for (int i = 0; i < n; i ++) {
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

        _shapeVertices.push_back(glm::vec3(0.0, 0.0, shapeVerticesZWeight));
        _shapeVertices.push_back(glm::vec3(p0.x, p0.y, shapeVerticesZWeight));
        _shapeVertices.push_back(glm::vec3(p1.x, p1.y, shapeVerticesZWeight));
    }
}

RoundedRect::RoundedRect():
    GeometryShape(0.0, 0.0, 0.0, 0.0),
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
        _leftTopRadius = minCornerRadius;
    }
    else if (_leftTopRadius > maxCornerRadius) {
        _leftTopRadius = maxCornerRadius;
    }

    if (_rightTopRadius < 0 || std::fabs(_rightTopRadius) < FLT_EPSILON) {
        _rightTopRadius = minCornerRadius;
    }
    else if (_rightTopRadius > maxCornerRadius) {
        _rightTopRadius = maxCornerRadius;
    }

    if (_rightBottomRadius < 0 || std::fabs(_rightBottomRadius) < FLT_EPSILON) {
        _rightBottomRadius = minCornerRadius;
    }
    else if (_rightBottomRadius > maxCornerRadius) {
        _rightBottomRadius = maxCornerRadius;
    }

    if (_leftBottomRadius < 0 || std::fabs(_leftBottomRadius) < FLT_EPSILON) {
        _leftBottomRadius = minCornerRadius;
    }
    else if (_leftBottomRadius > maxCornerRadius) {
        _leftBottomRadius = maxCornerRadius;
    }
}

RoundedRect::~ RoundedRect()
{

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

Oval::Oval()
    :
    GeometryShape(0.0, 0.0, 0.0, 0.0)
{

}

Oval::Oval(float x, float y, float width, float height)
    :
    GeometryShape(x, y, width, height)
{
}

Oval:: ~ Oval()
{

}

void Oval::createOutlinePoints()
{
    _outlinePoints.clear();
    _cuspIndexes.clear();

    float a = _width/2.0;
    float b = _height/2.0;
    int precision = 360;
    float radianUnit = (2.0 * M_PI)/float(precision);

    for (int i = 0; i < precision; i ++) {
        float theta = i * radianUnit;
        glm::vec3 p = glm::vec3(a * cosf(theta), b * sinf(theta), 0.0);
        _outlinePoints.push_back(p);
    }
}

Rect::Rect()
    :
    GeometryShape(0.0, 0.0, 0.0, 0.0)
{

}

Rect::Rect(float x, float y, float width, float height)
    :
    GeometryShape(x, y, width, height)
{
}

Rect:: ~ Rect()
{

}

void Rect::createOutlinePoints()
{
    _outlinePoints.clear();
    _cuspIndexes.clear();

    int c = 0;
    float x = 0.0;
    float y = 0.0;
    int roundWidth = std::ceil(_width);
    int roundHeight= std::ceil(_height);
    double intpart;
    float tailWidth = std::modf(_width, &intpart);
    float tailHeight = std::modf(_height, &intpart);

    // top line
    x = -_width/2.0;
    y = _height/2.0;
    _cuspIndexes.push_back(c);
    for (int i = 0; i < roundWidth; i ++) {
        _outlinePoints.push_back(glm::vec3(x, y, 0.0));

        if (i == roundWidth - 1){
            x += tailWidth;
        }
        else {
            x += 1.0;
        }
        c ++;
    }

    // right line
    x = _width/2.0;
    y = _height/2.0;
    _cuspIndexes.push_back(c);
    for (int i = 0; i < roundHeight; i ++) {
        _outlinePoints.push_back(glm::vec3(x, y, 0.0));

        if (i == roundHeight - 1){
            y -= tailHeight;
        }
        else {
            y -= 1.0;
        }
        c ++;
    }

    // bottom line
    x = _width/2.0;
    y = -_height/2.0;
    _cuspIndexes.push_back(c);
    for (int i = 0; i < roundWidth; i ++) {
        _outlinePoints.push_back(glm::vec3(x, y, 0.0));

        if (i == roundWidth - 1){
            x -= tailWidth;
        }
        else {
            x -= 1.0;
        }

        c ++;
    }

    // left line
    x = -_width/2.0;
    y = -_height/2.0;
    _cuspIndexes.push_back(c);
    for (int i = 0; i < roundHeight; i ++) {
        _outlinePoints.push_back(glm::vec3(x, y, 0.0));
        if (i == roundHeight - 1){
            y += tailHeight;
        }
        else {
            y += 1.0;
        }
        c ++;
    }

}

void Rect::createBorderVertices()
{
    if (!_outlinePoints.size()) {
        return ;
    }

    _borderVertices.clear();

    std::vector<glm::vec3> l0;
    std::vector<glm::vec3> l1;

    int n = _outlinePoints.size();

    for (int i = 0; i < n; i ++) {
        glm::vec3 p0;
        glm::vec3 p1;
        glm::vec3 normal;
        glm::vec3 c0;
        glm::vec3 c1;

        if (isCuspIndex(i)) {
            if (i == 0) {
                p0 = _outlinePoints[i];
                p1 = _outlinePoints[n-1];
            }
            else {
                p0 = _outlinePoints[i];
                p1 = _outlinePoints[i-1];
            }
            normal = orthogonal(p0, p1);
            c0 = p0 + (normal * (float)(_borderWidth / 2.0));
            c1 = p0 - (normal * (float)(_borderWidth / 2.0));
            _borderVertices.push_back(c0);
            _borderVertices.push_back(c1);
        }

        if (i == n-1) {
            p0 = _outlinePoints[i];
            p1 = _outlinePoints[i - 1];
        }
        else {
            p0 = _outlinePoints[i];
            p1 = _outlinePoints[i + 1];
        }

        normal = orthogonal(p0, p1);
        c0 = p0 + (normal * (float)(_borderWidth / 2.0));
        c1 = p0 - (normal * (float)(_borderWidth / 2.0));

        _borderVertices.push_back(c0);
        _borderVertices.push_back(c1);
    }

}

void Rect::createShapeVertices()
{
    _shapeVertices.clear();
    float hw = _width/2.0;
    float hh = _height/2.0;
    _shapeVertices.push_back(glm::vec3(-hw, -hh, shapeVerticesZWeight));
    _shapeVertices.push_back(glm::vec3(hw, -hh, shapeVerticesZWeight));
    _shapeVertices.push_back(glm::vec3(-hw, hh, shapeVerticesZWeight));
    _shapeVertices.push_back(glm::vec3(-hw, hh, shapeVerticesZWeight));
    _shapeVertices.push_back(glm::vec3(hw, -hh, shapeVerticesZWeight));
    _shapeVertices.push_back(glm::vec3(hw, hh, shapeVerticesZWeight));
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

void Bezier::createOutlinePoints()
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


Polygon::Polygon()
{

}

Polygon::Polygon(std::initializer_list<glm::vec3> list)
{
    if (!list.size())
        return;

    for (glm::vec3 p : list) {
        _points.push_back(p);
    }
}

Polygon:: ~ Polygon()
{

}


glm::vec3 Polygon::getCenter()
{
    auto minXIt = std::min_element(_points.begin(), _points.end(),
                                   [](const glm::vec3& a, const glm::vec3& b) {
                                       return a.x < b.x;
                                   });
    auto maxXIt = std::min_element(_points.begin(), _points.end(),
                                   [](const glm::vec3& a, const glm::vec3& b) {
                                       return a.x > b.x;
                                   });

    auto minYIt = std::min_element(_points.begin(), _points.end(),
                                   [](const glm::vec3& a, const glm::vec3& b) {
                                       return a.y < b.y;
                                   });
    auto maxYIt = std::min_element(_points.begin(), _points.end(),
                                   [](const glm::vec3& a, const glm::vec3& b) {
                                       return a.y > b.y;
                                   });
    float x = (minXIt->x + maxXIt->x) / 2.0;
    float y = (minYIt->y + maxYIt->y) / 2.0;
    float z = shapeVerticesZWeight;

    return glm::vec3(x, y, z);

}

void Polygon::createOutlinePoints()
{
    _outlinePoints.clear();
}

void Polygon::createBorderVertices()
{
    _borderVertices.clear();
}

void Polygon::createShapeVertices()
{
    _shapeVertices.clear();
    if (_points.size() < 3) {
        return;
    }

    glm::vec3 center = getCenter();

    for (int i = 0; i < _points.size(); i ++) {
        int j = 0;
        int k = 0;

        j = i;
        if (i == _points.size() - 1) {
            k = 0;
        }
        else {
            k = i+1;
        }

        _shapeVertices.push_back(_points[j]);
        _shapeVertices.push_back(_points[k]);
        _shapeVertices.push_back(center);
    }


}

}
