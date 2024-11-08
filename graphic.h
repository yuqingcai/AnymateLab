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

class BezierCurve : public Shape
{

public:
    BezierCurve(glm::vec2 point0, glm::vec2 point1,
                glm::vec2 point2, glm::vec2 point3);
    virtual ~BezierCurve();

    glm::vec2& getPoint0();
    glm::vec2& getPoint1();
    glm::vec2& getPoint2();
    glm::vec2& getPoint3();

    void setPoint0(glm::vec2& point0);
    void setPoint1(glm::vec2& point1);
    void setPoint2(glm::vec2& point2);
    void setPoint3(glm::vec2& point3);

protected:
    void createOutline() override;
    glm::vec2 interp(float t);

    glm::vec2 _point0;
    glm::vec2 _point1;
    glm::vec2 _point2;
    glm::vec2 _point3;
};

class Rectangle: public Shape
{
public:
    Rectangle(glm::vec2 center, float width, float height);
    virtual ~ Rectangle();
    glm::vec2& getCenter();
    void setCenter(glm::vec2& point);

protected:
    void createOutline() override;

    glm::vec2 _center;
    float _width;
    float _height;
};


class Line : public Shape
{
public:
    Line(glm::vec2 p0, glm::vec2 p1);
    virtual ~Line();

    glm::vec2& getPoint0();
    glm::vec2& getPoint1();

    void setPoint0(glm::vec2& point0);
    void setPoint1(glm::vec2& point1);

protected:
    void createOutline() override;

    glm::vec2 _point0;
    glm::vec2 _point1;
};

}



#endif // AMGRAPHIC_H
