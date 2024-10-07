#include "graphic.h"
#include "tools.h"
#include <cmath>
#include <cstring>
#include <algorithm>

namespace Vangoh {

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
    Graphic(0.0, 0.0, 0.0, 0.0)
{

}

GeometryShape::GeometryShape(float x, float y, float width, float height)
    :
    Graphic(x, y, width, height)
{

}

GeometryShape::~ GeometryShape()
{
    _shapeVertices.clear();
    _borderVertices.clear();
}

std::vector<glm::vec3>& GeometryShape::getBorderVertices()
{
    return _borderVertices;
}

std::vector<glm::vec3>& GeometryShape::getBorderGuideLineVertices()
{
    return _borderGuideLineVertices;
}

std::vector<glm::vec3>& GeometryShape::getShapeVertices()
{
    return _shapeVertices;
}

void GeometryShape::setPen(Pen& pen)
{
    _pen = pen;
}

void GeometryShape::draw()
{
    createBorderVertices();
    createShapeVertices();
}

void GeometryShape::createBorderVertices()
{
    createOutline();
    _outline.print();

    std::vector<glm::vec2> vertices;

    vertices = _vertexGenerator.generate(_pen, _outline);
    _borderVertices.clear();
    for (glm::vec2 p : vertices) {
        _borderVertices.push_back(glm::vec3(p.x, p.y, borderVerticesZWeight));
    }

    vertices = _vertexGenerator.guideLines(_pen, _outline);
    _borderGuideLineVertices.clear();

    for (glm::vec2 p : vertices) {
        _borderGuideLineVertices.push_back(
            glm::vec3(p.x, p.y, borderGuideLineVerticesZWeight));
    }
}

void GeometryShape::createShapeVertices()
{

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

Polygon::Polygon(std::vector<glm::vec3> list)
{
    if (!list.size())
        return;

    for (glm::vec3 p : list) {
        _points.push_back(p);
    }
}


Polygon::~ Polygon()
{

}


void Polygon::createOutline()
{
    _outline.reset();

    int n = _points.size();
    if (n < 2) {
        return;
    }

    // outline just need 2D points;
    std::vector<glm::vec2> points2D;
    for (glm::vec3 p : _points) {
        points2D.push_back(glm::vec2(p.x, p.y));
    }

    float tail = 0.0;
    float step = 0.0;

    for (int i = 0; i < n; i ++) {

        int j = i + 1;

        if (i == n-1) {
            j = 0;
        }

        glm::vec2& p0 = points2D[i];
        glm::vec2& p1 = points2D[j];
        float distance = glm::distance(p0, p1);
        glm::vec2 p;

        if (!_outline.getPoints().size() ||
            _outline.getPoints().back().getPosition() != p0){
            _outline.appendPosition(p0);
        }

        if (tail <= FLT_EPSILON) {
            p = p0;
        }
        else {
            if (fabs(tail - distance) < FLT_EPSILON) {
                _outline.appendPosition(p1);
                _outline.getPoints().back().setCuspPoint(true);
                tail = 0.0;
                // next segment
                continue;
            }
            else if (tail < distance) {
                p = pointRelateTo(p0, p1, tail);
                _outline.appendPosition(p);
            }
            else if (tail > distance){
                _outline.appendPosition(p1);
                _outline.getPoints().back().setCuspPoint(true);
                _outline.getPoints().back().setBreakPoint(true);
                tail -= distance;
                // next segment
                continue;
            }
        }

        step = Vangoh::outlinePrecision;
        while (true) {

            p = pointRelateTo(p, p1, step);
            float distanceP = glm::distance(p0, p);

            if (distanceP < distance) {
                _outline.appendPosition(p);
            }
            else if (fabs(distanceP - distance) < FLT_EPSILON) {
                _outline.appendPosition(p1);
                _outline.getPoints().back().setCuspPoint(true);
                tail = 0.0;
                break;
            }
            else if (distanceP > distance){
                _outline.appendPosition(p1);
                _outline.getPoints().back().setCuspPoint(true);
                _outline.getPoints().back().setBreakPoint(true);
                tail = distanceP - distance;
                break;
            }
        }
    }

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
