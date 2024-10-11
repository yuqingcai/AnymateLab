#include "graphic.h"
#include "tools.h"
#include <cmath>
#include <cstring>
#include <algorithm>
#include <cassert>
#include <glm/gtc/matrix_transform.hpp>

namespace Vangoh {

Graphic::Graphic()
    :
    _x(0.0),
    _y(0.0),
    _z(0.0)
{
}

Graphic::Graphic(float x, float y, float z)
    :
    _x(0.0),
    _y(0.0),
    _z(0.0)
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

float Graphic::getX()
{
    return _x;
}

float Graphic::getY()
{
    return _y;
}

float Graphic::getZ()
{
    return _z;
}


Shape::Shape()
    :
    Graphic(0.0, 0.0, 0.00)
{

}

Shape::Shape(float x, float y, float z)
    :
    Graphic(x, y, z)
{

}

Shape::~ Shape()
{
    _outlineVertices.clear();
    _guideLineVertices.clear();
    _shapeVertices.clear();
}

std::vector<glm::vec3>& Shape::getOutlineVertices()
{
    return _outlineVertices;
}

std::vector<glm::vec3>& Shape::getGuideLineVertices()
{
    return _guideLineVertices;
}

std::vector<glm::vec3>& Shape::getShapeVertices()
{
    return _shapeVertices;
}

void Shape::setPen(Pen& pen)
{
    _pen = pen;
}

void Shape::draw()
{
    createOutlineVertices();
    createShapeVertices();
}

void Shape::createOutlineVertices()
{
    createOutline();
    // _outline.print();

    std::vector<glm::vec3> vertices;

    vertices = _vertexGenerator.createOutlineMeshes(_pen, _outline);
    _outlineVertices.clear();
    _outlineVertices.insert(_outlineVertices.end(), vertices.begin(), vertices.end());

    vertices = _vertexGenerator.createOutlineGuideLines(_pen, _outline);
    _guideLineVertices.clear();
    _guideLineVertices.insert(_guideLineVertices.end(), vertices.begin(), vertices.end());

}

void Shape::createShapeVertices()
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

    // make sure polygon is closed
    if (_points.back() != _points.front()) {
        _points.push_back(_points.front());
    }
}

Polygon::Polygon(std::vector<glm::vec3> list)
{
    if (!list.size())
        return;

    for (glm::vec3 p : list) {
        _points.push_back(p);
    }

    // make sure polygon is closed
    if (_points.back() != _points.front()) {
        _points.push_back(_points.front());
    }
}

Polygon::~ Polygon()
{

}

void Polygon::createOutline()
{
    assert(_points.size() >= 3);
    int n = _points.size();
    if (n < 3) {
        fprintf(stderr, "Polygon should at least contains 3 points\n");
        return;
    }

    _outline.reset();

    // outline just need 2D points;

    float tail = 0.0;
    float step = 0.0;
    for (int i = 0; i < n-1; i ++) {

        int j = i + 1;

        glm::vec3& p0 = _points[i];
        glm::vec3& p1 = _points[j];
        float distance = glm::distance(p0, p1);
        glm::vec3 p;

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


void Polygon::createShapeVertices()
{
    _shapeVertices.clear();
    if (_points.size() < 3) {
        return;
    }
}



Line::Line(glm::vec3 p0, glm::vec3 p1)
    :
    _endPoint0(p0),
    _endPoint1(p1)
{

}

Line::~Line()
{

}

void Line::createOutline()
{
    _outline.reset();

    float step = Vangoh::outlinePrecision;

    // outline just need 2D points;
    glm::vec3 p0 = _endPoint0;
    glm::vec3 p1 = _endPoint1;
    float distance = glm::distance(p0, p1);
    glm::vec3 p = p0;

    _outline.appendPosition(p0);

    while (true) {
        p = pointRelateTo(p, p1, step);
        float distanceP = glm::distance(p0, p);
        if (distanceP < distance) {
            _outline.appendPosition(p);
        }
        else if (fabs(distanceP - distance) < FLT_EPSILON ||
                distanceP > distance) {
            _outline.appendPosition(p1);
            break;
        }
    }

}


}
