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
VertexGenerator::createOutlineFrame(Outline& outline, int i, float weight,
                                    JoinStyle joinStyle)
{
    std::vector<glm::vec2> vertices;

    std::vector<OutlinePoint>& points = outline.getPoints();
    int n = points.size();

    if (!n || n < 2 || i < 0 || i >= n) {
        return vertices;
    }

    float w = weight / 2.0;

    if (points[i].isCuspPoint()) {

        if (i == 0) {
            return vertices;
        }
        else if (i == n - 1) {

        }
        else {
            int j = i - 1;
            int k = i + 1;

            glm::vec2 pj = points[j].getPosition();
            glm::vec2 pi = points[i].getPosition();
            glm::vec2 pk = points[k].getPosition();

            glm::vec2 normal_ji = orthogonal(pj, pi, CounterClockwise);
            glm::vec2 normal_ik = orthogonal(pi, pk, CounterClockwise);

            float offset_ij = glm::distance(pj, pi)/2.0;
            float offset_ik = glm::distance(pi, pk)/2.0;

            glm::vec2 direction_ij = glm::normalize(pj - pi);
            glm::vec2 direction_ik = glm::normalize(pk - pi);

            glm::vec2 pij0 = pi + (normal_ji * (w));
            glm::vec2 pij1 = pi - (normal_ji * (w));
            glm::vec2 pj0 = pij0 + (direction_ij * offset_ij);
            glm::vec2 pj1 = pij1 + (direction_ij * offset_ij);

            glm::vec2 pik0 = pi + (normal_ik * (w));
            glm::vec2 pik1 = pi - (normal_ik * (w));
            glm::vec2 pk0 = pik0 + (direction_ik * offset_ik);
            glm::vec2 pk1 = pik1 + (direction_ik * offset_ik);

            vertices.push_back(pj0);
            vertices.push_back(pj1);
            vertices.push_back(pij1);
            vertices.push_back(pij1);
            vertices.push_back(pij0);
            vertices.push_back(pj0);

            vertices.push_back(pik0);
            vertices.push_back(pik1);
            vertices.push_back(pk1);
            vertices.push_back(pk1);
            vertices.push_back(pk0);
            vertices.push_back(pik0);

            // gap
            if (joinStyle == BevelJoin) {

                glm::vec2 v0 = pij0 - pij1;
                glm::vec2 v1 = pik0 - pik1;
                float a = angle2Vectors(v0, v1, CounterClockwise);

                if (a <= M_PI) {
                    vertices.push_back(pij1);
                    vertices.push_back(pik1);
                    vertices.push_back(pi);
                }
                else {
                    vertices.push_back(pij0);
                    vertices.push_back(pi);
                    vertices.push_back(pik0);
                }

            }
            else if (joinStyle == MiterJoin) {
                glm::vec2 v0 = pij0 - pij1;
                glm::vec2 v1 = pik0 - pik1;
                float a = angle2Vectors(v0, v1, CounterClockwise);

                if (a <= M_PI) {
                    glm::vec2 gp = intersectPoint(pij1, pj1, pik1, pk1);
                    vertices.push_back(pij1);
                    vertices.push_back(pi);
                    vertices.push_back(gp);
                    vertices.push_back(gp);
                    vertices.push_back(pi);
                    vertices.push_back(pik1);

                }
                else {
                    glm::vec2 gp = intersectPoint(pij0, pj0, pik0, pk0);
                    vertices.push_back(pij0);
                    vertices.push_back(pi);
                    vertices.push_back(gp);
                    vertices.push_back(gp);
                    vertices.push_back(pi);
                    vertices.push_back(pik0);
                }

            }
            else if (joinStyle == RoundJoin) {
                glm::vec2 v0 = pij0 - pij1;
                glm::vec2 v1 = pik0 - pik1;
                float a = angle2Vectors(v0, v1, CounterClockwise);

                if (a <= M_PI) {
                    glm::vec2 vij = pij1 - pij0;
                    glm::vec2 vik = pik1 - pik0;
                    a = angle2Vectors(vij, vik, CounterClockwise);

                    int interps = 20;
                    float step = a / (float)interps;
                    glm::vec2 p0 = pij1;
                    for (int i = 0; i < interps; i ++) {
                        glm::vec2 p = pointRotateAround(pi, p0, step, CounterClockwise);
                        vertices.push_back(p0);
                        vertices.push_back(p);
                        vertices.push_back(pi);
                        p0 = p;
                    }

                }
                else {
                    glm::vec2 vik = pik0 - pik1;
                    glm::vec2 vij = pij0 - pij1;
                    a = angle2Vectors(vik, vij, CounterClockwise);

                    int interps = 20;
                    float step = a / (float)interps;
                    glm::vec2 p0 = pik0;
                    for (int i = 0; i < interps; i ++) {
                        glm::vec2 p = pointRotateAround(pi, p0, step, CounterClockwise);
                        vertices.push_back(p0);
                        vertices.push_back(p);
                        vertices.push_back(pi);
                        p0 = p;
                    }

                }
            }
        }
    }
    else {
        if (i == 0) {
            glm::vec2 pi = points[i].getPosition();
            glm::vec2 pk = points[i+1].getPosition();
            glm::vec2 normal_ik = orthogonal(pi, pk, CounterClockwise);
            float offset_ik = glm::distance(pi, pk) / 2.0;
            glm::vec2 direction_ik = glm::normalize(pk - pi);

            glm::vec2 pi0 = pi + (normal_ik * (w));
            glm::vec2 pi1 = pi - (normal_ik * (w));
            glm::vec2 pi2 = pi + (normal_ik * (w)) + (direction_ik * offset_ik);
            glm::vec2 pi3 = pi - (normal_ik * (w)) + (direction_ik * offset_ik);

            vertices.push_back(pi0);
            vertices.push_back(pi1);
            vertices.push_back(pi3);
            vertices.push_back(pi3);
            vertices.push_back(pi2);
            vertices.push_back(pi0);
        }
        else if (i == n - 1) {
            glm::vec2 pj = points[i-1].getPosition();
            glm::vec2 pi = points[i].getPosition();
            glm::vec2 normal_ji = orthogonal(pj, pi, CounterClockwise);
            float offset_ji = glm::distance(pj, pi) / 2.0;
            glm::vec2 direction_ji = glm::normalize(pj - pi);

            glm::vec2 pi0 = pi + (normal_ji * (w)) + (direction_ji * offset_ji);
            glm::vec2 pi1 = pi - (normal_ji * (w)) + (direction_ji * offset_ji);
            glm::vec2 pi2 = pi + (normal_ji * (w));
            glm::vec2 pi3 = pi - (normal_ji * (w));

            vertices.push_back(pi0);
            vertices.push_back(pi1);
            vertices.push_back(pi3);
            vertices.push_back(pi3);
            vertices.push_back(pi2);
            vertices.push_back(pi0);

        }
        else {

            glm::vec2 pj = points[i-1].getPosition();
            glm::vec2 pi = points[i].getPosition();
            glm::vec2 pk = points[i+1].getPosition();
            glm::vec2 normal_ji = orthogonal(pj, pi, CounterClockwise);
            glm::vec2 normal_ik = orthogonal(pi, pk, CounterClockwise);
            float offset_ij = glm::distance(pj, pi) / 2.0;
            float offset_ik = glm::distance(pi, pk) / 2.0;
            glm::vec2 direction_ij = glm::normalize(pj - pi);
            glm::vec2 direction_ik = glm::normalize(pk - pi);

            glm::vec2 pi0 = pi + (normal_ji * (w)) + (direction_ij * offset_ij);
            glm::vec2 pi1 = pi - (normal_ji * (w)) + (direction_ij * offset_ij);
            glm::vec2 pi2 = pi + (normal_ik * (w)) + (direction_ik * offset_ik);
            glm::vec2 pi3 = pi - (normal_ik * (w)) + (direction_ik * offset_ik);

            vertices.push_back(pi0);
            vertices.push_back(pi1);
            vertices.push_back(pi3);
            vertices.push_back(pi3);
            vertices.push_back(pi2);
            vertices.push_back(pi0);
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
    JoinStyle joinStyle = pen.getJoinStyle();

    // int phase = 0;
    // int segments = 5;
    // int blanks = 3;

    // int j = 0;
    // int c = 0;
    // for (int i = 0; i < n; i ++) {
    //     OutlinePoint& point = points[i];

    //     if (c == 0) {
    //         j = i;
    //     }

    //     if (!point.isBreakPoint()) {
    //         c ++;
    //     }

    //     if (c == steps) {
    //         printf("from:%d to:%d\n", j, i+1);
    //         std::vector<glm::vec2> frames = createOutlineFrames(outline, j, i+1, weight, joinStyle);
    //         vertices.insert(vertices.end(), frames.begin(), frames.end());
    //         c = 0;
    //     }

    // }
    // fflush(stdout);


    for (int i = 0; i < n; i ++) {
        std::vector<glm::vec2> frame =
            createOutlineFrame(outline, i, weight, joinStyle);
        vertices.insert(vertices.end(), frame.begin(), frame.end());
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
