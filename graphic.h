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
    Graphic(float x, float y, float width, float height);

    void setX(float x);
    void setY(float y);
    void setWidth(float width);
    void setHeight(float height);

    float getX();
    float getY();
    float getWidth();
    float getHeight();

protected:

    float _x;
    float _y;
    float _width;
    float _height;
};

class LinearGradient
{
public:
    LinearGradient();
    LinearGradient(std::initializer_list<glm::vec3> colors);
    ~LinearGradient();

private:
    std::vector<glm::vec4> _colors;
};

class RadialGradient
{
public:
    RadialGradient();
    RadialGradient(std::initializer_list<glm::vec3> colors);
    ~RadialGradient();

private:
    std::vector<glm::vec4> _colors;
};

class GeometryShape : public Graphic
{
public:
    enum BorderStyle {
        NoBorder,
        SolidLineBorder,
        DashLineBorder,
        DotLineBorder,
        DashDotLineBorder,
        DashDotDotLineBorder,
    };

    enum FillStyle {
        NoFill,
        SolidFill,
        LinearGradientFill,
        RadialGradientFill
    };

    enum VertexType {
        ShapeVertex,
        BorderVertex,
    };

    GeometryShape();
    GeometryShape(float x, float y, float width, float height);
    virtual ~ GeometryShape();

    void setBorderStyle(BorderStyle style);
    BorderStyle getBorderStyle();

    void setFillStyle(FillStyle style);
    FillStyle getFillStyle();

    void setBorderWidth(float borderWidth);
    float getBorderWidth();

    void setShapePrecision(uint32_t precision);
    uint32_t getShapePrecision();

    glm::vec3* getVertices(VertexType type);
    virtual void createVertices();

    uint32_t getVertexCount(VertexType type);
    uint32_t getVertexBufferSize(VertexType type);


protected:

    static const uint32_t defaultShapePrecision = 200;

    float _borderWidth;
    BorderStyle _borderStyle;
    glm::vec4 _borderColor;

    FillStyle _fillStyle;
    glm::vec3 _fillSolidColor;
    LinearGradient _fillLinearGradient;
    RadialGradient _fillRadialGradient;

    uint32_t _shapePrecision;

    glm::vec3* _shapeVertices;
    uint32_t _shapeVertexCount;
    uint32_t _shapeVertexBufferSize;

    glm::vec3* _borderVertices;
    uint32_t _borderVertexCount;
    uint32_t _borderVertexBufferSize;

    glm::mat4 _rotateMatrix;
    glm::mat4 _scaleMatrix;
    glm::mat4 _shearingMatrix;
};

class RoundedRect : public GeometryShape
{
public:
    RoundedRect();
    RoundedRect(float x, float y, float width, float height,
                float leftTopRadius, float rightTopRadius,
                float leftBottomRadius, float rightBottomRadius);
    virtual ~ RoundedRect();
    virtual void createVertices() override;

protected:
    void createBorderVierices();
    void createShapeVierices();
    float _leftTopRadius;
    float _rightTopRadius;
    float _leftBottomRadius;
    float _rightBottomRadius;

};

class Oval : public GeometryShape
{
public:
    Oval();
    Oval(float x, float y, float width, float height);
    virtual ~ Oval();
    virtual void createVertices() override;
protected:

private:

};

class Bezier : public GeometryShape
{
public:
    Bezier();
    Bezier(std::initializer_list<glm::vec2> list);
    virtual ~ Bezier();
    void appendPoint(glm::vec2 point);
    virtual void createVertices() override;

protected:

    glm::vec2 bezier(float t, const glm::vec2& p0, const glm::vec2& p1,
                     const glm::vec2& p2, const glm::vec2& p3);

private:
    std::vector<glm::vec2> _points;
};


}



#endif // AMGRAPHIC_H
