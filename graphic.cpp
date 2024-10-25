#include "graphic.h"
#include "tools.h"
#include <cmath>
#include <cstring>
#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <variant>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Triangulation_conformer_2.h>
#include <CGAL/lloyd_optimize_mesh_2.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Intersection_traits_2.h>
#include <boost/optional.hpp>
#include <CGAL/intersections.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Delaunay_mesh_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, Tds> CDT;
typedef CDT::Vertex_handle Vertex_handle;
typedef CGAL::Arr_segment_traits_2<K> Traits_2;
typedef CGAL::Arrangement_2<Traits_2> Arrangement_2;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Ray_2<K> Ray;
typedef CGAL::Segment_2<K> Segment_2;

namespace Vangoh {

GraphicObject::GraphicObject()
{
}


Shape::Shape()
    :
    GraphicObject()
{

}


Shape::~ Shape()
{
    _outlineVertices.clear();
    _guideLineVertices.clear();
    _shapeVertices.clear();
}

std::vector<glm::vec2>& Shape::getOutlineVertices()
{
    return _outlineVertices;
}

std::vector<glm::vec2>& Shape::getGuideLineVertices()
{
    return _guideLineVertices;
}

std::vector<glm::vec2>& Shape::getShapeVertices()
{
    return _shapeVertices;
}

void Shape::setPen(Pen& pen)
{
    _pen = pen;
}

void Shape::createVertices()
{
    createOutline();
    createOutlineVertices();
    createShapeVertices();
}

void Shape::createOutlineVertices()
{
    _outlineVertices = _outline.createFrames(_pen.getWidth(),
                                     _pen.getJoinStyle(),
                                     _pen.getCapStyle());

    _guideLineVertices = _outline.createGuideLines(_pen.getWidth(),
                                         _pen.getJoinStyle(),
                                         _pen.getCapStyle());
}


std::vector<Segment_2> segmentsInPolygon(Polygon_2& polygon)
{
    std::vector<Segment_2> segments;
    for (auto it = polygon.edges_begin(); it != polygon.edges_end(); it ++) {
        segments.push_back(*it);
    }
    return segments;
}

bool segmentIsCommoned(Segment_2& segment, std::vector<Polygon_2>& polygons)
{
    int n = 0;
    for (auto& polygon : polygons) {
        std::vector<Segment_2> segments = segmentsInPolygon(polygon);
        for (auto& target : segments) {
            if (segment.source() == target.source() && segment.target() == target.target() ||
                segment.source() == target.target() && segment.target() == target.source()) {
                n ++;
            }
        }
    }
    return n > 1;
}

bool pointIsVertexInPolygon(K::Point_2& p, Polygon_2& polygon)
{
    for (auto& target : polygon.vertices()) {
        if (p == target)
            return true;
    }
    return false;
}


K::Point_2 getPointFromPolygon(Polygon_2& polygon)
{
    K::Point_2 p0 = polygon.vertex(0);
    K::Point_2 p1 = polygon.vertex(1);
    K::Point_2 p2 = polygon.vertex(2);
    K::Vector_2 v = K::Vector_2(p0 - p1) + K::Vector_2(p2 - p1);

    // normalized
    double length = std::sqrt(CGAL::to_double(v.squared_length()));
    K::Vector_2 norm = v / length;

    return p1 + (norm * 0.1);
}

Ray rayFromPolygon(Polygon_2& polygon, double delta)
{    
    K::Point_2 source = getPointFromPolygon(polygon);
    K::Point_2 directon = K::Point_2(source.x()+0.1, source.y()+delta);
    return Ray(source, directon);
}

bool rayIntersectWithPolygon(Ray& ray, Polygon_2& polygon)
{
    std::vector<Segment_2> segments = segmentsInPolygon(polygon);
    for (Segment_2& segment : segments) {
        std::optional<std::variant<K::Point_2, Segment_2>>
            result = CGAL::intersection(ray, segment);

        std::cout << segment << " ";

        if (result) {
            std::cout << "intersection:" << std::endl;

            if (std::holds_alternative<K::Point_2>(*result)) {
                K::Point_2 point = std::get<K::Point_2>(*result);
                std::cout << "Point:" << point << std::endl;
            }
            else if (std::holds_alternative<Segment_2>(*result)) {
                Segment_2 segment = std::get<Segment_2>(*result);
                std::cout << "Segment:" << segment << std::endl;
            }
        }
        else {
            std::cout << "No intersection." << std::endl;
        }
    }

    return false;
}

bool polygonCompleteContain(Polygon_2& container, Polygon_2& target)
{
    for (auto& p : target.vertices()) {
        auto result = CGAL::bounded_side_2(container.vertices_begin(),
                                           container.vertices_end(),
                                           p, K());
        if (result == CGAL::ON_BOUNDED_SIDE || result == CGAL::ON_BOUNDARY) {
        }
        else {
            return false;
        }
    }
    return true;
}

std::vector<glm::vec2> triangulation(Polygon_2& polygon,
                                     std::list<K::Point_2>& seeds)
{
    std::vector<glm::vec2> vertices;

    CDT cdt;
    std::vector<Vertex_handle> vhs;
    for (const K::Point_2& point : polygon.vertices()) {
        vhs.push_back(cdt.insert(point));
    }

    int n = vhs.size();
    for (int i = 0; i < n; i ++) {
        Vertex_handle vi = vhs[i];
        Vertex_handle vj = vhs[i+1];
        if (i == n - 1) {
            vj = vhs[0];
        }
        cdt.insert_constraint(vi, vj);
    }

    CGAL::refine_Delaunay_mesh_2(cdt, CGAL::parameters::seeds(seeds));

    for (auto fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); fit ++) {
        if(fit->is_in_domain()) {
            K::Point_2 p0 = fit->vertex(0)->point();
            K::Point_2 p1 = fit->vertex(1)->point();
            K::Point_2 p2 = fit->vertex(2)->point();

            double x, y;

            x = CGAL::to_double(p0.x());
            y = CGAL::to_double(p0.y());
            vertices.push_back(glm::vec2(x, y));

            x = CGAL::to_double(p1.x());
            y = CGAL::to_double(p1.y());
            vertices.push_back(glm::vec2(x, y));

            // x = CGAL::to_double(p1.x());
            // y = CGAL::to_double(p1.y());
            // vertices.push_back(glm::vec2(x, y));

            x = CGAL::to_double(p2.x());
            y = CGAL::to_double(p2.y());
            vertices.push_back(glm::vec2(x, y));

            // x = CGAL::to_double(p2.x());
            // y = CGAL::to_double(p2.y());
            // vertices.push_back(glm::vec2(x, y));

            // x = CGAL::to_double(p0.x());
            // y = CGAL::to_double(p0.y());
            // vertices.push_back(glm::vec2(x, y));
        }
    }

    return vertices;
}

K::Point_2 fineTuning(K::Point_2 p0, K::Point_2 p1)
{
    K::Vector_2 v = K::Vector_2(p1 - p0);
    double length = std::sqrt(CGAL::to_double(v.squared_length()));
    K::Vector_2 norm = v / length;
    K::Vector_2 orth = K::Vector_2(-norm.y(), norm.x());
    return p1 + orth*0.001;
}

bool segmentsOverlap(Segment_2& s0, Segment_2& s1)
{
    if (CGAL::collinear(s0.source(), s0.target(), s1.source()) &&
        CGAL::collinear(s0.source(), s0.target(), s1.target()))
    {
        return CGAL::do_intersect(s0, s1);
    }

    return false;
}

void regularPoints(std::vector<K::Point_2>& points)
{
    std::vector<Segment_2> segments;
    int n = points.size();
    bool tunningP0 = false;

    for (int i = 0; i < n - 1; i ++) {
        int j = i + 1;
        K::Point_2& pi = points[i];
        K::Point_2& pj = points[j];

        Segment_2 test(pi, pj);

        for (Segment_2& segment : segments) {

            if (segmentsOverlap(test, segment)) {
                std::cout << "segment overlap: ";

                std::cout << test.source() << "," << test.target() << "->"
                          << segment.source() << "," << segment.target() << std::endl;

                // first and last points has same position in a closed path,
                // tunning p0
                if (j == n - 1 && pj == points[0]) {
                    tunningP0 = true;
                }

                K::Point_2 tunning = fineTuning(points[i], points[j]);

                std::cout << "fineTuning:" << points[j] << "->" << tunning << std::endl;

                points[j] = tunning;

                if (tunningP0) {
                    std::cout << "tuning point[0]:" << points[0] << "->" << tunning << std::endl;
                    points[0] = points[j];
                }
            }
        }
        segments.push_back(Segment_2(pi, pj));
    }

}

void Shape::createShapeVertices()
{
    _shapeVertices.clear();

    std::vector<OutlinePoint2D>& outlinePoints = _outline.getPoints();

    Arrangement_2 arr;
    std::vector<K::Point_2> points;
    for (OutlinePoint2D& p : outlinePoints) {
        glm::vec2& pos = p.getPosition();
        points.push_back(K::Point_2(pos.x, pos.y));
    }
    regularPoints(points);

    std::vector<Traits_2::Point_2> traitPoints;
    for (K::Point_2& p : points) {
        traitPoints.push_back(Traits_2::Point_2(p.x(), p.y()));
    }


    // put segment to arrangement
    std::vector<Traits_2::Segment_2> segments;
    for (int i = 0; i < traitPoints.size() - 1; i ++) {
        segments.push_back(Traits_2::Segment_2(traitPoints[i], traitPoints[i+1]));
    }
    CGAL::insert(arr, segments.begin(), segments.end());

    std::vector<Polygon_2> faces;
    // create faces (a face is a simple polygon)
    for (auto fit = arr.faces_begin(); fit != arr.faces_end(); fit ++) {
        if (!fit->is_unbounded()) {
            Polygon_2 face;
            Arrangement_2::Ccb_halfedge_circulator curr = fit->outer_ccb();
            do {
                K::Point_2 source = curr->source()->point();
                K::Point_2 target = curr->target()->point();
                face.push_back(source);

                curr ++;
            } while (curr != fit->outer_ccb());

            faces.push_back(face);
        }
    }


    std::vector<Polygon_2> odds;
    std::vector<Polygon_2> events;
    // pushPolygonVertices(faces[2], _shapeVertices);

    int i = 0;
    int n = faces.size();

    double delta = 0.0;
    while (i < n) {
        bool testSuccess = true;
        Ray ray = rayFromPolygon(faces[i], delta);
        float sum = 0.0;

        for (auto& testFace : faces) {

            std::vector<Segment_2> testSegments = segmentsInPolygon(testFace);

            for (Segment_2& testSegment : testSegments) {
                std::optional<std::variant<K::Point_2, Segment_2>>
                    result = CGAL::intersection(ray, testSegment);

                if (result) {

                    if (std::holds_alternative<K::Point_2>(*result)) {

                        K::Point_2 p = std::get<K::Point_2>(*result);

                        if (pointIsVertexInPolygon(p, testFace)) {
                            delta += 0.1;
                            testSuccess = false;
                            break;
                        }
                        else if (segmentIsCommoned(testSegment, faces)) {
                            sum += 0.5;
                        }
                        else {
                            sum += 1.0;
                        }
                    }
                    else if (std::holds_alternative<Segment_2>(*result)) {
                        // Segment_2 segment = std::get<Segment_2>(*result);
                        delta += 0.1;
                        testSuccess = false;
                        break;
                    }
                }
            }

            if (!testSuccess) {
                break;
            }
        }

        if (testSuccess) {
            int c = (int)sum;
            // odd, create face
            if (c & 1) {
                odds.push_back(faces[i]);
            }
            else {
                events.push_back(faces[i]);
            }
            i ++;
            delta = 0.0;
        }
    }

    for (auto& odd : odds) {
        std::list<K::Point_2> seeds;
        for (auto& event : events) {
            K::Point_2 seed = getPointFromPolygon(event);
            seeds.push_back(seed);
        }

        std::vector<glm::vec2> vectices = triangulation(odd, seeds);
        _shapeVertices.insert(_shapeVertices.end(),
                              vectices.begin(), vectices.end());
    }

}

Polygon::Polygon()
{

}

Polygon::Polygon(std::initializer_list<glm::vec2> list)
{
    if (!list.size())
        return;

    assert(list.size() >= 3);

    if (list.size() < 3) {
        fprintf(stderr, "Polygon should at least contains 3 points\n");
        return;
    }

    for (glm::vec2 p : list) {
        _points.push_back(p);
    }

    makeClose();

}

Polygon::Polygon(std::vector<glm::vec2> list)
{
    if (!list.size())
        return;

    assert(list.size() >= 3);

    if (list.size() < 3) {
        fprintf(stderr, "Polygon should at least contains 3 points\n");
        return;
    }

    for (glm::vec2 p : list) {
        _points.push_back(p);
    }

    makeClose();
}

void Polygon::makeClose()
{
    // make sure polygon is closed
    if (_points.back() != _points.front()) {
        _points.push_back(_points.front());
    }
}

Polygon::~ Polygon()
{

}

void Polygon::createOutline()
{
    int n = _points.size();
    _outline.reset();

    glm::vec2 p = _points[0];
    _outline.appendPosition(p);

    int i = 1;
    while (i < n) {
        p = _points[i];
        _outline.appendPosition(p);
        _outline.getPoints().back().setCuspPoint(true);
        i ++;
    }

}

Line::Line(glm::vec2 p0, glm::vec2 p1)
    :
    _endPoint0(p0),
    _endPoint1(p1)
{

}

Line::~Line()
{

}

void Line::createOutline()
{
    _outline.reset();
    float precision = 20.0;
    // outline just need 2D points;
    glm::vec2 p0 = _endPoint0;
    glm::vec2 p1 = _endPoint1;
    float distance = glm::distance(p0, p1);
    glm::vec2 p = p0;

    _outline.appendPosition(p0);

    while (true) {
        p = pointRelateTo(p, p1, precision);
        float distanceP = glm::distance(p0, p);
        if (distanceP < distance) {
            _outline.appendPosition(p);
        }
        else if (fabs(distanceP - distance) < FLT_EPSILON ||
                distanceP > distance) {
            _outline.appendPosition(p1);
            break;
        }
    }

}


Squircles::Squircles(glm::vec2 center, float ra, float rb, float n)
    :
    _center(center),
    _ra(ra),
    _rb(rb),
    _n(n)
{

}

Squircles:: ~ Squircles()
{

}

void Squircles::createOutline()
{
    std::vector<glm::vec2> points;
    float step = 1.0;
    for (int i = 0; i < 180; ++i) {
        double theta = i * M_PI / 180.0;
        double x = _ra * pow(cos(theta), 2.0 / _n);
        double y = _rb * pow(sin(theta), 2.0 / _n);
        points.emplace_back(x, y);
    }

    for (int i = 0; i < points.size(); i ++) {
        glm::vec p = glm::vec2(points[i].x, points[i].y);
        _outline.appendPosition(p);
    }

    for (int i = points.size() - 1; i >= 0; i --) {
        glm::vec p = glm::vec2(-points[i].x, points[i].y);
        _outline.appendPosition(p);
    }

    for (int i = 0; i < points.size(); i ++) {
        glm::vec p = glm::vec2(-points[i].x, -points[i].y);
        _outline.appendPosition(p);
    }

    for (int i = points.size() - 1; i >= 0; i --) {
        glm::vec p = glm::vec2(points[i].x, -points[i].y);
        _outline.appendPosition(p);
    }
}

BezierCurve::BezierCurve(glm::vec2 p0, glm::vec2 p1,
            glm::vec2 p2, glm::vec2 p3)
    :
    _point0(p0),
    _point1(p1),
    _point2(p2),
    _point3(p3)
{

}

BezierCurve::~ BezierCurve()
{

}


glm::vec2 BezierCurve::interp(float t)
{
    float u = 1 - t;
    return _point0 * (u * u * u) +
           _point1 * (3 * u * u * t) +
           _point2 * (3 * u * t * t) +
           _point3 * (t * t * t);
}


glm::vec2 BezierCurve::derivative(float t)
{
    float u = 1 - t;
    return (_point1 - _point0) * (3 * u * u) +
           (_point2 - _point1) * (6 * u * t) +
           (_point3 - _point2) * (3 * t * t);
}

float BezierCurve::arcLength(float t0, float t1, int segments) {
    float length = 0.0f;
    float dt = (t1 - t0) / segments;

    for (int i = 0; i < segments; ++i) {
        float t_start = t0 + i * dt;
        float t_end = t0 + (i + 1) * dt;
        glm::vec2 p_start = derivative(t_start);
        glm::vec2 p_end = derivative(t_end);
        length += 0.5f * (glm::length(p_start) + glm::length(p_end)) * dt;
    }

    return length;
}

float BezierCurve::findTForArcLength(float targetLength, int segments) {
    float t_low = 0.0f;
    float t_high = 1.0f;
    float currentLength = 0.0f;

    while (t_high - t_low > 1e-5f) {
        float t_mid = (t_low + t_high) / 2.0f;
        currentLength = arcLength(0, t_mid, segments);

        if (currentLength < targetLength) {
            t_low = t_mid;
        } else {
            t_high = t_mid;
        }
    }

    return (t_low + t_high) / 2.0f;
}

void BezierCurve::createOutline()
{
    // float totalLength = arcLength(0.0, 1.0);
    // int steps = static_cast<int>(std::ceil(totalLength))/2;
    // for (int i = 0; i <= steps; i ++) {
    //     float targetLength = (totalLength / steps) * i;
    //     float t = findTForArcLength(targetLength, 10);
    //     glm::vec2 p = interp(t);
    //     _outline.appendPosition(p);
    // }

    int samples = 100;
    for (int i = 0; i <= samples; ++i) {
        float t = static_cast<float>(i) / samples;
        glm::vec2 p = interp(t);
        _outline.appendPosition(p);
    }
}


}
