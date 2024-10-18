#ifndef ARRANGEMENTS2D_H
#define ARRANGEMENTS2D_H

#include "sampleitem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Triangulation_conformer_2.h>
#include <CGAL/lloyd_optimize_mesh_2.h>


class Arrangements2DRenderer: public QQuickRhiItemRenderer
{
public:
    Arrangements2DRenderer();
    ~Arrangements2DRenderer();
    void initialize(QRhiCommandBuffer *cb) override;
    void synchronize(QQuickRhiItem *item) override;
    void render(QRhiCommandBuffer *cb) override;

private:
    int createBuffer0();
    int createShaderResourceBinding0();
    int createPipline0();

    QRhi *_rhi = nullptr;
    int _sampleCount = 4;
    QRhiTexture::Format _textureFormat = QRhiTexture::RGBA8;

    std::unique_ptr<QRhiGraphicsPipeline> _pipeline0;
    std::unique_ptr<QRhiShaderResourceBindings> _srb0;
    std::unique_ptr<QRhiBuffer> _uniformBuffer0;
    std::unique_ptr<QRhiBuffer> _vectexBuffer0;
    std::unique_ptr<QRhiBuffer> _indexBuffer0;
    std::unique_ptr<QRhiBuffer> _modelBuffer0;

    QMatrix4x4 _view;
    QMatrix4x4 _projection;

    static constexpr auto m_shaderResourceStages =
        QRhiShaderResourceBinding::VertexStage |
        QRhiShaderResourceBinding::GeometryStage |
        QRhiShaderResourceBinding::FragmentStage |
        QRhiShaderResourceBinding::TessellationControlStage |
        QRhiShaderResourceBinding::TessellationEvaluationStage |
        QRhiShaderResourceBinding::ComputeStage;

    float _angle = 0.0f;
    float _scale = 1.0f;
    float _orthoX = 0.0f;
    float _orthoY = 0.0f;
    float _zoom = 1.0f;
    QPointF _focus = {0.0f, 0.0f};
    std::vector<float> _vertices;
};

class Arrangements2D: public SampleItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Arrangements2D)

public:
    Arrangements2D();
    virtual ~ Arrangements2D();
    QQuickRhiItemRenderer *createRenderer() override;
    std::vector<float>& getVertices();

private:
    std::vector<float> _vertices;
};

#endif // ARRANGEMENTS2D_H
