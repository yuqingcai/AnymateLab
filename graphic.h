#ifndef AMGRAPHIC_H
#define AMGRAPHIC_H

#include <vector>
#include <initializer_list>
#include <glm/glm.hpp>
#include <color.h>
#include "outline.h"
#include "pen.h"
#include "vertexgenerator.h"

namespace Vangoh {


class Graphic
{
public:
    Graphic();
    Graphic(float x, float y, float z);

    void setX(float x);
    void setY(float y);
    void setZ(float z);

    float getX();
    float getY();
    float getZ();

    virtual void draw() = 0;

protected:

    float _x;
    float _y;
    float _z;
};


class Shape : public Graphic
{
public:
    Shape();
    Shape(float x, float y, float z);
    virtual ~ Shape();
    void setPen(Pen& pen);
    std::vector<glm::vec3>& getOutlineVertices();
    std::vector<glm::vec3>& getShapeVertices();
    std::vector<glm::vec3>& getGuideLineVertices();

    void draw() override;

protected:

    virtual void createOutline() = 0;
    virtual void createOutlineVertices();
    virtual void createShapeVertices();

    VertexGenerator _vertexGenerator;

    Pen _pen;
    Outline _outline;

    std::vector<glm::vec3> _outlineVertices;
    std::vector<glm::vec3> _guideLineVertices;
    std::vector<glm::vec3> _shapeVertices;

    glm::mat4 _translationMatrix;
    glm::mat4 _rotateMatrix;
    glm::mat4 _scaleMatrix;
    glm::mat4 _shearingMatrix;

};

class Polygon: public Shape
{
public:
    Polygon();
    Polygon(std::initializer_list<glm::vec3> list);
    Polygon(std::vector<glm::vec3> list);
    virtual ~ Polygon();

protected:
    void createShapeVertices() override;
    void createOutline() override;
    std::vector<glm::vec3> _points;

};

class Line: public Shape
{
public:
    Line(glm::vec3 p0, glm::vec3 p1);
    virtual ~ Line();

protected:
    void createOutline() override;

    glm::vec3 _endPoint0;
    glm::vec3 _endPoint1;
};



}



#endif // AMGRAPHIC_H
