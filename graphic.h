#ifndef AMGRAPHIC_H
#define AMGRAPHIC_H

#include <vector>
#include <initializer_list>
#include <glm/glm.hpp>
#include <color.h>
#include "pen.h"
#include "outline2D.h"

namespace Vangoh {


class GraphicObject
{
public:
    GraphicObject();

protected:

};


class Shape : public GraphicObject
{
public:
    Shape();
    virtual ~ Shape();
    void setPen(Pen& pen);
    std::vector<glm::vec2>& getOutlineVertices();
    std::vector<glm::vec2>& getShapeVertices();
    std::vector<glm::vec2>& getGuideLineVertices();

    virtual void createVertices();

protected:

    virtual void createOutline() = 0;
    virtual void createOutlineVertices();
    virtual void createShapeVertices();

    Pen _pen;
    Outline2D _outline;

    std::vector<glm::vec2> _outlineVertices;
    std::vector<glm::vec2> _guideLineVertices;
    std::vector<glm::vec2> _shapeVertices;

    glm::mat4 _translationMatrix;
    glm::mat4 _rotateMatrix;
    glm::mat4 _scaleMatrix;
    glm::mat4 _shearingMatrix;

};

class Polygon: public Shape
{
public:
    Polygon();
    Polygon(std::initializer_list<glm::vec2> list);
    Polygon(std::vector<glm::vec2> list);
    virtual ~ Polygon();

protected:
    // void createShapeVertices() override;
    void createOutline() override;
    void makeClose();
    std::vector<glm::vec2> _points;
};

class Line: public Shape
{
public:
    Line(glm::vec2 p0, glm::vec2 p1);
    virtual ~ Line();

protected:
    void createOutline() override;

    glm::vec2 _endPoint0;
    glm::vec2 _endPoint1;
};

class Squircles: public Shape
{
public:
    Squircles(glm::vec2 center, float ra, float rb, float n);
    virtual ~ Squircles();

protected:
    void createOutline() override;

    glm::vec2 _center;
    float _ra;
    float _rb;
    float _n;

};

class BezierCurve: public Shape
{

public:
    BezierCurve(glm::vec2 point0, glm::vec2 point1,
                glm::vec2 point2, glm::vec2 point3);
    virtual ~ BezierCurve();

protected:
    // void createShapeVertices() override;
    void createOutline() override;
    glm::vec2 interp(float t);
    glm::vec2 derivative(float t);
    float arcLength(float t0, float t1, int segments = 100);
    float findTForArcLength(float targetLength, int segments = 100);

    glm::vec2 _point0;
    glm::vec2 _point1;
    glm::vec2 _point2;
    glm::vec2 _point3;
};

}



#endif // AMGRAPHIC_H
