#include "tools.h"
#include <stdlib.h>
#include <stdio.h>
#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

namespace Vangoh {

glm::vec2 pointRelateTo(const glm::vec2& p0, const glm::vec2& p1,
                        const float distance)
{

    glm::vec2 n = glm::normalize(p1 - p0);
    return p0 + (n * distance);
}

glm::vec3 pointRelateTo(const glm::vec3& p0, const glm::vec3& p1,
                        const float distance)
{

    glm::vec3 n = glm::normalize(p1 - p0);
    return p0 + (n * distance);
}

glm::vec2 orthogonalXY(const glm::vec2& p0, const glm::vec2& p1)
{
    glm::vec2 n = glm::normalize(p1 - p0);
    return glm::vec2(-n.y, n.x);
}

glm::vec3 orthogonalXY(const glm::vec3& p0, const glm::vec3& p1)
{
    glm::vec3 n = glm::normalize(p1 - p0);

    // p1, p0 on Z axis
    if (fabs(n.y) < FLT_EPSILON && fabs(n.x) < FLT_EPSILON) {
        if (n.z >= 0.0)
            return glm::normalize(glm::vec3(0.0, -1.0, 0.0));
        else
            return glm::normalize(glm::vec3(0.0, 1.0, 0.0));
    }

    return glm::vec3(-n.y, n.x, 0.0);
}

glm::vec3 orthogonalXZ(const glm::vec3& p0, const glm::vec3& p1)
{
    glm::vec3 n = glm::normalize(p1 - p0);

    // p1, p0 on Y axis
    if (fabs(n.z) < FLT_EPSILON && fabs(n.x) < FLT_EPSILON) {
        if (n.y >= 0)
            return glm::normalize(glm::vec3(0.0, 0.0, -1.0));
        else
            return glm::normalize(glm::vec3(0.0, 0.0, 1.0));
    }

    return glm::vec3(-n.z, 0.0, n.x);
}

glm::vec3 orthogonalYZ(const glm::vec3& p0, const glm::vec3& p1)
{
    glm::vec3 n = glm::normalize(p1 - p0);
    return glm::vec3(0.0, -n.z, n.y);
}

glm::vec2 intersectPoint(const glm::vec2& p0, const glm::vec2& p1,
                        const glm::vec2& p2, const glm::vec2& p3)
{
    double A1 = p1.y - p0.y;
    double B1 = p0.x - p1.x;
    double C1 = A1 * p0.x + B1 * p0.y;

    double A2 = p3.y - p2.y;
    double B2 = p2.x - p3.x;
    double C2 = A2 * p2.x + B2 * p2.y;

    double det = A1 * B2 - A2 * B1;

    if (det == 0) {
        printf("Lines are parallel or coincident, no unique intersection\n");
        return glm::vec2(0.0, 0.0);
    }

    glm::vec2 intersection;
    intersection.x = (C1 * B2 - C2 * B1) / det;
    intersection.y = (A1 * C2 - A2 * C1) / det;

    return intersection;
}

float intersectionAngle(const glm::vec2& p0, const glm::vec2& p1,
                        const glm::vec2& p2, const glm::vec2& p3)
{
    glm::vec2 v1 = p1 - p0;
    glm::vec2 v2 = p3 - p2;

    float dot = glm::dot(v1, v2);
    float mag_v1 = glm::length(v1);
    float mag_v2 = glm::length(v2);

    float cosTheta = dot / (mag_v1 * mag_v2);

    if (cosTheta < -1.0) cosTheta = -1.0;
    if (cosTheta > 1.0) cosTheta = 1.0;

    return acos(cosTheta);
}

glm::vec2 pointRotateAround(const glm::vec2& center, const glm::vec2& p,
                            const float radian, ClockDirection direction)
{
    glm::vec2 relativePos = p - center;
    float cosA = glm::cos(radian);
    float sinA = glm::sin(radian);

    glm::vec2 rotatedPos;
    if (direction == CounterClockwise) {
        rotatedPos = glm::vec2(
            cosA * relativePos.x - sinA * relativePos.y,
            sinA * relativePos.x + cosA * relativePos.y
        );
    }
    else {
        rotatedPos = glm::vec2(
            cosA * relativePos.x + sinA * relativePos.y,
            -sinA * relativePos.x + cosA * relativePos.y
        );

    }
    glm::vec2 finalPos = rotatedPos + center;
    return finalPos;
}

static float cross2D(const glm::vec2& a, const glm::vec2& b) {
    return a.x * b.y - a.y * b.x;
}

bool isPointOnLineSegment(const glm::vec2& p, const glm::vec2& p0,
                          const glm::vec2& p1)
{
    glm::vec2 p0p = p - p0;
    glm::vec2 p0p1 = p1 - p0;

    // if p0p and p0p1 are parallel
    if (glm::abs(cross2D(p0p, p0p1)) > 1e-6) {
        return false;
    }

    // if p0p and p0p1 has same direction
    if (glm::dot(p0p, p0p1) < 0) {
        return false;
    }

    // if p is between p0p1
    if (glm::length2(p0p) > glm::length2(p0p1)) {
        return false;
    }

    return true;
}

float angle2Vectors(const glm::vec2& v0, const glm::vec2& v1,
                    ClockDirection direction)
{
    float dotProduct = glm::dot(v0, v1);
    float crossProduct = cross2D(v0, v1);
    float angle = glm::atan(crossProduct, dotProduct);

    if (direction == CounterClockwise) {
        if (angle < 0) {
            angle += glm::two_pi<float>();
        }
    }
    else if (direction == Clockwise) {
        if (angle > 0) {
            angle -= glm::two_pi<float>();
        }
    }
    return angle;
}

}

