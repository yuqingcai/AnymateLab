#include "vertexgenerator.h"
#include "tools.h"
#include <stdio.h>

namespace Vangoh {

VertexGenerator::VertexGenerator()
{

}

VertexGenerator::~VertexGenerator()
{

}

bool VertexGenerator::verticesOfOutlineMesh(Outline& outline,
                                            int i,
                                            float height,
                                            float length,
                                            glm::vec3* pi0,
                                            glm::vec3* pi1,
                                            glm::vec3* pi2,
                                            glm::vec3* pi3,
                                            glm::vec3* pi4,
                                            glm::vec3* pi5,
                                            glm::vec3* pi6,
                                            glm::vec3* pi7,
                                            glm::vec3* pi8,
                                            glm::vec3* pi9,
                                            glm::vec3* pi10,
                                            glm::vec3* pi11)
{
    std::vector<OutlinePoint>& points = outline.getPoints();
    int n = points.size();
    float h = height * 0.5;
    float l = length * 0.5;

    if (!n || n < 2 || i < 0 || i >= n) {
        return false;
    }

    if (i == 0) {
        int k = i + 1;
        glm::vec3 pi = points[i].getPosition();
        glm::vec3 pk = points[k].getPosition();

        glm::vec3 normal_xy_ik = orthogonalXY(pi, pk);
        glm::vec3 normal_xz_ik = orthogonalXZ(pi, pk);

        float offset_ik = glm::distance(pi, pk) * 0.5;
        glm::vec3 direction_ik = glm::normalize(pk - pi);

        *pi0 = pi + (normal_xy_ik * h);
        *pi1 = pi - (normal_xy_ik * h);
        *pi2 = pi + (normal_xy_ik * h) + (direction_ik * offset_ik);
        *pi3 = pi - (normal_xy_ik * h) + (direction_ik * offset_ik);

        *pi4 = *pi0 + (normal_xz_ik * l);
        *pi5 = *pi1 + (normal_xz_ik * l);
        *pi6 = *pi2 + (normal_xz_ik * l);
        *pi7 = *pi3 + (normal_xz_ik * l);

        *pi8 = *pi0 - (normal_xz_ik * l);
        *pi9 = *pi1 - (normal_xz_ik * l);
        *pi10 = *pi2 - (normal_xz_ik * l);
        *pi11 = *pi3 - (normal_xz_ik * l);
    }
    else if (i == n - 1) {
        int j = i - 1;
        glm::vec3 pj = points[j].getPosition();
        glm::vec3 pi = points[i].getPosition();

        glm::vec3 normal_xy_ji = orthogonalXY(pj, pi);
        glm::vec3 normal_xz_ji = orthogonalXZ(pj, pi);

        float offset_ji = glm::distance(pj, pi) * 0.5;
        glm::vec3 direction_ij = glm::normalize(pj - pi);

        *pi0 = pi + (normal_xy_ji * h) + (direction_ij * offset_ji);
        *pi1 = pi - (normal_xy_ji * h) + (direction_ij * offset_ji);
        *pi2 = pi + (normal_xy_ji * h);
        *pi3 = pi - (normal_xy_ji * h);

        *pi4 = *pi0 + (normal_xz_ji * l);
        *pi5 = *pi1 + (normal_xz_ji * l);
        *pi6 = *pi2 + (normal_xz_ji * l);
        *pi7 = *pi3 + (normal_xz_ji * l);

        *pi8 = *pi0 - (normal_xz_ji * l);
        *pi9 = *pi1 - (normal_xz_ji * l);
        *pi10 = *pi2 - (normal_xz_ji * l);
        *pi11 = *pi3 - (normal_xz_ji * l);
    }
    else {
        int j = i - 1;
        int k = i + 1;
        glm::vec3 pj = points[j].getPosition();
        glm::vec3 pi = points[i].getPosition();
        glm::vec3 pk = points[k].getPosition();

        glm::vec3 normal_xy_ji = orthogonalXY(pj, pi);
        glm::vec3 normal_xz_ji = orthogonalXZ(pj, pi);
        float offset_ij = glm::distance(pj, pi)  * 0.5;
        glm::vec3 direction_ij = glm::normalize(pj - pi);

        glm::vec3 normal_xy_ik = orthogonalXY(pi, pk);
        glm::vec3 normal_xz_ik = orthogonalXZ(pi, pk);
        float offset_ik = glm::distance(pi, pk) * 0.5;
        glm::vec3 direction_ik = glm::normalize(pk - pi);

        *pi0 = pi + (normal_xy_ji * h) + (direction_ij * offset_ij);
        *pi1 = pi - (normal_xy_ji * h) + (direction_ij * offset_ij);
        *pi2 = pi + (normal_xy_ik * h) + (direction_ik * offset_ik);
        *pi3 = pi - (normal_xy_ik * h) + (direction_ik * offset_ik);

        *pi4 = *pi0 + (normal_xz_ji * l);
        *pi5 = *pi1 + (normal_xz_ji * l);
        *pi6 = *pi2 + (normal_xz_ik * l);
        *pi7 = *pi3 + (normal_xz_ik * l);

        *pi8 = *pi0 - (normal_xz_ji * l);
        *pi9 = *pi1 - (normal_xz_ji * l);
        *pi10 = *pi2 - (normal_xz_ik * l);
        *pi11 = *pi3 - (normal_xz_ik * l);
    }

    return true;
}

void printPoint(glm::vec3& p)
{
    printf("%.1f %.1f %.1f\n", p.x, p.y, p.z);
}

std::vector<glm::vec3> VertexGenerator::createOutlineMesh(Outline& outline,
                                         int i,
                                         float height,
                                         float length,
                                         JoinStyle joinStyle,
                                         CapStyle capStyle)
{
    std::vector<glm::vec3> vertices;

    std::vector<OutlinePoint>& points = outline.getPoints();
    int n = points.size();

    if (!n || n < 2 || i < 0 || i >= n) {
        return vertices;
    }

    if (points[i].isCuspPoint()) {
        return vertices;
        // glm::vec3 pij0, pij1, pj0, pj1, pik0, pik1, pk0, pk1;
        // std::vector<glm::vec2> gaps;
        // glm::vec2 pi = points[i].getPosition();

        // if (verticesOfOutlineCuspFrame(outline, i, weight,
        //                                &pij0, &pij1, &pj0, &pj1,
        //                                &pik0, &pik1, &pk0, &pk1))
        // {
        //     if (i == n - 1) {
        //         vertices.push_back(pj0);
        //         vertices.push_back(pj1);
        //         vertices.push_back(pij1);
        //         vertices.push_back(pij1);
        //         vertices.push_back(pij0);
        //         vertices.push_back(pj0);

        //         if (outline.isClosedPath()) {
        //             // gap connection
        //             gaps = verticesOfGap(joinStyle, pi,pij0, pij1, pj0, pj1,
        //                                  pik0, pik1, pk0, pk1);
        //             vertices.insert(vertices.end(), gaps.begin(), gaps.end());
        //         }
        //     }
        //     else {
        //         vertices.push_back(pj0);
        //         vertices.push_back(pj1);
        //         vertices.push_back(pij1);
        //         vertices.push_back(pij1);
        //         vertices.push_back(pij0);
        //         vertices.push_back(pj0);

        //         vertices.push_back(pik0);
        //         vertices.push_back(pik1);
        //         vertices.push_back(pk1);
        //         vertices.push_back(pk1);
        //         vertices.push_back(pk0);
        //         vertices.push_back(pik0);

        //         // gap connection
        //         gaps = verticesOfGap(joinStyle, pi,pij0, pij1, pj0, pj1,
        //                              pik0, pik1, pk0, pk1);
        //         vertices.insert(vertices.end(), gaps.begin(), gaps.end());
        //    }
        // }
    }
    else {
        glm::vec3 pi0, pi1, pi2, pi3, pi4, pi5, pi6, pi7, pi8, pi9, pi10, pi11;
        std::vector<glm::vec3> caps;

        if (verticesOfOutlineMesh(outline, i, height, length,
                                  &pi0, &pi1, &pi2, &pi3,
                                  &pi4, &pi5, &pi6, &pi7,
                                  &pi8, &pi9, &pi10, &pi11)) {


            // plane 0
            vertices.push_back(pi4);
            vertices.push_back(pi5);
            vertices.push_back(pi6);
            vertices.push_back(pi6);
            vertices.push_back(pi5);
            vertices.push_back(pi7);

            // plane 1
            vertices.push_back(pi5);
            vertices.push_back(pi9);
            vertices.push_back(pi7);
            vertices.push_back(pi7);
            vertices.push_back(pi9);
            vertices.push_back(pi11);

            // plane 2
            vertices.push_back(pi10);
            vertices.push_back(pi11);
            vertices.push_back(pi8);
            vertices.push_back(pi8);
            vertices.push_back(pi11);
            vertices.push_back(pi9);

            // plane 3
            vertices.push_back(pi8);
            vertices.push_back(pi4);
            vertices.push_back(pi10);
            vertices.push_back(pi10);
            vertices.push_back(pi4);
            vertices.push_back(pi6);


            // if (!outline.isClosedPath()) {
            //     if (i == 0) {
            //         caps = verticesOfCap(HeadCap, capStyle, pi0, pi1, pi2, pi3);
            //     }
            //     else if (i == n-1) {
            //         caps = verticesOfCap(TailCap, capStyle, pi0, pi1, pi2, pi3);
            //     }
            //     vertices.insert(vertices.end(), caps.begin(), caps.end());
            // }

        }

    }



    return vertices;
}

std::vector<glm::vec3>
VertexGenerator::createOutlineGuideLineUnit(Outline& outline,
                                            int i,
                                            float weight)
{

    std::vector<glm::vec3> vertices;

    std::vector<OutlinePoint>& points = outline.getPoints();
    int n = points.size();
    float w = weight * 0.5;

    if (!n || n < 2 || i < 0 || i >= n) {
        return vertices;
    }

    glm::vec3 pi = points[i].getPosition();
    glm::vec3 pj;
    glm::vec3 pk;
    glm::vec3 pi0, pi1, pi2, pi3;

    if (i == 0) {
        pk = points[i+1].getPosition();
        vertices.push_back(pi);
        vertices.push_back(pk);
    }
    else if (i == n - 1) {
        pj = points[i-1].getPosition();

        glm::vec3 normal_ji = orthogonalXY(pj, pi);
        float offset_ji = glm::distance(pj, pi) / 2.0;
        glm::vec3 direction_ij = glm::normalize(pj - pi);

        pi0 = pi + (normal_ji * (w));
        pi1 = pi - (normal_ji * (w));
        pi2 = pi0 + (direction_ij * offset_ji);
        pi3 = pi1 + (direction_ij * offset_ji);

        vertices.push_back(pi2);
        vertices.push_back(pi);
        vertices.push_back(pi);
        vertices.push_back(pi3);

    }
    else {
        pk = points[i+1].getPosition();
        vertices.push_back(pi);
        vertices.push_back(pk);

        pj = points[i-1].getPosition();
        glm::vec3 normal_xy_ji = orthogonalXY(pj, pi);
        float offset_ji = glm::distance(pj, pi) / 2.0;
        glm::vec3 direction_ij = glm::normalize(pj - pi);

        pi0 = pi + (normal_xy_ji * (w));
        pi1 = pi - (normal_xy_ji * (w));
        pi2 = pi0 + (direction_ij * offset_ji);
        pi3 = pi1 + (direction_ij * offset_ji);

        vertices.push_back(pi2);
        vertices.push_back(pi);
        vertices.push_back(pi);
        vertices.push_back(pi3);
    }

    return vertices;
}




std::vector<glm::vec3> VertexGenerator::createOutlineMeshes(Pen& pen, Outline& outline)
{
    std::vector<glm::vec3> vertices;

    std::vector<OutlinePoint>& points = outline.getPoints();
    int n = points.size();
    if (!n || n < 2) {
        return vertices;
    }

    for (int i = 0; i < n; i ++) {
        std::vector<glm::vec3> mesh = createOutlineMesh(
            outline, i, pen.getWidth(), pen.getWidth(),
            pen.getJoinStyle(), pen.getCapStyle());
        vertices.insert(vertices.end(), mesh.begin(), mesh.end());
    }

    return vertices;
}


std::vector<glm::vec3> VertexGenerator::createOutlineGuideLines(Pen& pen, Outline& outline)
{
    std::vector<glm::vec3> vertices;

    std::vector<OutlinePoint>& points = outline.getPoints();
    int n = points.size();

    if (!n || n < 2) {
        return vertices;
    }

    for (int i = 0; i < n; i ++) {
        std::vector<glm::vec3> lines =
            createOutlineGuideLineUnit(outline, i, pen.getWidth());
        vertices.insert(vertices.end(), lines.begin(), lines.end());
    }
    return vertices;
}



}
