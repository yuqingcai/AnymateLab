#ifndef OUTLINE_H
#define OUTLINE_H

#include <vector>
#include <initializer_list>
#include <glm/glm.hpp>
#include "vangohnamespace.h"

namespace Vangoh {

class OutlineSegmentFrame final {

public:
    OutlineSegmentFrame(const float weight, const glm::vec2& base0,
                        const glm::vec2& base1);
    virtual ~OutlineSegmentFrame();
    std::vector<glm::vec2> getPoints();
    glm::vec2 getAnchor0();
    glm::vec2 getAnchor1();
    bool connectToFrame(OutlineSegmentFrame& frame);
    glm::vec2 connectPoint(OutlineSegmentFrame& frame);
    float angleToFrame(OutlineSegmentFrame& frame);
    std::vector<glm::vec2> findGapShapePoints(OutlineSegmentFrame& frame);
    std::vector<glm::vec2> findIntersectionShapePoints(OutlineSegmentFrame& frame);
    std::vector<glm::vec2> createGapShape(OutlineSegmentFrame& frame,
                                          JoinStyle joinStyle);
    std::vector<glm::vec2> createIntersectionsShape(OutlineSegmentFrame& frame,
                                                    JoinStyle joinStyle);
    void pushTrianglePoints(std::vector<glm::vec2>& vertices);

    OutlineSegmentFrame opposite();

private:
    float _weight;
    glm::vec2 _anchor0;
    glm::vec2 _anchor1;
    glm::vec2 _p0;
    glm::vec2 _p1;
    glm::vec2 _p2;
    glm::vec2 _p3;
};

class OutlineSegment
{
public:
    OutlineSegment();
    OutlineSegment(std::initializer_list<glm::vec2> anchors);
    virtual ~ OutlineSegment();
    void appendAnchor(glm::vec2 anchor);
    std::vector<glm::vec2>& getAnchors();
    bool containCusp();
    void setContainCusp(bool flag);

protected:
    std::vector<glm::vec2> _anchors;
    bool _containCusp;


private:
};

class Outline
{
public:
    Outline();
    virtual ~ Outline();
    void reset();
    void appendSegment(OutlineSegment& segment);
    void appendSegment(glm::vec2 anchor0, glm::vec2 anchor1);
    void lastSegmentAppendAnchor(glm::vec2 anchor);

    std::vector<OutlineSegment>& getSegments();
    bool isClosedPath();
    void print();

protected:
    std::vector<OutlineSegment> _segments;

};
}


#endif // OUTLINE_H
