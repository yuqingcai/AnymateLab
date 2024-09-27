#ifndef OUTLINE_H
#define OUTLINE_H

#include <vector>
#include <initializer_list>
#include <glm/glm.hpp>

namespace Anymate {

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
