#ifndef AMGRAPHIC_H
#define AMGRAPHIC_H

#include <cstdint>
#include <vector>
#include <initializer_list>
#include <glm/glm.hpp>

namespace Anymate {

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

    enum JoinStyle {
        BevelJoin,
        MiterJoin,
        RoundJoin,
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

    std::vector<glm::vec3>& getVertices(VertexType type);
    virtual void createVertices();

protected:

    constexpr const static float borderVerticesZWeight = 0.0;
    constexpr const static float shapeVerticesZWeight = -0.01;

    virtual void createOutlinePoints();
    virtual void createBorderVertices();
    virtual void createShapeVertices();
    bool isCuspIndex(int i);
    glm::vec3 orthogonal(glm::vec3& p0, glm::vec3& p1);

    float _borderWidth;
    BorderStyle _borderStyle;
    glm::vec4 _borderColor;

    FillStyle _fillStyle;
    glm::vec3 _fillSolidColor;
    LinearGradient _fillLinearGradient;
    RadialGradient _fillRadialGradient;

    std::vector<glm::vec3> _outlinePoints;
    std::vector<glm::vec3> _borderVertices;
    std::vector<glm::vec3> _shapeVertices;
    std::vector<glm::vec3> _errorVertices;
    std::vector<int> _cuspIndexes;

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
                float rightBottomRadius, float leftBottomRadius);
    virtual ~ RoundedRect();

protected:
    constexpr const static float minCornerRadius = 1.0;

    void createOutlinePoints() override;

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

protected:
    void createOutlinePoints() override;

private:

};

class Rect : public GeometryShape
{
public:
    Rect();
    Rect(float x, float y, float width, float height);
    virtual ~ Rect();

protected:
    void createOutlinePoints() override;
    void createBorderVertices() override;
    void createShapeVertices() override;
};

class Bezier : public GeometryShape
{
public:
    Bezier();
    Bezier(std::initializer_list<glm::vec2> list);
    virtual ~ Bezier();
    void appendPoint(glm::vec2 point);

protected:
    void createOutlinePoints() override;

    glm::vec2 bezier(float t, const glm::vec2& p0, const glm::vec2& p1,
                     const glm::vec2& p2, const glm::vec2& p3);

private:
    std::vector<glm::vec2> _points;
};


class Polygon: public GeometryShape
{
public:
    Polygon();
    Polygon(std::initializer_list<glm::vec3> list);
    virtual ~ Polygon();

protected:
    glm::vec3 getCenter();
    void createOutlinePoints() override;
    void createBorderVertices() override;
    void createShapeVertices() override;

    std::vector<glm::vec3> _points;
};

}



#endif // AMGRAPHIC_H
