#ifndef TOOLS_H
#define TOOLS_H

#include <glm/glm.hpp>
#include <vector>

namespace Anymate {

enum ClockDirection {
    Clockwise,
    CounterClockwise,
};

glm::vec2 normalize(const glm::vec2& p0, const glm::vec2& p1);
glm::vec2 pointRelateTo(const glm::vec2& p0, const glm::vec2& p1,
                        const float distance);
glm::vec2 orthogonal(const glm::vec2& p0, const glm::vec2& p1,
                     ClockDirection direction);
glm::vec2 intersectPoint(const glm::vec2& p0, const glm::vec2& p1,
                        const glm::vec2& p2, const glm::vec2& p3);
float intersectionAngle(const glm::vec2& p0, const glm::vec2& p1,
                        const glm::vec2& p2, const glm::vec2& p3);
glm::vec2 pointRotateAround(const glm::vec2& center, const glm::vec2& p1,
                            const float radian, ClockDirection direction);
bool isPointOnLineSegment(const glm::vec2& p, const glm::vec2& p0,
                          const glm::vec2& p1);
float angle2Vectors(const glm::vec2& v0, const glm::vec2& v1,
                    ClockDirection direction);

void printPoint(const glm::vec3& p);
void printPoints(const std::vector<glm::vec3> points);

}

#endif // TOOLS_H
