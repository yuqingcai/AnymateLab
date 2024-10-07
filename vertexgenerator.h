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
    std::vector<glm::vec2> generate(Pen& pen, Outline& outline);
    std::vector<glm::vec2> guideLines(Pen& pen, Outline& outline);

private:


    std::vector<glm::vec2> createOutlineSegment(
        Outline& outline, int from, int to, float weight, JoinStyle joinStyle);

};
}


#endif // VERTEXGENERATOR_H
