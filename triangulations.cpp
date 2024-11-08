#include "triangulations.h"
#include "features.h"
#include <QFile>
#include <QtGui/qevent.h>
#include <cstdio>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

// #include <freetype/freetype.h>
#include <iostream>

// void traverseOutline(const FT_Outline& outline, std::vector<float>& vertices) {
//     for (int i = 0; i < outline.n_contours; ++i) {
//         int contour_start = (i == 0) ? 0 : outline.contours[i - 1] + 1;
//         int contour_end = outline.contours[i];

//         for (int j = contour_start; j <= contour_end; ++j) {
//             vertices.push_back(outline.points[j].x);
//             vertices.push_back(outline.points[j].y);
//             vertices.push_back(0.0);

//             if (j == contour_end) {
//                 vertices.push_back(outline.points[contour_start].x);
//                 vertices.push_back(outline.points[contour_start].y);
//                 vertices.push_back(0.0);
//             }
//             else {
//                 vertices.push_back(outline.points[j+1].x);
//                 vertices.push_back(outline.points[j+1].y);
//                 vertices.push_back(0.0);
//             }

//         }
//     }
// }

// int GetFontOutline(FT_ULong c, std::vector<float>& vertices) {
//     // 初始化 FreeType 库
//     FT_Library library;
//     if (FT_Init_FreeType(&library)) {
//         std::cerr << "Could not initialize FreeType library.\n";
//         return 1;
//     }

//     // 加载字体文件
//     FT_Face face;
//     if (FT_New_Face(library, "/Volumes/Volume0/Users/yuqingcai/Library/Fonts/狮尾福腿黑体-Bold.ttf", 0, &face)) {
//         std::cerr << "Could not load font file.\n";
//         FT_Done_FreeType(library);
//         return 1;
//     }

//     // 设置字符大小 (单位: 1/64 点)
//     FT_Set_Char_Size(face, 0, 24 * 64, 10, 10);  // 设置字体大小为 16pt，DPI 为 300

//     // 加载字符
//     FT_UInt glyph_index = FT_Get_Char_Index(face, c);
//     if (FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_BITMAP)) {
//         std::cerr << "Could not load glyph.\n";
//         FT_Done_Face(face);
//         FT_Done_FreeType(library);
//         return 1;
//     }

//     // 检查是否存在轮廓
//     if (face->glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
//         std::cerr << "Glyph does not contain an outline.\n";
//         FT_Done_Face(face);
//         FT_Done_FreeType(library);
//         return 1;
//     }

//     // 获取轮廓数据
//     FT_Outline outline = face->glyph->outline;
//     traverseOutline(outline, vertices);

//     // 清理
//     FT_Done_Face(face);
//     FT_Done_FreeType(library);

//     return 0;
// }

TriangulationsRenderer::TriangulationsRenderer()
{
    _model = new glm::mat4(1.0f);
}

TriangulationsRenderer::~TriangulationsRenderer()
{
    if (_model) {
        delete [] _model;
    }
}

int TriangulationsRenderer::createBuffer0()
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

int TriangulationsRenderer::createShaderResourceBinding0()
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
int TriangulationsRenderer::createPipline0()
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

    _pipeline0->setTopology(QRhiGraphicsPipeline::Lines);
    // _pipeline0->setTopology(QRhiGraphicsPipeline::LineStrip);
    // _pipeline0->setTopology(QRhiGraphicsPipeline::Triangles);
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

void TriangulationsRenderer::initialize(QRhiCommandBuffer *cb)
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

void TriangulationsRenderer::pushPointToVetices(Point_2& p, std::vector<float>& vectices)
{
    double x = CGAL::to_double(p.x());
    double y = CGAL::to_double(p.y());
    vectices.push_back(x);
    vectices.push_back(y);
    vectices.push_back(0.0);
}

void TriangulationsRenderer::render(QRhiCommandBuffer *cb)
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



    CDT cdt;
    // 绘制一个圆，并添加 seeds
    //
    // double cx = 0.0, cy = 0.0, r = 50.0;
    // double step = 1.0;
    // std::vector<Vertex_handle> vs;
    // for (int i = 0; i < 360; i += step) {
    //     double theta = i * M_PI / 180.0;
    //     double x = cx + r * cos(theta);
    //     double y = cy + r * sin(theta);
    //     Vertex_handle v = cdt.insert(Point_2(x,y));
    //     vs.push_back(v);
    // }

    // for (int i = 0; i < vs.size(); i ++) {
    //     if (i < vs.size() - 1) {
    //         cdt.insert_constraint(vs[i], vs[i+1]);
    //     }
    //     else {
    //         cdt.insert_constraint(vs[i], vs[0]);
    //     }
    // }

    // Vertex_handle v0 = cdt.insert(Point_2(-10, 10));
    // Vertex_handle v1 = cdt.insert(Point_2(10, 10));
    // Vertex_handle v2 = cdt.insert(Point_2(10, -10));
    // Vertex_handle v3 = cdt.insert(Point_2(-10, -10));

    // cdt.insert_constraint(v0, v1);
    // cdt.insert_constraint(v1, v2);
    // cdt.insert_constraint(v2, v3);
    // cdt.insert_constraint(v3, v0);

    // Vertex_handle v4 = cdt.insert(Point_2(-20, 20));
    // Vertex_handle v5 = cdt.insert(Point_2(20, 20));
    // Vertex_handle v6 = cdt.insert(Point_2(20, -20));
    // Vertex_handle v7 = cdt.insert(Point_2(-20, -20));

    // cdt.insert_constraint(v4, v5);
    // cdt.insert_constraint(v5, v6);
    // cdt.insert_constraint(v6, v7);
    // cdt.insert_constraint(v7, v4);

    Vertex_handle a = cdt.insert(Point_2(0, 0));
    Vertex_handle b = cdt.insert(Point_2(100, 0));
    Vertex_handle c = cdt.insert(Point_2(100, 100));
    Vertex_handle d = cdt.insert(Point_2(0, 100));
    Vertex_handle e = cdt.insert(Point_2(50, 0));
    Vertex_handle f = cdt.insert(Point_2(75, 75));
    Vertex_handle g = cdt.insert(Point_2(25, 75));

    // cdt.insert_constraint(a, b);
    // cdt.insert_constraint(b, c);
    // cdt.insert_constraint(c, d);
    // cdt.insert_constraint(d, a);
    // cdt.insert_constraint(e, f);
    // cdt.insert_constraint(f, g);
    // cdt.insert_constraint(g, e);

    cdt.insert_constraint(a, e);
    cdt.insert_constraint(e, g);
    cdt.insert_constraint(g, f);
    cdt.insert_constraint(f, e);
    cdt.insert_constraint(e, b);
    cdt.insert_constraint(b, c);
    cdt.insert_constraint(c, d);
    cdt.insert_constraint(d, a);

    std::list<Point_2> list_of_seeds;
    list_of_seeds.push_back(Point_2(50, 50));
    CGAL::refine_Delaunay_mesh_2(cdt, CGAL::parameters::seeds(list_of_seeds).seeds_are_in_domain(false));

    std::vector<float> vertices;
    for (auto fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); fit ++) {
        if(fit->is_in_domain()) {

            Point_2 p0 = fit->vertex(0)->point();
            Point_2 p1 = fit->vertex(1)->point();
            Point_2 p2 = fit->vertex(2)->point();

            pushPointToVetices(p0, vertices);
            pushPointToVetices(p1, vertices);

            pushPointToVetices(p1, vertices);
            pushPointToVetices(p2, vertices);

            pushPointToVetices(p2, vertices);
            pushPointToVetices(p0, vertices);
        }
    }
    batch->uploadStaticBuffer(_vectexBuffer0.get(),
                              0,
                              vertices.size() * sizeof(float),
                              vertices.data());

    // 绘制字符曲线
    // GetFontOutline(0x6587, vertices);
    // batch->uploadStaticBuffer(_vectexBuffer0.get(),
    //                           0,
    //                           vertices.size() * sizeof(float),
    //                           vertices.data());

    // std::vector<float> indices;
    // int n = vertices.size()/3;
    // for (int i = 0; i < n; i ++) {
    //     indices.push_back(i);
    //     if (i < n - 1) {
    //         indices.push_back(i + 1);
    //     }
    //     else {
    //         indices.push_back(0);
    //     }
    // }
    // batch->uploadStaticBuffer(_indexBuffer0.get(), 0, indices.size(), indices.data());

    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::rotate(model, qDegreesToRadians(_angle), glm::vec3(0.0f, 1.0f, 0.0f));
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
    cb->draw(vertices.size()/3);

    cb->endPass();
}

void TriangulationsRenderer::synchronize(QQuickRhiItem *rhiItem)
{
    Triangulations *item = static_cast<Triangulations *>(rhiItem);
    if (item->angle() != _angle)
        _angle = item->angle();


    if (item->scale() != _scale)
        _scale = item->scale();

    _orthoX = item->getOrthoX();
    _orthoY = item->getOrthoY();
    _zoom = item->getZoom();
    _focus = item->getFocus();
}

Triangulations::Triangulations()
{
    setFocusPolicy(Qt::ClickFocus);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(ItemAcceptsInputMethod, true);
}

Triangulations:: ~ Triangulations()
{

}

QQuickRhiItemRenderer* Triangulations::createRenderer()
{
    return new TriangulationsRenderer();
}

