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
    std::vector<glm::vec3> createOutlineMeshes(Pen& pen, Outline& outline);
    std::vector<glm::vec3> createOutlineGuideLines(Pen& pen, Outline& outline);

private:
    std::vector<glm::vec3> createOutlineGuideLineUnit(Outline& outline,
                                                      int i,
                                                      float weight);

    std::vector<glm::vec3> createOutlineMesh(Outline& outline,
                                             int i,
                                             float height,
                                             float length,
                                             JoinStyle joinStyle,
                                             CapStyle capStyle);


    bool verticesOfOutlineMesh(Outline& outline,
                               int i,
                               float height,
                               float length,
                               glm::vec3* pi0,
                               glm::vec3* pi1,
                               glm::vec3* pi2,
                               glm::vec3* pi3,
                               glm::vec3* pi4,
                               glm::vec3* pi5,
                               glm::vec3* pi6,
                               glm::vec3* pi7,
                               glm::vec3* pi8,
                               glm::vec3* pi9,
                               glm::vec3* pi10,
                               glm::vec3* pi11);
};
}


#endif // VERTEXGENERATOR_H
