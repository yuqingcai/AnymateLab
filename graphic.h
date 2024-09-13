#ifndef AMGRAPHIC_H
#define AMGRAPHIC_H

#include <cstdint>
#include <vector>
#include <initializer_list>
#include <glm/glm.hpp>

namespace Anymate {

class Graphic
{
public:
    Graphic();

    void setX(float x);
    void setY(float y);
    void setWidth(float width);
    void setHeight(float height);

    float getX();
    float getY();
    float getWidth();
    float getHeight();

    virtual void generateCurve() = 0
        ;
protected:

    float _x;
    float _y;
    float _width;
    float _height;
};

class GeometryShape : public Graphic
{
public:
    enum BorderType {
        None,
        Solid,
        Dash,
        Dot,
    };
    GeometryShape();
    virtual ~ GeometryShape();

    void setBorderType(BorderType type);
    void setBorderWidth(float borderWidth);
    BorderType getBorderType();
    float getBorderWidth();

    void setCurveSegments(uint32_t segments);
    uint32_t getCurveSegments();
    float* getVertexBuffer();
    uint32_t getVertexCount();
    uint32_t getVertexBufferSize();

    virtual void generateCurve() override;

protected:

    BorderType _borderType;
    float _borderWidth;
    uint32_t _curveSegments;
    float* _vertexBuffer;
    uint32_t _vertexCount;
    uint32_t _vertexBufferSize;

};

class Rectangle : public GeometryShape
{
public:
    Rectangle();
    virtual ~ Rectangle();
    virtual void generateCurve() override;

protected:

};


class Bezier : public GeometryShape
{
public:
    Bezier();
    Bezier(std::initializer_list<glm::vec2> list);
    virtual ~ Bezier();
    void appendPoint(glm::vec2 point);
    virtual void generateCurve() override;

protected:

    glm::vec2 bezier(float t, const glm::vec2& p0, const glm::vec2& p1,
                     const glm::vec2& p2, const glm::vec2& p3);

private:
    std::vector<glm::vec2> _points;

};


class Circle : public GeometryShape
{
public:
    Circle();
    Circle(float radius);
    virtual ~ Circle();
    virtual void generateCurve() override;

    void setRadius(float radius);
    float getRadius();
protected:

private:
    float _radius;

};

}



#endif // AMGRAPHIC_H
