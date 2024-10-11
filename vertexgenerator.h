#ifndef VERTEXGENERATOR_H
#define VERTEXGENERATOR_H

#include "pen.h"
#include "outline.h"

namespace Vangoh {

class VertexGenerator
{
public:
    VertexGenerator();
    ~VertexGenerator();
    std::vector<glm::vec2> createOutlineFrames(Pen& pen, Outline& outline);
    std::vector<glm::vec2> createOutlineGuideLines(Pen& pen, Outline& outline);

private:
    std::vector<glm::vec2> createOutlineFrame(
        Outline& outline, int i, float weight,
        JoinStyle joinStyle, CapStyle capStyle);

    std::vector<glm::vec2> createOutlineGuideLine(
        Outline& outline, int i, float weight,
        JoinStyle joinStyle, CapStyle capStyle);

    bool verticesOfOutlineFrame(
        Outline& outline, int i, float weight,
        glm::vec2* pi0, glm::vec2* pi1,
        glm::vec2* pi2, glm::vec2* pi3);

    bool verticesOfOutlineCuspFrame(
        Outline& outline, int i, float weight,
        glm::vec2* pij0, glm::vec2* pij1,
        glm::vec2* pj0, glm::vec2* pj1,
        glm::vec2* pik0, glm::vec2* pik1,
        glm::vec2* pk0, glm::vec2* pk1);

    std::vector<glm::vec2> verticesOfGap(
        JoinStyle joinStyle,
        glm::vec2& pi,
        glm::vec2& pij0, glm::vec2& pij1,
        glm::vec2& pj0, glm::vec2& pj1,
        glm::vec2& pik0, glm::vec2& pik1,
        glm::vec2& pk0, glm::vec2& pk1);

    enum CapPosition {
        HeadCap,
        TailCap,
    };

    std::vector<glm::vec2> verticesOfCap(
        CapPosition position,
        CapStyle capStyle,
        glm::vec2& pi0,
        glm::vec2& pi1,
        glm::vec2& pi2,
        glm::vec2& pi3);


};
}


#endif // VERTEXGENERATOR_H
