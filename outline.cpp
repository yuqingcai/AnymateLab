#include "outline.h"
#include <iostream>
#include <format>
#include <string>

namespace Anymate {

OutlineSegment::OutlineSegment()
{

}

OutlineSegment::~OutlineSegment()
{

}

OutlineSegment::OutlineSegment(std::initializer_list<glm::vec2> anchors)
    :
    _containCusp(false)
{
    for (glm::vec2 p : anchors) {
        _anchors.push_back(p);
    }
}

void OutlineSegment::appendAnchor(glm::vec2 anchor)
{
    _anchors.push_back(anchor);
}

std::vector<glm::vec2>& OutlineSegment::getAnchors()
{
    return _anchors;
}

bool OutlineSegment::containCusp()
{
    return _containCusp;
}

void OutlineSegment::setContainCusp(bool flag)
{
    _containCusp = flag;
}

Outline::Outline()
{
    _segments.clear();
}

Outline:: ~ Outline()
{
    _segments.clear();
}
void Outline::reset()
{
    _segments.clear();
}

void Outline::appendSegment(OutlineSegment& segment)
{
    _segments.push_back(segment);
}


void Outline::appendSegment(glm::vec2 anchor0, glm::vec2 anchor1)
{
    OutlineSegment segment = OutlineSegment({anchor0, anchor1});
    _segments.push_back(segment);
}

void Outline::lastSegmentAppendAnchor(glm::vec2 anchor)
{
    _segments.back().appendAnchor(anchor);
    _segments.back().setContainCusp(true);
}

std::vector<OutlineSegment>& Outline::getSegments()
{
    return _segments;
}


bool Outline::isClosedPath()
{
    return _segments.front().getAnchors().front() ==
           _segments.back().getAnchors().back();
}

void Outline::print()
{
    for (int i = 0; i < _segments.size(); i ++) {
        std::vector<glm::vec2>& points = _segments[i].getAnchors();
        for (int j = 0; j < points.size(); j ++) {
            std::string result = std::format("({:0>10.1f}, {:0>10.1f}) ",
                points[j].x, points[j].y);
            std::cout << result;
        }
        std::cout << std::endl;
    }
}


}
