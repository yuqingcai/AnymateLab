#include "earclipping.h"
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <tuple>

// 计算叉积，用于判断顶点是凸顶点还是凹顶点
float crossProduct(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C) {
    return (B.x - A.x) * (C.y - B.y) - (B.y - A.y) * (C.x - B.x);
}

// 判断一个顶点是否是凸顶点
bool isConvex(const glm::vec2& prev, const glm::vec2& current, const glm::vec2& next) {
    return crossProduct(prev, current, next) > 0;
}

// 判断三角形 ABC 是否包含点 P（用于耳朵检查）
bool isPointInTriangle(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C, const glm::vec2& P) {
    float areaABC = crossProduct(A, B, C);
    float areaPAB = crossProduct(P, A, B);
    float areaPBC = crossProduct(P, B, C);
    float areaPCA = crossProduct(P, C, A);
    return (areaABC > 0 && areaPAB > 0 && areaPBC > 0 && areaPCA > 0) ||
           (areaABC < 0 && areaPAB < 0 && areaPBC < 0 && areaPCA < 0);
}

// 检查耳朵是否有效，即三角形不与其他顶点相交
bool isEar(const std::vector<glm::vec2>& polygon, int i, int prev, int next) {
    glm::vec2 A = polygon[prev];
    glm::vec2 B = polygon[i];
    glm::vec2 C = polygon[next];

    // 检查多边形中的其他顶点是否在这个三角形内
    for (int j = 0; j < polygon.size(); ++j) {
        if (j != prev && j != i && j != next && isPointInTriangle(A, B, C, polygon[j])) {
            return false;
        }
    }

    return true;
}

// 实现耳切法三角剖分
std::vector<std::tuple<glm::vec2, glm::vec2, glm::vec2>> earClippingTriangulation(std::vector<glm::vec2>& polygon) {
    std::vector<std::tuple<glm::vec2, glm::vec2, glm::vec2>> triangles;

    int n = polygon.size();
    std::vector<int> indices(n);
    for (int i = 0; i < n; ++i) indices[i] = i;

    while (n > 2) {
        bool earFound = false;

        // 遍历所有顶点，寻找耳朵
        for (int i = 0; i < n; ++i) {
            int prev = (i - 1 + n) % n;
            int next = (i + 1) % n;

            glm::vec2 A = polygon[indices[prev]];
            glm::vec2 B = polygon[indices[i]];
            glm::vec2 C = polygon[indices[next]];

            if (isConvex(A, B, C) && isEar(polygon, indices[i], indices[prev], indices[next])) {
                // 找到一个耳朵，记录三角形
                triangles.emplace_back(A, B, C);

                // 从多边形中移除顶点 i
                indices.erase(indices.begin() + i);
                n--;
                earFound = true;
                break;
            }
        }

        if (!earFound) {
            std::cerr << "Error: No ear found, check if the polygon is simple and valid." << std::endl;
            break;
        }
    }

    return triangles;
}
