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
    std::vector<glm::vec2> vertices = _vertexGenerator.generate(_pen, _outline);
    // std::vector<glm::vec2> vertices = _vertexGenerator.guideLines(_pen, _outline);

    _borderVertices.clear();

    for (glm::vec2 p : vertices) {
        _borderVertices.push_back(glm::vec3(p.x, p.y, borderVerticesZWeight));
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

    bool joinLastSegment = false;
    float tail = 0.0;

    for (int i = 0; i < n; i ++) {

        int k = i + 1;
        if (i == n-1) {
            k = 0;
        }

        glm::vec2& p0 = points2D[i];
        glm::vec2& p1 = points2D[k];
        float distance = glm::distance(p0, p1);
        float steps = 0.0;

        if (tail > FLT_EPSILON) {
            steps = tail;
            joinLastSegment = true;
        }
        else {
            steps = outlineSegmentPrecision;
            joinLastSegment = false;
        }

        glm::vec2 p0_ = p0;
        glm::vec2 p;

        while (true) {
            p = pointRelateTo(p0_, p1, steps);
            float distance_ = glm::distance(p0, p);

            if (distance_ < distance) {

                if (joinLastSegment) {
                    _outline.lastSegmentAppendAnchor(p);
                    joinLastSegment = false;
                    steps = outlineSegmentPrecision;
                }
                else {
                    _outline.appendSegment(p0_, p);
                }

                p0_ = p;
            }
            else {
                if (fabs(distance_ - distance) < FLT_EPSILON) {
                    tail = 0.0;
                }
                else if (distance_ > distance) {
                    tail = distance_ - distance;
                }

                p = p1;

                if (joinLastSegment) {
                    _outline.lastSegmentAppendAnchor(p);
                }
                else {
                    OutlineSegment segment = OutlineSegment({p0_, p});
                    segment.setContainCusp(true);
                    _outline.appendSegment(segment);
                }

                break;
            }
        }
    }

    // _outline.print();
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
