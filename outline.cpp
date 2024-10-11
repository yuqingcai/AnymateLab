#include <stdio.h>
#include "outline.h"

namespace Vangoh {

OutlinePoint::OutlinePoint(glm::vec3& position):
    _position(position),
    _isCuspPoint(false),
    _isBreakPoint(false)
{
}

OutlinePoint::~OutlinePoint()
{

}

bool OutlinePoint::isCuspPoint()
{
    return _isCuspPoint;
}

void OutlinePoint::setCuspPoint(bool flag)
{
    _isCuspPoint = flag;
}


bool OutlinePoint::isBreakPoint()
{
    return _isBreakPoint;
}

void OutlinePoint::setBreakPoint(bool flag)
{
    _isBreakPoint = flag;
}

glm::vec3& OutlinePoint::getPosition()
{
    return _position;
}

Outline::Outline()
{

}

Outline::~Outline()
{

}

void Outline::reset()
{
    _points.clear();
}

void Outline::appendPosition(glm::vec3& position)
{
    _points.push_back(OutlinePoint(position));
}


std::vector<OutlinePoint>& Outline::getPoints()
{
    return _points;
}

bool Outline::isClosedPath()
{
    return _points.back().getPosition() == _points.front().getPosition();
}

void Outline::print()
{
    printf("Points:\n");
    for (auto point : _points) {
            glm::vec3& position = point.getPosition();
            printf("(%.1f, %.1f, %.1f) ", position.x, position.y, position.z);
    }
    printf("\n");


    printf("Cusp Points:\n");
    for (auto point : _points) {
        if (point.isCuspPoint()) {
            glm::vec3& position = point.getPosition();
            printf("(%.1f, %.1f, %.1f) ", position.x, position.y, position.z);
        }
    }
    printf("\n");


    printf("Break Points:\n");
    for (auto point : _points) {
        if (point.isBreakPoint()) {
            glm::vec3& position = point.getPosition();
            printf("(%.1f, %.1f, %.1f) ", position.x, position.y, position.z);
        }
    }
    printf("\n");

    fflush(stdout);
}

}
