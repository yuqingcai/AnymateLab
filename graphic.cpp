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

    std::vector<glm::vec2> vertices;

    vertices = _vertexGenerator.createOutlineFrames(_pen, _outline);
    _outlineVertices.clear();
    for (glm::vec2 p : vertices) {
        _outlineVertices.push_back(glm::vec3(p.x, p.y, borderVerticesZWeight));
    }

    vertices = _vertexGenerator.createOutlineGuideLines(_pen, _outline);
    _guideLineVertices.clear();
    for (glm::vec2 p : vertices) {
        _guideLineVertices.push_back(glm::vec3(p.x, p.y, borderGuideLineVerticesZWeight));
    }
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
    std::vector<glm::vec2> points2D;
    for (glm::vec3 p : _points) {
        points2D.push_back(glm::vec2(p.x, p.y));
    }

    float tail = 0.0;
    float step = 0.0;
    for (int i = 0; i < n-1; i ++) {

        int j = i + 1;

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

glm::vec3 Polygon::center()
{
    float x = 0.0, y = 0.0, z = 0.0;
    int n = _points.size();
    for (int i = 0; i < n; i ++) {
        x += _points[i].x;
        y += _points[i].y;
        z += _points[i].z;
    }
    return glm::vec3(x/n, y/n, z/n);
}

void Polygon::createShapeVertices()
{
    _shapeVertices.clear();
    if (_points.size() < 3) {
        return;
    }
}

glm::vec3 Polygon::normal()
{
    assert(_points.size() >= 3);

    if (_points.size() < 3) {
        fprintf(stderr, "Polygon should at least contains 3 points\n");
    }

    int n = _points.size();
    glm::vec3 p0 = _points[0];
    glm::vec3 p1 = _points[1];
    float epsilon = 1e-6f;
    glm::vec3 p2;
    bool found = false;

    // find the point that are not collinear
    int i = 2;
    while (i < n) {
        p2 = _points[i];
        glm::vec3 AB = p1 - p0;
        glm::vec3 AC = p2 - p0;
        if (glm::length(glm::cross(AB, AC)) > epsilon) {
            found = true;
            break;
        }
        i ++;
    }

    if (found) {
        return glm::normalize(glm::cross(p1 - p0, p2 - p0));
    }

    fprintf(stderr, "Polygon should at least contains 3 points that are not collinear\n");
    return glm::vec3(0.0, 0.0, 0.0);
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
    glm::vec2 p0 = glm::vec2(_endPoint0.x, _endPoint0.y);
    glm::vec2 p1 = glm::vec2(_endPoint1.x, _endPoint1.y);
    float distance = glm::distance(p0, p1);
    glm::vec2 p = p0;

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
