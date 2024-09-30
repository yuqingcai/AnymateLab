#include "vertexgenerator.h"
#include <iostream>

namespace Vangoh {

VertexGenerator::VertexGenerator()
{

}

VertexGenerator::~VertexGenerator()
{

}

std::vector<glm::vec2> VertexGenerator::createOutlineSegment(
    Outline& outline, int index, float weight, JoinStyle joinStyle)
{
    std::vector<glm::vec2> vertices;
    std::vector<OutlineSegment>& segments = outline.getSegments();

    if (!segments.size()) {
        return vertices;
    }

    OutlineSegment segment = segments[index];
    std::vector<glm::vec2> anchors = segment.getAnchors();

    if (anchors.size() == 3) {
        OutlineSegmentFrame frame0(weight, anchors[0], anchors[1]);
        OutlineSegmentFrame frame1(weight, anchors[1], anchors[2]);

        frame0.pushTrianglePoints(vertices);
        frame1.pushTrianglePoints(vertices);

        std::vector<glm::vec2> gap = frame0.createGapShape(frame1, joinStyle);
        vertices.insert(vertices.end(), gap.begin(), gap.end());
        std::vector<glm::vec2> ints = frame0.createIntersectionsShape(frame1, joinStyle);
        vertices.insert(vertices.end(), ints.begin(), ints.end());

        if (index == segments.size() -1 && outline.isClosedPath()) {
            OutlineSegment next = segments[0];
            anchors = next.getAnchors();
            OutlineSegmentFrame frame2(weight, anchors[0], anchors[1]);

            std::vector<glm::vec2> gap = frame1.createGapShape(frame2, joinStyle);
            vertices.insert(vertices.end(), gap.begin(), gap.end());
            std::vector<glm::vec2> ints = frame1.createIntersectionsShape(frame2, joinStyle);
            vertices.insert(vertices.end(), ints.begin(), ints.end());
        }
    }
    else if (anchors.size() == 2) {
        OutlineSegmentFrame frame0(weight, anchors[0], anchors[1]);
        frame0.pushTrianglePoints(vertices);

        if (segment.containCusp()) {
            if (index < segments.size() -1) {
                OutlineSegment next = segments[index+1];
                anchors = next.getAnchors();

                OutlineSegmentFrame frame1(weight, anchors[0], anchors[1]);
                std::vector<glm::vec2> gap = frame0.createGapShape(frame1, joinStyle);
                vertices.insert(vertices.end(), gap.begin(), gap.end());
                std::vector<glm::vec2> ints = frame0.createIntersectionsShape(frame1, joinStyle);
                vertices.insert(vertices.end(), ints.begin(), ints.end());
            }
            else {
                if (outline.isClosedPath()) {
                    OutlineSegment next = segments[0];
                    anchors = next.getAnchors();

                    OutlineSegmentFrame frame1(weight, anchors[0], anchors[1]);
                    std::vector<glm::vec2> gap = frame0.createGapShape(frame1, joinStyle);
                    vertices.insert(vertices.end(), gap.begin(), gap.end());
                    std::vector<glm::vec2> ints = frame0.createIntersectionsShape(frame1, joinStyle);
                    vertices.insert(vertices.end(), ints.begin(), ints.end());

                }
            }
        }

    }

    return vertices;
}


std::vector<glm::vec2> VertexGenerator::generate(Pen& pen, Outline& outline)
{
    std::vector<glm::vec2> vertices;

    std::vector<OutlineSegment>& segments = outline.getSegments();
    if (!segments.size()) {
        return vertices;
    }

    for (int i = 0; i < segments.size(); i ++) {
        // if (i % 5 == 1) {
            std::vector<glm::vec2> segment = createOutlineSegment(outline, i,
                pen.getWidth(), pen.getJoinStyle());
            vertices.insert(vertices.end(), segment.begin(), segment.end());
        // }
    }

    return vertices;
}


std::vector<glm::vec2> VertexGenerator::guideLines(Pen& pen, Outline& outline)
{
    std::vector<glm::vec2> vertices;

    // std::vector<OutlineSegment>& segments = outline.getSegments();
    // if (!segments.size()) {
    //     return vertices;
    // }

    // for (int i = 0; i < segments.size(); i ++) {

    //     OutlineSegment segment = segments[i];
    //     std::vector<glm::vec2> anchors = segment.getAnchors();
    //     float weight = pen.getWidth();
    //     float w = weight / 2.0;

    //     // Segment with 3 points, point[1] is a cusp point
    //     if (anchors.size() == 3) {
    //         glm::vec2 normal = orthogonal(anchors[0], anchors[1], CounterClockwise);
    //         glm::vec2 p0 = anchors[0] + (normal * (w));
    //         glm::vec2 p1 = anchors[0] - (normal * (w));
    //         pushPoints(vertices, p0);
    //         pushPoints(vertices, p1);

    //         p0 = anchors[1] + (normal * (w));
    //         p1 = anchors[1] - (normal * (w));
    //         pushPoints(vertices, p0);
    //         pushPoints(vertices, p1);

    //         normal = orthogonal(anchors[1], anchors[2], CounterClockwise);
    //         p0 = anchors[1] + (normal * (w));
    //         p1 = anchors[1] - (normal * (w));
    //         pushPoints(vertices, p0);
    //         pushPoints(vertices, p1);

    //         p0 = anchors[2] + (normal * (w));
    //         p1 = anchors[2] - (normal * (w));
    //         pushPoints(vertices, p0);
    //         pushPoints(vertices, p1);
    //     }
    //     // Segment with 2 points
    //     else if (anchors.size() == 2) {
    //         if (!segment.containCusp()) {
    //             glm::vec2 normal = orthogonal(anchors[0], anchors[1], CounterClockwise);
    //             glm::vec2 p0 = anchors[0] + (normal * (w));
    //             glm::vec2 p1 = anchors[0] - (normal * (w));
    //             pushPoints(vertices, p0);
    //             pushPoints(vertices, p1);

    //             p0 = anchors[1] + (normal * (w));
    //             p1 = anchors[1] - (normal * (w));
    //             pushPoints(vertices, p0);
    //             pushPoints(vertices, p1);

    //         }
    //         else {
    //             // with cusp point
    //             // if (i < segments.size() -1) {
    //                 glm::vec2 normal = orthogonal(anchors[0], anchors[1], CounterClockwise);
    //                 glm::vec2 p0 = anchors[0] + (normal * (w));
    //                 glm::vec2 p1 = anchors[0] - (normal * (w));
    //                 pushPoints(vertices, p0);
    //                 pushPoints(vertices, p1);

    //                 p0 = anchors[1] + (normal * (w));
    //                 p1 = anchors[1] - (normal * (w));
    //                 pushPoints(vertices, p0);
    //                 pushPoints(vertices, p1);

    //             // }
    //         }
    //     }
    //     else {
    //         // should never got here
    //     }
    // }

    return vertices;
}



}
