#ifndef TOOLS_H
#define TOOLS_H

#include <glm/glm.hpp>

namespace Vangoh {


enum ClockDirection {
    Clockwise,
    CounterClockwise,
};

glm::vec2 pointRelateTo(const glm::vec2& p0, const glm::vec2& p1,
                        const float distance);
glm::vec3 pointRelateTo(const glm::vec3& p0, const glm::vec3& p1,
                        const float distance);
glm::vec2 orthogonal(const glm::vec2& p0, const glm::vec2& p1);
glm::vec3 orthogonalXY(const glm::vec3& p0, const glm::vec3& p1);
glm::vec3 orthogonalXZ(const glm::vec3& p0, const glm::vec3& p1);
glm::vec3 orthogonalYZ(const glm::vec3& p0, const glm::vec3& p1);
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
}

#endif // TOOLS_H
