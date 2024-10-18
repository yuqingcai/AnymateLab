#include "arrangements2d.h"
#include <cstdio>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>


typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Arr_segment_traits_2<K> Traits_2;
typedef CGAL::Arrangement_2<Traits_2> Arrangement_2;
typedef Traits_2::Segment_2 Segment_2;

typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Delaunay_mesh_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, Tds> CDT;
typedef CDT::Vertex_handle Vertex_handle;

Arrangements2DRenderer::Arrangements2DRenderer()
{
}


Arrangements2DRenderer::~Arrangements2DRenderer()
{
}

int Arrangements2DRenderer::createBuffer0()
{
    if (!_rhi)
        return -1;

    size_t verticesSize = sizeof(float) * 1024 * 1024 * 4;

    _vectexBuffer0.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                         QRhiBuffer::VertexBuffer,
                                         verticesSize));
    _vectexBuffer0->create();

    _indexBuffer0.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                        QRhiBuffer::IndexBuffer,
                                        verticesSize));
    _indexBuffer0->create();

    _modelBuffer0.reset(_rhi->newBuffer(QRhiBuffer::Immutable,
                                        QRhiBuffer::VertexBuffer,
                                        sizeof(glm::mat4) * 1024));
    _modelBuffer0->create();

    return 0;
}

int Arrangements2DRenderer::createShaderResourceBinding0()
{

    if (!_rhi)
        return -1;

    // view and projection uniform
    int blockSize = sizeof(glm::mat4) +
                    sizeof(glm::mat4) +
                    sizeof(glm::vec4)
        ;
    int bufferSize = _rhi->ubufAligned(blockSize);
    _uniformBuffer0.reset(_rhi->newBuffer(QRhiBuffer::Dynamic,
                                          QRhiBuffer::UniformBuffer,
                                          bufferSize));
    _uniformBuffer0->create();

    _srb0.reset(_rhi->newShaderResourceBindings());
    _srb0->setBindings({
        QRhiShaderResourceBinding::uniformBufferWithDynamicOffset(
            0,
            m_shaderResourceStages,
            _uniformBuffer0.get(),
            bufferSize),
    });
    _srb0->create();
    return 0;
}

int Arrangements2DRenderer::createPipline0()
{
    if (!_rhi || !_srb0)
        return -1;

    _pipeline0.reset(_rhi->newGraphicsPipeline());

    _pipeline0->setShaderStages({
        {
            QRhiShaderStage::Vertex,
            getShader(QLatin1String(":/AnymateLab/shaders/curve.vert.qsb"))
        },
        {
            QRhiShaderStage::Fragment,
            getShader(QLatin1String(":/AnymateLab/shaders/curve.frag.qsb"))
        }
    });

    QRhiVertexInputLayout inputLayout;
    inputLayout.setBindings(
        {
         { 3 * sizeof(float), QRhiVertexInputBinding::PerVertex },
         { 64, QRhiVertexInputBinding::PerInstance },
         });

    inputLayout.setAttributes(
        {
         // binding0
         { 0, 0, QRhiVertexInputAttribute::Float3, 0                 },
         // binding1
         { 1, 1, QRhiVertexInputAttribute::Float4, 0 },
         { 1, 2, QRhiVertexInputAttribute::Float4, 4 * sizeof(float) },
         { 1, 3, QRhiVertexInputAttribute::Float4, 8 * sizeof(float) },
         { 1, 4, QRhiVertexInputAttribute::Float4, 12 * sizeof(float) },
         });


    _pipeline0->setVertexInputLayout(inputLayout);
    _pipeline0->setSampleCount(_sampleCount);
    _pipeline0->setShaderResourceBindings(_srb0.get());
    _pipeline0->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    _pipeline0->setDepthTest(true);
    _pipeline0->setDepthWrite(true);

    // _pipeline0->setTopology(QRhiGraphicsPipeline::Lines);
    // _pipeline0->setTopology(QRhiGraphicsPipeline::LineStrip);
    _pipeline0->setTopology(QRhiGraphicsPipeline::Triangles);
    QList<QRhiGraphicsPipeline::TargetBlend> targetBlends(1);
    targetBlends[0].enable = true;
    targetBlends[0].srcColor = QRhiGraphicsPipeline::SrcAlpha;
    targetBlends[0].dstColor = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opColor = QRhiGraphicsPipeline::Add;
    targetBlends[0].srcAlpha = QRhiGraphicsPipeline::One;
    targetBlends[0].dstAlpha = QRhiGraphicsPipeline::OneMinusSrcAlpha;
    targetBlends[0].opAlpha = QRhiGraphicsPipeline::Add;
    _pipeline0->setTargetBlends(targetBlends.begin(), targetBlends.end());
    _pipeline0->create();
    return 0;
}

void Arrangements2DRenderer::initialize(QRhiCommandBuffer *cb)
{

    if (_rhi != rhi()) {
        _rhi = rhi();
        createBuffer0();
        _pipeline0.reset();
    }

    if (_sampleCount != renderTarget()->sampleCount()) {
        _sampleCount = renderTarget()->sampleCount();
        _pipeline0.reset();
    }

    QRhiTexture *finalTex = _sampleCount > 1 ? resolveTexture() : colorTexture();
    if (_textureFormat != finalTex->format()) {
        _textureFormat = finalTex->format();
        _pipeline0.reset();
    }

    if (!_srb0) {
        createShaderResourceBinding0();
    }

    if (!_pipeline0) {
        createPipline0();
    }
}

void Arrangements2DRenderer::render(QRhiCommandBuffer *cb)
{
    const QSize outputSize = renderTarget()->pixelSize();
    _projection = _rhi->clipSpaceCorrMatrix();

    _projection.perspective(45.0f,
                            outputSize.width() / (float) outputSize.height(),
                            10.0f,
                            1000000.0f);

    QVector3D cameraPos(_focus.rx(), _focus.ry(), _zoom);
    QVector3D cameraTarget(_focus.rx(), _focus.ry(), 0.0f);
    QVector3D cameraUp(0.0f, 1.0f, 0.0f);
    _view.setToIdentity();
    _view.lookAt(cameraPos, cameraTarget, cameraUp);

    QRhiResourceUpdateBatch *batch = _rhi->nextResourceUpdateBatch();

    const QColor clearColor = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f);
    cb->beginPass(renderTarget(), clearColor, { 1.0f, 0 }, batch);
    cb->setViewport(QRhiViewport(0, 0, outputSize.width(), outputSize.height()));


    size_t offset = 0;
    glm::vec4 lineColor = glm::vec4(1.0, 0.0, 0.0, 1.0);

    offset = align(0, STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(_uniformBuffer0.get(),
                               offset,
                               sizeof(glm::mat4),
                               _view.constData());
    offset = align(sizeof(glm::mat4), STD140_ALIGN_MAT4);
    batch->updateDynamicBuffer(_uniformBuffer0.get(),
                               offset,
                               sizeof(glm::mat4),
                               _projection.constData());
    offset = align(sizeof(glm::mat4) + sizeof(glm::mat4),
                   STD140_ALIGN_VEC4);
    batch->updateDynamicBuffer(_uniformBuffer0.get(),
                               offset,
                               sizeof(glm::vec4),
                               &lineColor);

    batch->uploadStaticBuffer(_vectexBuffer0.get(),
                              0,
                              _vertices.size() * sizeof(float),
                              _vertices.data());



    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, qDegreesToRadians(_angle), glm::vec3(0.0f, 1.0f, 0.0f));
    // model = glm::scale(model, glm::vec3(_scale, _scale, _scale));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    batch->uploadStaticBuffer(_modelBuffer0.get(), 0, sizeof(glm::mat4), &model);

    cb->resourceUpdate(batch);

    // draw indexed
    cb->setGraphicsPipeline(_pipeline0.get());
    cb->setShaderResources(_srb0.get());

    const QRhiCommandBuffer::VertexInput inputBindings[] = {
        { _vectexBuffer0.get(), 0 },
        { _modelBuffer0.get(), 0 }
    };
    cb->setVertexInput(0, 2, inputBindings, _indexBuffer0.get(), 0, QRhiCommandBuffer::IndexUInt32);
    // cb->drawIndexed(sizeof(indices));
    cb->draw(_vertices.size()/3);

    cb->endPass();
}

void Arrangements2DRenderer::synchronize(QQuickRhiItem *rhiItem)
{
    Arrangements2D *item = static_cast<Arrangements2D *>(rhiItem);
    if (item->angle() != _angle)
        _angle = item->angle();

    if (item->scale() != _scale)
        _scale = item->scale();

    _orthoX = item->getOrthoX();
    _orthoY = item->getOrthoY();
    _zoom = item->getZoom();
    _focus = item->getFocus();
    _vertices = item->getVertices();
}

// void print_incident_halfedges(Arrangement_2::Vertex_const_handle v) {
//     if (v->is_isolated()) {
//         std::cout << "The vertex (" << v->point() << ") is isolated\n";
//         return;
//     }
//     Arrangement_2::Halfedge_around_vertex_const_circulator first, curr;
//     first = curr = v->incident_halfedges();
//     std::cout << "The neighbors of the vertex (" << v->point() << ") are:";
//     do std::cout << " (" << curr->source()->point() << ")";
//     while (++curr != first);
//     std::cout << std::endl;
// }

void print_ccb(Arrangement_2::Ccb_halfedge_const_circulator circ) {
    Arrangement_2::Ccb_halfedge_const_circulator curr = circ;
    std::cout << "(" << curr->source()->point() << ")";
    do {
        Arrangement_2::Halfedge_const_handle e = curr;
        std::cout << " [" << e->curve() << "] "
                  << "(" << e->target()->point() << ")";
    } while (++curr != circ);
    std::cout << std::endl;
}

void print_face(Arrangement_2::Face_const_handle f) {
    // Print the outer boundary.
    if (f->is_unbounded()) {
    }
    else {
        std::cout << "Outer boundary: ";
        print_ccb(f->outer_ccb());
    }

    // Print the boundary of each of the holes.
    size_t index = 1;
    for (auto hi = f->holes_begin(); hi != f->holes_end(); ++hi) {
        std::cout << " Hole #" << index++ << ": ";
        print_ccb(*hi);
    }

    // Print the isolated vertices.
    index = 1;
    for (auto iv = f->isolated_vertices_begin();
         iv != f->isolated_vertices_end(); ++iv)
    {
        std::cout << " Isolated vertex #" << index++ << ": "
                  << "(" << iv->point() << ")\n";
    }
}

void print_arrangement(const Arrangement_2& arr) {
    // Print the arrangement vertices.
    std::cout << arr.number_of_vertices() << " vertices:\n";
    for (auto vit = arr.vertices_begin(); vit != arr.vertices_end(); ++vit) {
        std::cout << "(" << vit->point() << ")";
        if (vit->is_isolated()) std::cout << " - Isolated.\n";
        else std::cout << " - degree " << vit->degree() << std::endl;
    }

    // Print the arrangement edges.
    std::cout << arr.number_of_edges() << " edges:\n";
    for (auto eit = arr.edges_begin(); eit != arr.edges_end(); ++eit)
        std::cout << "[" << eit->curve() << "]\n";

    // Print the arrangement faces.
    std::cout << arr.number_of_faces() << " faces:\n";
    for (auto fit = arr.faces_begin(); fit != arr.faces_end(); ++fit)
        print_face(fit);
}


void pushFaceLinesToVertices (
    const Arrangement_2& arr,
    std::vector<float>& vectices
)
{
    std::cout << arr.number_of_faces() << " faces:" << std::endl;
    int i = 0;
    for (auto fit = arr.faces_begin(); fit != arr.faces_end(); ++fit) {
        if (!fit->is_unbounded()) {

            std::cout << "#" << i << std::endl;

            Arrangement_2::Ccb_halfedge_const_circulator curr = fit->outer_ccb();
            do {
                vectices.push_back(CGAL::to_double(curr->source()->point().x()));
                vectices.push_back(CGAL::to_double(curr->source()->point().y()));
                vectices.push_back(0.0);

                vectices.push_back(CGAL::to_double(curr->target()->point().x()));
                vectices.push_back(CGAL::to_double(curr->target()->point().y()));
                vectices.push_back(0.0);

                std::cout << curr->source()->point()
                          << " "
                          << curr->target()->point()
                          << std::endl;

            } while (++curr != fit->outer_ccb());

            i ++;
        }
    }
}

void pushPointToVetices(CDT::Point_2& p, std::vector<float>& vertices)
{
    double x = CGAL::to_double(p.x());
    double y = CGAL::to_double(p.y());
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(0.0);
}

void pushTriangulationVertices(const Arrangement_2& arr,
                               std::vector<float>& vertices)
{
    // 遍历face
    for (auto fit = arr.faces_begin(); fit != arr.faces_end(); ++fit) {
        if (!fit->is_unbounded()) {

            CDT cdt;

            // 获取 face 的顶点
            std::vector<Vertex_handle> vs;
            Arrangement_2::Ccb_halfedge_const_circulator curr = fit->outer_ccb();
            do {
                double x = CGAL::to_double(curr->source()->point().x());
                double y = CGAL::to_double(curr->source()->point().y());
                vs.push_back(cdt.insert(CDT::Point_2(x, y)));
            } while (++ curr != fit->outer_ccb());

            // 用顶点构成一个“简单多边形”
            int n = vs.size();
            for (int i = 0; i < n; i ++) {
                Vertex_handle vi = vs[i];
                Vertex_handle vj = vs[i+1];
                if (i == n - 1) {
                    vj = vs[0];
                }
                cdt.insert_constraint(vi, vj);
            }
            // “简单多变形”三角测量，形成三角形序列
            std::list<CDT::Point_2> list_of_seeds;
            CGAL::refine_Delaunay_mesh_2(cdt, CGAL::parameters::seeds(list_of_seeds));

            // 获取三角形序列的顶点
            for (auto fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); fit ++) {
                 if(fit->is_in_domain()) {

                    CDT::Point_2 p0 = fit->vertex(0)->point();
                    CDT::Point_2 p1 = fit->vertex(1)->point();
                    CDT::Point_2 p2 = fit->vertex(2)->point();

                    pushPointToVetices(p0, vertices);
                    pushPointToVetices(p1, vertices);
                    pushPointToVetices(p2, vertices);

                    // pushPointToVetices(p0, vertices);
                    // pushPointToVetices(p1, vertices);
                    // pushPointToVetices(p1, vertices);
                    // pushPointToVetices(p2, vertices);
                    // pushPointToVetices(p2, vertices);
                    // pushPointToVetices(p0, vertices);

                }

            }


        }
    }
}

Arrangements2D::Arrangements2D()
{
    // Arrangements2D 的作用是把一个多边形分解成多个“简单多边形”。
    //
    // 定义多边形的顶点，由顶点生成线段，将线段插入到 arr，arr会生成这由个多边形生成的
    // 面(face，每一个face都是一个“简单多边形”，“简单多边形”是指边不相交的多边形， 可
    // 用于进行三角测量）。遍历这些face获取其顶点，由这些顶点便可构成“简单多边形”，便可
    // 进行德劳内三角测量从而得到构成该多变形的三角形组合进行图形渲染。

    Arrangement_2 arr;
    Traits_2::Point_2 p0(0.012, 0.120),
        p1(0.230, 20.123430),
        p2(50.450, 20.6540),
        p3(50.023, 40.02342),
        p4(0.023, 40.02342),
        p5(10.023, 50.02342),
        p6(30.023, 30.02342);

    Segment_2 cv[] = {
        Segment_2(p0, p1),
        Segment_2(p1, p2),
        Segment_2(p2, p3),
        Segment_2(p3, p4),
        Segment_2(p4, p5),
        Segment_2(p5, p6),
        Segment_2(p6, p0),
    };
    CGAL::insert(arr, &cv[0], &cv[sizeof(cv)/sizeof(Segment_2)]);
    // pushFaceLinesToVertices(arr, _vertices);
    pushTriangulationVertices(arr, _vertices);

}

std::vector<float>& Arrangements2D::getVertices()
{
    return _vertices;
}

Arrangements2D:: ~ Arrangements2D()
{

}

QQuickRhiItemRenderer* Arrangements2D::createRenderer()
{
    return new Arrangements2DRenderer();
}

