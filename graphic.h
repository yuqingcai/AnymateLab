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
    Graphic(float x, float y, float width, float height);

    void setX(float x);
    void setY(float y);
    void setWidth(float width);
    void setHeight(float height);

    float getX();
    float getY();
    float getWidth();
    float getHeight();

    virtual void draw() = 0;

protected:

    float _x;
    float _y;
    float _width;
    float _height;
};


class GeometryShape : public Graphic
{
public:

    GeometryShape();
    GeometryShape(float x, float y, float width, float height);
    virtual ~ GeometryShape();
    void setPen(Pen& pen);
    std::vector<glm::vec3>& getBorderVertices();
    std::vector<glm::vec3>& getShapeVertices();

    void draw() override;

protected:

    virtual void createOutline() = 0;
    virtual void createBorderVertices();
    virtual void createShapeVertices();

    VertexGenerator _vertexGenerator;
    Pen _pen;
    Outline _outline;
    std::vector<glm::vec3> _borderVertices;
    std::vector<glm::vec3> _shapeVertices;

    glm::mat4 _rotateMatrix;
    glm::mat4 _scaleMatrix;
    glm::mat4 _shearingMatrix;

};

class Polygon: public GeometryShape
{
public:
    Polygon();
    Polygon(std::initializer_list<glm::vec3> list);
    Polygon(std::vector<glm::vec3> list);
    virtual ~ Polygon();

protected:
    void createShapeVertices() override;
    void createOutline() override;

    void sortPointsCounterClockwise();

    glm::vec3 getCenter();

    std::vector<glm::vec3> _points;
};



}



#endif // AMGRAPHIC_H
