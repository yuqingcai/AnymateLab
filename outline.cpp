#include <stdlib.h>
#include "outline.h"
#include "tools.h"

namespace Vangoh {

OutlineSegmentFrame::OutlineSegmentFrame(const float weight,
                                         const glm::vec2& anchor0,
                                         const glm::vec2& anchor1)
{
    _weight = weight;
    _anchor0 = anchor0;
    _anchor1 = anchor1;

    glm::vec2 normal = orthogonal(_anchor0, _anchor1, CounterClockwise);
    float w = weight / 2.0;

    _p0 = _anchor0 + (normal * w);
    _p1 = _anchor0 - (normal * w);
    _p2 = _anchor1 - (normal * w);
    _p3 = _anchor1 + (normal * w);
}

OutlineSegmentFrame:: ~OutlineSegmentFrame()
{

}

glm::vec2 OutlineSegmentFrame::connectPoint(OutlineSegmentFrame& frame)
{
    glm::vec2 point;
    if (_anchor0 == frame.getAnchor0() ||
        _anchor0 == frame.getAnchor1()) {
        point = _anchor0;
    }
    else if (_anchor1 == frame.getAnchor0() ||
             _anchor1 == frame.getAnchor1()) {
        point = _anchor1;
    }
    return point;
}

std::vector<glm::vec2>
OutlineSegmentFrame::findGapShapePoints(OutlineSegmentFrame& frame)
{
    glm::vec2 cp = connectPoint(frame);
    std::vector<glm::vec2> shapePoints;
    float radian = angleToFrame(frame);
    std::vector<glm::vec2> points = frame.getPoints();
    if (radian >=0 && radian < M_PI) {
        shapePoints.push_back(_p2);
        shapePoints.push_back(cp);
        shapePoints.push_back(points[1]);
        shapePoints.push_back(intersectPoint(_p1, _p2, points[1], points[2]));
    }
    else if (radian >= M_PI && radian <= 2*M_PI) {
        shapePoints.push_back(_p3);
        shapePoints.push_back(cp);
        shapePoints.push_back(points[0]);
        shapePoints.push_back(intersectPoint(_p0, _p3, points[0], points[3]));

    }
    return shapePoints;
}

std::vector<glm::vec2>
OutlineSegmentFrame::findIntersectionShapePoints(OutlineSegmentFrame& frame)
{
    glm::vec2 cp = connectPoint(frame);
    float radian = angleToFrame(frame);
    std::vector<glm::vec2> shapePoints;

    std::vector<glm::vec2> points = frame.getPoints();
    if (radian >=0 && radian < M_PI) {
        shapePoints.push_back(cp);
        shapePoints.push_back(points[1]);
        shapePoints.push_back(intersectPoint(_p0, _p3, points[1], points[2]));
        shapePoints.push_back(_p3);

    }
    else if (radian >= M_PI && radian <= 2*M_PI) {
        shapePoints.push_back(cp);
        shapePoints.push_back(_p2);
        shapePoints.push_back(intersectPoint(_p1, _p2, points[0], points[3]));
        shapePoints.push_back(points[0]);
    }

    return shapePoints;
}

glm::vec2 OutlineSegmentFrame::getAnchor0()
{
    return _anchor0;
}

glm::vec2 OutlineSegmentFrame::getAnchor1()
{
    return _anchor1;
}

float OutlineSegmentFrame::angleToFrame(OutlineSegmentFrame& frame)
{
    glm::vec2 v0 = _anchor1 - _anchor0;
    glm::vec2 v1 = frame.getAnchor1() - frame.getAnchor0();
    return angle2Vectors(v0, v1, CounterClockwise);
}

std::vector<glm::vec2> OutlineSegmentFrame::getPoints()
{
    std::vector<glm::vec2> points = {_p0, _p1, _p2, _p3};
    return points;
}


bool OutlineSegmentFrame::connectToFrame(OutlineSegmentFrame& frame)
{
    bool connected = false;
    if (_anchor0 == frame.getAnchor0() || _anchor0 == frame.getAnchor1()) {
        connected = true;
    }
    else if (_anchor1 == frame.getAnchor0() || _anchor1 == frame.getAnchor1()) {
        connected = true;
    }
    return connected;
}

std::vector<glm::vec2> OutlineSegmentFrame::createGapShape(
    OutlineSegmentFrame& frame, JoinStyle joinStyle)
{
    std::vector<glm::vec2> triangles;

    if (!connectToFrame(frame)) {
        return triangles;
    }

    std::vector<glm::vec2> shapePoints = findGapShapePoints(frame);
    if (!shapePoints.empty()) {
        if (joinStyle == BevelJoin) {
            triangles.push_back(shapePoints[0]);
            triangles.push_back(shapePoints[1]);
            triangles.push_back(shapePoints[2]);
        }
        else if (joinStyle == MiterJoin) {
            triangles.push_back(shapePoints[0]);
            triangles.push_back(shapePoints[1]);
            triangles.push_back(shapePoints[3]);
            triangles.push_back(shapePoints[3]);
            triangles.push_back(shapePoints[1]);
            triangles.push_back(shapePoints[2]);

        }
        else if (joinStyle == RoundJoin) {
            glm::vec2 v0 = shapePoints[0] - shapePoints[1];
            glm::vec2 v1 = shapePoints[2] - shapePoints[1];

            float a = angle2Vectors(v0, v1, CounterClockwise);
            glm::vec2 p0 = shapePoints[0];
            if (a >= M_PI) {
                a = 2* M_PI - a;
                p0 = shapePoints[2];
            }

            int interps = 20;
            float step = a / (float)interps;
            for (int i = 0; i < interps; i ++) {
                glm::vec2 p = pointRotateAround(shapePoints[1], p0, step,
                                                CounterClockwise);
                triangles.push_back(p0);
                triangles.push_back(p);
                triangles.push_back(shapePoints[1]);

                p0 = p;
            }
        }
    }

    return triangles;
}


std::vector<glm::vec2> OutlineSegmentFrame::createIntersectionsShape(
    OutlineSegmentFrame& frame, JoinStyle joinStyle)
{
    std::vector<glm::vec2> triangles;

    if (!connectToFrame(frame)) {
        return triangles;
    }
    std::vector<glm::vec2> intersection0 = findIntersectionShapePoints(frame);
    if (!intersection0.empty()) {
        triangles.push_back(intersection0[0]);
        triangles.push_back(intersection0[1]);
        triangles.push_back(intersection0[3]);
        triangles.push_back(intersection0[3]);
        triangles.push_back(intersection0[1]);
        triangles.push_back(intersection0[2]);
    }

    OutlineSegmentFrame frame0 = frame.opposite();
    OutlineSegmentFrame frame1 = opposite();
    std::vector<glm::vec2> intersection2 =
        frame0.findIntersectionShapePoints(frame1);
    if (!intersection2.empty()) {
        triangles.push_back(intersection2[0]);
        triangles.push_back(intersection2[1]);
        triangles.push_back(intersection2[3]);
        triangles.push_back(intersection2[3]);
        triangles.push_back(intersection2[1]);
        triangles.push_back(intersection2[2]);
    }

    return triangles;
}

void OutlineSegmentFrame::pushTrianglePoints(std::vector<glm::vec2>& vertices)
{
    // Counter Clockwise
    vertices.push_back(_p0);
    vertices.push_back(_p1);
    vertices.push_back(_p3);
    vertices.push_back(_p3);
    vertices.push_back(_p1);
    vertices.push_back(_p2);
}


OutlineSegmentFrame OutlineSegmentFrame::opposite()
{
    OutlineSegmentFrame frameopp(_weight, _anchor1, _anchor0);
    return frameopp;
}

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
            printf("(%.1f, %.1f)\n", points[j].x, points[j].y);
        }
    }
}


}
