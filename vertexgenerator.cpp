#include "vertexgenerator.h"
#include <iostream>
#include <cmath>
#include "tools.h"

namespace Vangoh {

VertexGenerator::VertexGenerator()
{

}

VertexGenerator::~VertexGenerator()
{

}

std::vector<glm::vec2>
VertexGenerator::createOutlineSegment(Outline& outline, int from, int to,
                                      float weight, JoinStyle joinStyle)
{
    std::vector<glm::vec2> vertices;

    std::vector<OutlinePoint>& points = outline.getPoints();
    int n = points.size();
    if (!n || n < 2 || from >= n || to >= n) {
        return vertices;
    }

    float w = weight / 2.0;

    printf("from:%d to:%d\n", from, to);
    fflush(stdout);

    for (int i = from; i <= to; i ++) {

        if (points[i].isCuspPoint()) {
            // glm::vec2 pi = points[i].getPosition();

            // glm::vec2 pk = points[i - 1].getPosition();
            // glm::vec2 normal = orthogonal(pk, pi, CounterClockwise);
            // glm::vec2 p0 = pi + (normal * (w));
            // glm::vec2 p1 = pi - (normal * (w));

            // pk = points[i + 1].getPosition();
            // normal = orthogonal(pi, pk, CounterClockwise);
            // glm::vec2 p2 = pi + (normal * (w));
            // glm::vec2 p3 = pi - (normal * (w));

            // vertices.push_back(pi);
            // vertices.push_back(p2);
            // vertices.push_back(p0);
        }
        else {
            int j = i + 1;
            glm::vec2 pi = points[i].getPosition();
            glm::vec2 pj = points[j].getPosition();
            glm::vec2 normal = orthogonal(pi,pj, CounterClockwise);
            glm::vec2 p0 = pi + (normal * (w));
            glm::vec2 p1 = pi - (normal * (w));
            glm::vec2 p2 = pj + (normal * (w));
            glm::vec2 p3 = pj - (normal * (w));

            vertices.push_back(p0);
            vertices.push_back(p1);
            vertices.push_back(p3);
            vertices.push_back(p3);
            vertices.push_back(p2);
            vertices.push_back(p0);
        }

    }


    return vertices;
}


std::vector<glm::vec2> VertexGenerator::generate(Pen& pen, Outline& outline)
{
    std::vector<glm::vec2> vertices;

    std::vector<OutlinePoint>& points = outline.getPoints();
    int n = points.size();
    if (!n || n < 2) {
        return vertices;
    }


    int steps = static_cast<int>(std::round(pen.getWidth()));
    float weight = pen.getWidth();
    JoinStyle style = pen.getJoinStyle();

    int j = 0;
    int c = 0;
    for (int i = 0; i < n; i ++) {
        OutlinePoint& point = points[i];

        if (c == 0) {
            j = i;
        }

        if (!point.isBreakPoint()) {
            c ++;
        }

        if (c == steps || i == n - 2) {
            std::vector<glm::vec2> segment =
                createOutlineSegment(outline, j, i, weight, style);
            vertices.insert(vertices.end(), segment.begin(), segment.end());
            c = 0;
        }

    }


    return vertices;
}


std::vector<glm::vec2> VertexGenerator::guideLines(Pen& pen, Outline& outline)
{
    std::vector<glm::vec2> vertices;

    std::vector<OutlinePoint>& points = outline.getPoints();
    int n = points.size();
    if (!n) {
        return vertices;
    }

    for (int i = 0; i < n; i ++) {
        float weight = pen.getWidth();
        float w = weight / 2.0;

        int j = i + 1;
        if (i == n-1) {
            j = 0;
        }

        if (points[i].isCuspPoint()) {
            if (i == n - 1){
                int k = i - 1;
                glm::vec2 pk = points[k].getPosition();
                glm::vec2 pi = points[i].getPosition();
                glm::vec2 normal = orthogonal(pk, pi, CounterClockwise);
                glm::vec2 p0 = pi + (normal * (w));
                glm::vec2 p1 = pi - (normal * (w));
                vertices.push_back(p0);
                vertices.push_back(p1);

                if (outline.isClosedPath()) {
                    k = 1;
                    pi = points[i].getPosition();
                    pk = points[k].getPosition();
                    glm::vec2 normal = orthogonal(pi, pk, CounterClockwise);
                    glm::vec2 p0 = pi + (normal * (w));
                    glm::vec2 p1 = pi - (normal * (w));
                    vertices.push_back(p0);
                    vertices.push_back(p1);
                }

            }
            else {
                int k = i - 1;
                glm::vec2 pk = points[k].getPosition();
                glm::vec2 pi = points[i].getPosition();
                glm::vec2 normal = orthogonal(pk, pi, CounterClockwise);
                glm::vec2 p0 = pi + (normal * (w));
                glm::vec2 p1 = pi - (normal * (w));
                vertices.push_back(p0);
                vertices.push_back(p1);

                k = i + 1;
                pi = points[i].getPosition();
                pk = points[k].getPosition();
                normal = orthogonal(pi, pk, CounterClockwise);
                p0 = pi + (normal * (w));
                p1 = pi - (normal * (w));
                vertices.push_back(p0);
                vertices.push_back(p1);
            }
        }
        else {
            glm::vec2 pi = points[i].getPosition();
            glm::vec2 pj = points[j].getPosition();
            glm::vec2 normal = orthogonal(pi,pj, CounterClockwise);
            glm::vec2 p0 = pi + (normal * (w));
            glm::vec2 p1 = pi - (normal * (w));
            vertices.push_back(p0);
            vertices.push_back(p1);
        }


    }

    return vertices;
}



}
