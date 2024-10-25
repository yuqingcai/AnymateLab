#ifndef SAMPLEITEM_H
#define SAMPLEITEM_H

#include <QFile>
#include <QQuickRhiItem>
#include <rhi/qrhi.h>

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

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernal;
typedef CGAL::Triangulation_vertex_base_2<Kernal> Vb;
typedef CGAL::Delaunay_mesh_face_base_2<Kernal> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Constrained_Delaunay_triangulation_2<Kernal, Tds> CDT;
typedef CDT::Vertex_handle Vertex_handle;
typedef CDT::Point_2 Point_2;
typedef Kernal::Vector_2 Vector_2;
typedef CGAL::Polygon_2<Kernal> Polygon_2;


#define STD140_ALIGN_INT   4
#define STD140_ALIGN_FLOAT 4
#define STD140_ALIGN_VEC2  8
#define STD140_ALIGN_VEC3 16
#define STD140_ALIGN_VEC4 16
#define STD140_ALIGN_MAT4 64

size_t align(size_t offset, size_t alignment);
QShader getShader(const QString &name);

class SampleItem: public QQuickRhiItem
{
    Q_OBJECT
    Q_PROPERTY(float angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)

public:
    SampleItem();
    virtual ~ SampleItem();
    float angle() const;
    void setAngle(float angle);
    float scale() const;
    void setScale(float scale);
    float getOrthoX();
    float getOrthoY();
    float getZoom();
    QPointF& getFocus();

protected:
    void hoverMoveEvent(QHoverEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void renderToTexture();

    float _angle = 0.0f;
    float _scale = 1.0f;
    float _orthoX = 0.0f;
    float _orthoY = 0.0f;
    float _zoom = 500.0f;
    QPointF _focus = {0.0f, 0.0f};
    bool _spaceButtonDown = false;
    bool _leftButtonDown = false;

    QPoint _mosePosition0 = {0, 0};
    QPoint _mosePosition1 = {0, 0};

Q_SIGNALS:
    void angleChanged();
    void scaleChanged();

private:

};

#endif // SAMPLEITEM_H
