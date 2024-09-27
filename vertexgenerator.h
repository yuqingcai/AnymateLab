#ifndef VERTEXGENERATOR_H
#define VERTEXGENERATOR_H

#include "pen.h"
#include "outline.h"

namespace Anymate {

struct Triangles {
    Triangles(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2);
    glm::vec2 _p0;
    glm::vec2 _p1;
    glm::vec2 _p2;
};

class Frame final {

public:
    Frame(const float weight, const glm::vec2& base0, const glm::vec2& base1);
    virtual ~Frame();
    bool edgeContainPoint(const glm::vec2& p, glm::vec2* p0, glm::vec2* p1);
    std::vector<glm::vec2> getPoints();
    bool findCrossKeyPoints(Frame& frame, glm::vec2& crossPoint,
                            glm::vec2* p0, glm::vec2* p1, glm::vec2* p2);
    glm::vec2 getAnchor0();
    glm::vec2 getAnchor1();
    float angleToFrame(Frame& frame);

    std::vector<Triangles> createPaddings(Frame& frame0, Frame& frame1,
                                          JoinStyle joinStyle);

private:
    float _weight;
    glm::vec2 _anchor0;
    glm::vec2 _anchor1;
    glm::vec2 _p0;
    glm::vec2 _p1;
    glm::vec2 _p2;
    glm::vec2 _p3;
} ;

class VertexGenerator
{
public:
    VertexGenerator();
    ~VertexGenerator();
    std::vector<glm::vec2> generate(Pen& pen, Outline& outline);
    std::vector<glm::vec2> guideLines(Pen& pen, Outline& outline);

private:
    void pushPoints(std::vector<glm::vec2>& vertices, const glm::vec2& p);

    void pushPoints(std::vector<glm::vec2>& vertices,
                    const glm::vec2& p0, const glm::vec2& p1,
                    const glm::vec2& p2);

    void pushPoints(std::vector<glm::vec2>& vertices,
                    const glm::vec2& p0, const glm::vec2& p1,
                    const glm::vec2& p2, const glm::vec2& p3);

    void pushFrame(std::vector<glm::vec2>& vertices, Frame& frame);
    void pushTriangle(std::vector<glm::vec2>& vertices, Frame& frame);



};
}


#endif // VERTEXGENERATOR_H
