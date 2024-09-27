#include "vertexgenerator.h"
#include "tools.h"
#include <iostream>

namespace Anymate {

Triangles::Triangles(const glm::vec2& p0, const
                     glm::vec2& p1, const glm::vec2& p2)
    :
    _p0(p0),
    _p1(p1),
    _p2(p2)
{

}


Frame::Frame(const float weight, const glm::vec2& anchor0, const glm::vec2& anchor1)
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

Frame:: ~Frame()
{

}

bool Frame::edgeContainPoint(const glm::vec2& p, glm::vec2* p0, glm::vec2* p1)
{
    bool ret = false;

    if (isPointOnLineSegment(p, _p0, _p1)) {
        *p0 = _p0;
        *p1 = _p1;
        ret = true;
    }
    else if (isPointOnLineSegment(p, _p1, _p2)) {
        *p0 = _p1;
        *p1 = _p2;
        ret = true;
    }
    else if (isPointOnLineSegment(p, _p2, _p3)) {
        *p0 = _p2;
        *p1 = _p3;
        ret = true;
    }
    else if (isPointOnLineSegment(p, _p3, _p0)) {
        *p0 = _p3;
        *p1 = _p0;
        ret = true;
    }

    return ret;
}

bool Frame::findCrossKeyPoints(Frame& frame, glm::vec2& crossPoint,
                               glm::vec2* p0, glm::vec2* p1, glm::vec2* p2)
{
    float radian = angleToFrame(frame);
    if (radian >=0 && radian < M_PI) {
        *p0 = _p2;
        *p1 = frame.getPoints()[1];
        *p2 = intersectPoint(_p1, _p2, frame.getPoints()[1], frame.getPoints()[2]);
    }
    else if (radian >= M_PI && radian <= 2*M_PI) {
        *p0 = _p3;
        *p1 = frame.getPoints()[0];
        *p2 = intersectPoint(_p0, _p3, frame.getPoints()[0], frame.getPoints()[3]);
    }
    return true;
}

glm::vec2 Frame::getAnchor0()
{
    return _anchor0;
}

glm::vec2 Frame::getAnchor1()
{
    return _anchor1;
}


float Frame::angleToFrame(Frame& frame)
{
    glm::vec2 v0 = _anchor1 - _anchor0;
    glm::vec2 v1 = frame.getAnchor1() - frame.getAnchor0();
    return angle2Vectors(v0, v1, CounterClockwise);
}

std::vector<glm::vec2> Frame::getPoints()
{
    std::vector<glm::vec2> points = {_p0, _p1, _p2, _p3};
    return points;
}

VertexGenerator::VertexGenerator()
{

}

VertexGenerator::~VertexGenerator()
{

}

std::vector<Triangles> createPaddings(Frame& frame0, Frame& frame1,
                                      JoinStyle joinStyle)
{
    std::vector<Triangles> triangles;

    // find connection point
    glm::vec2 connectPoint;
    bool connected = false;
    if (frame0.getAnchor0() == frame1.getAnchor0() ||
        frame0.getAnchor0() == frame1.getAnchor1()) {
        connected = true;
        connectPoint = frame0.getAnchor0();
    }
    else if (frame0.getAnchor1() == frame1.getAnchor0() ||
               frame0.getAnchor1() == frame1.getAnchor1()) {
        connected = true;
        connectPoint = frame0.getAnchor1();
    }

    if (!connected) {
        return triangles;
    }

    glm::vec2 keyPoint0, keyPoint1, keyPoint2;
    if (frame0.findCrossKeyPoints(frame1, connectPoint, &keyPoint0,
                                  &keyPoint1, &keyPoint2)) {
        if (joinStyle == BevelJoin) {
            triangles.push_back(Triangles(keyPoint0, connectPoint, keyPoint1));
        }
        else if (joinStyle == MiterJoin) {
            triangles.push_back(Triangles(keyPoint0, connectPoint, keyPoint2));
            triangles.push_back(Triangles(keyPoint2, connectPoint, keyPoint1));
        }
        else if (joinStyle == RoundJoin) {
            glm::vec2 v0 = keyPoint0 - connectPoint;
            glm::vec2 v1 = keyPoint1 - connectPoint;

            float a = angle2Vectors(v0, v1, CounterClockwise);
            glm::vec2 p0 = keyPoint0;
            if (a >= M_PI) {
                a = 2* M_PI - a;
                p0 = keyPoint1;
            }

            int interps = 20;
            float step = a / (float)interps;
            for (int i = 0; i < interps; i ++) {
                glm::vec2 p = pointRotateAround(connectPoint, p0, step,
                                                CounterClockwise);
                triangles.push_back(Triangles(p0, p, connectPoint));
                p0 = p;
            }

        }
    }
    return triangles;
}

std::vector<glm::vec2> VertexGenerator::generate(Pen& pen, Outline& outline)
{
    std::vector<glm::vec2> vertices;

    std::vector<OutlineSegment>& segments = outline.getSegments();
    if (!segments.size()) {
        return vertices;
    }

    for (int i = 0; i < segments.size(); i ++) {

        OutlineSegment segment = segments[i];
        std::vector<glm::vec2> anchors = segment.getAnchors();
        float weight = pen.getWidth();

        // Segment with 3 anchors, anchors[1] is a cusp point
        if (anchors.size() == 3) {
            Frame frame0(weight, anchors[0], anchors[1]);
            Frame frame1(weight, anchors[1], anchors[2]);
            pushFrame(vertices, frame0);
            pushFrame(vertices, frame1);

            std::vector<Triangles> paddings =
                createPaddings(frame0, frame1, pen.getJoinStyle());
            for (auto triangle : paddings) {
                pushPoints(vertices, triangle._p0, triangle._p1, triangle._p2);
            }
        }
        // Segment with 2 anchors
        else if (anchors.size() == 2) {
            if (!segment.containCusp()) {
                Frame frame0(weight, anchors[0], anchors[1]);
                pushFrame(vertices, frame0);
            }
            else {
                Frame frame0 = Frame(weight, anchors[0], anchors[1]);
                pushFrame(vertices, frame0);

                if (i < segments.size() -1) {
                    OutlineSegment nextSegment = segments[i+1];
                    std::vector<glm::vec2> nextPoints = nextSegment.getAnchors();
                    Frame frame1 = Frame(weight, nextPoints[0], nextPoints[1]);

                    std::vector<Triangles> paddings =
                        createPaddings(frame0, frame1, pen.getJoinStyle());
                    for (auto triangle : paddings) {
                        pushPoints(vertices, triangle._p0, triangle._p1, triangle._p2);
                    }
                }
                else {
                    if (outline.isClosedPath()) {
                        OutlineSegment nextSegment = segments[0];
                        std::vector<glm::vec2> nextPoints = nextSegment.getAnchors();
                        Frame frame1 = Frame(weight, nextPoints[0], nextPoints[1]);

                        std::vector<Triangles> paddings =
                            createPaddings(frame0, frame1, pen.getJoinStyle());
                        for (auto triangle : paddings) {
                            pushPoints(vertices, triangle._p0, triangle._p1, triangle._p2);
                        }
                    }
                }
            }
        }
    }

    return vertices;
}


std::vector<glm::vec2> VertexGenerator::guideLines(Pen& pen, Outline& outline)
{

    std::vector<glm::vec2> vertices;

    std::vector<OutlineSegment>& segments = outline.getSegments();
    if (!segments.size()) {
        return vertices;
    }

    for (int i = 0; i < segments.size(); i ++) {

        OutlineSegment segment = segments[i];
        std::vector<glm::vec2> anchors = segment.getAnchors();
        float weight = pen.getWidth();
        float w = weight / 2.0;

        // Segment with 3 points, point[1] is a cusp point
        if (anchors.size() == 3) {
            glm::vec2 normal = orthogonal(anchors[0], anchors[1], CounterClockwise);
            glm::vec2 p0 = anchors[0] + (normal * (w));
            glm::vec2 p1 = anchors[0] - (normal * (w));
            pushPoints(vertices, p0);
            pushPoints(vertices, p1);

            p0 = anchors[1] + (normal * (w));
            p1 = anchors[1] - (normal * (w));
            pushPoints(vertices, p0);
            pushPoints(vertices, p1);

            normal = orthogonal(anchors[1], anchors[2], CounterClockwise);
            p0 = anchors[1] + (normal * (w));
            p1 = anchors[1] - (normal * (w));
            pushPoints(vertices, p0);
            pushPoints(vertices, p1);

            p0 = anchors[2] + (normal * (w));
            p1 = anchors[2] - (normal * (w));
            pushPoints(vertices, p0);
            pushPoints(vertices, p1);
        }
        // Segment with 2 points
        else if (anchors.size() == 2) {
            if (!segment.containCusp()) {
                glm::vec2 normal = orthogonal(anchors[0], anchors[1], CounterClockwise);
                glm::vec2 p0 = anchors[0] + (normal * (w));
                glm::vec2 p1 = anchors[0] - (normal * (w));
                pushPoints(vertices, p0);
                pushPoints(vertices, p1);

                p0 = anchors[1] + (normal * (w));
                p1 = anchors[1] - (normal * (w));
                pushPoints(vertices, p0);
                pushPoints(vertices, p1);

            }
            else {
                // with cusp point
                // if (i < segments.size() -1) {
                    glm::vec2 normal = orthogonal(anchors[0], anchors[1], CounterClockwise);
                    glm::vec2 p0 = anchors[0] + (normal * (w));
                    glm::vec2 p1 = anchors[0] - (normal * (w));
                    pushPoints(vertices, p0);
                    pushPoints(vertices, p1);

                    p0 = anchors[1] + (normal * (w));
                    p1 = anchors[1] - (normal * (w));
                    pushPoints(vertices, p0);
                    pushPoints(vertices, p1);

                // }
            }
        }
        else {
            // should never got here
        }
    }

    return vertices;
}

void VertexGenerator::pushPoints(std::vector<glm::vec2>& vertices, const glm::vec2& p)
{
    vertices.push_back(glm::vec2(p.x, p.y));
}

void VertexGenerator::pushPoints(std::vector<glm::vec2>& vertices,
                const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2)
{
    vertices.push_back(glm::vec2(p0.x, p0.y));
    vertices.push_back(glm::vec2(p1.x, p1.y));
    vertices.push_back(glm::vec2(p2.x, p2.y));
}

void VertexGenerator::pushPoints(std::vector<glm::vec2>& vertices,
                                 const glm::vec2& p0, const glm::vec2& p1,
                                 const glm::vec2& p2, const glm::vec2& p3)
{
    vertices.push_back(glm::vec2(p0.x, p0.y));
    vertices.push_back(glm::vec2(p1.x, p1.y));
    vertices.push_back(glm::vec2(p3.x, p3.y));
    vertices.push_back(glm::vec2(p3.x, p3.y));
    vertices.push_back(glm::vec2(p1.x, p1.y));
    vertices.push_back(glm::vec2(p2.x, p2.y));
}

void VertexGenerator::pushFrame(std::vector<glm::vec2>& vertices, Frame& frame)
{
    std::vector<glm::vec2> points = frame.getPoints();
    pushPoints(vertices, points[0], points[1], points[2], points[3]);
}

}
