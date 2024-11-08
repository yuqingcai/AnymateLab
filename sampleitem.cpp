#include "sampleitem.h"
#include <QtGui/qevent.h>
#include <cstdio>

size_t align(size_t offset, size_t alignment) {
    return (offset + alignment - 1) & ~(alignment - 1);
}

QShader getShader(const QString &name)
{
    QFile f(name);
    return f.open(QIODevice::ReadOnly) ?
               QShader::fromSerialized(f.readAll()) : QShader();
}


SampleItem::SampleItem()
{
    setFocusPolicy(Qt::ClickFocus);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(ItemAcceptsInputMethod, true);
}

SampleItem:: ~ SampleItem()
{

}


void SampleItem::hoverMoveEvent(QHoverEvent *event)
{
    if (_spaceButtonDown) {

        QPointF currentPos = event->position();
        QPointF lastPos = event->oldPosF();
        QPointF offset = currentPos - lastPos;

        _focus.setX(_focus.x()-offset.x());
        _focus.setY(_focus.y()+offset.y());

        // qDebug() << m_focus << m_mosePosition0 << event->position();
    }
    return QQuickRhiItem::hoverMoveEvent(event);
}

void SampleItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!_leftButtonDown) {
            _leftButtonDown = true;
            // qDebug("m_leftButtonDown true");
        }
    }
    return QQuickRhiItem::mousePressEvent(event);
}

void SampleItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (_leftButtonDown) {
            _leftButtonDown = false;
            // qDebug("m_leftButtonDown false");
        }
    }
    return QQuickRhiItem::mouseReleaseEvent(event);
}

void SampleItem::wheelEvent(QWheelEvent *event)
{
    // qDebug() << "Mouse wheel delta: " << event->angleDelta();
    if (_zoomable) {
        if (event->angleDelta().y() > 0) {
            _zoom += 10.0;
        }
        else if (event->angleDelta().y() < 0) {
            if (_zoom - 10.0 >= 10.0)
                _zoom -= 10.0;
        }
        qDebug("zoom: %.1f", _zoom);
    }
    return QQuickRhiItem::wheelEvent(event);
}


void SampleItem::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up) {
        _orthoY -= 100.0;
    }
    if (event->key() == Qt::Key_Down) {
        _orthoY += 100.0;
    }
    if (event->key() == Qt::Key_Left) {
        _orthoX += 100.0;
    }
    if (event->key() == Qt::Key_Right) {
        _orthoX -= 100.0;
    }
    if (event->key() == Qt::Key_Space) {
        if (!_spaceButtonDown) {
            _spaceButtonDown = true;
            // qDebug("m_spaceButtonDown true");
        }
    }
    if (event->key() == Qt::Key_Alt) {
        _zoomable = true;
    }
    return QQuickRhiItem::keyPressEvent(event);
}

void SampleItem::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        if (_spaceButtonDown) {
            _spaceButtonDown = false;
            // qDebug("m_spaceButtonDown false");
        }
    }

    if (event->key() == Qt::Key_Alt) {
        _zoomable = false;
    }

    return QQuickRhiItem::keyReleaseEvent(event);
}

float SampleItem::angle() const
{
    return _angle;
}

void SampleItem::setAngle(float angle)
{
    if (_angle == angle)
        return;

    _angle = angle;
    Q_EMIT angleChanged();
    update();
}

float SampleItem::scale() const
{
    return _scale;
}

void SampleItem::setScale(float scale)
{
    if (_scale == scale)
        return;

    _scale = scale;
    Q_EMIT scaleChanged();
    update();
}

float SampleItem::getOrthoX()
{
    return _orthoX;
}

float SampleItem::getOrthoY()
{
    return _orthoY;
}

float SampleItem::getZoom()
{
    return _zoom;
}

QPointF& SampleItem::getFocus()
{
    return _focus;
}


void SampleItem::renderToTexture()
{
    qDebug("render!");

    std::unique_ptr<QRhi> rhi;

#if defined(Q_OS_WIN)
    QRhiD3D11InitParams params;
    params.enableDebugLayer = true;
    rhi.reset(QRhi::create(QRhi::D3D11, &params));
#elif defined(Q_OS_MACOS)
    QRhiMetalInitParams params;
    rhi.reset(QRhi::create(QRhi::Metal, &params));
#else
    QRhiVulkanInitParams params;
    params.inst = vulkanInstance();
    params.window = this;
    rhi.reset(QRhi::create(QRhi::Vulkan, &params));
#endif
    if (rhi)
        qDebug() << rhi->backendName() << rhi->driverInfo();
    else
        qFatal("Failed to initialize RHI");

    float rotation = 0.0f;
    float opacity = 1.0f;
    int opacityDir = 1;

    std::unique_ptr<QRhiTexture> tex(rhi->newTexture(QRhiTexture::RGBA8,
                                                     QSize(800, 600),
                                                     1,
                                                     QRhiTexture::RenderTarget | QRhiTexture::UsedAsTransferSource));
    tex->create();

    std::unique_ptr<QRhiTextureRenderTarget> rt(
        rhi->newTextureRenderTarget({ tex.get() }));
    std::unique_ptr<QRhiRenderPassDescriptor> rp(rt->newCompatibleRenderPassDescriptor());

    rt->setRenderPassDescriptor(rp.get());
    rt->create();

    QMatrix4x4 viewProjection = rhi->clipSpaceCorrMatrix();
    viewProjection.perspective(45.0f, 1280 / 720.f, 0.01f, 1000.0f);
    viewProjection.translate(0, 0, -4);

    static float vertexData[] = {
        // Y up, CCW
        0.0f,   0.5f,     1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,     0.0f, 1.0f, 0.0f,
        0.5f,  -0.5f,     0.0f, 0.0f, 1.0f,
    };

    std::unique_ptr<QRhiBuffer> vbuf(rhi->newBuffer(QRhiBuffer::Immutable,
                                                    QRhiBuffer::VertexBuffer,
                                                    sizeof(vertexData)));
    vbuf->create();

    std::unique_ptr<QRhiBuffer> ubuf(rhi->newBuffer(QRhiBuffer::Dynamic,
                                                    QRhiBuffer::UniformBuffer,
                                                    64 + 4));
    ubuf->create();

    std::unique_ptr<QRhiShaderResourceBindings> srb(rhi->newShaderResourceBindings());
    srb->setBindings({
        QRhiShaderResourceBinding::uniformBuffer(0,
                                                 QRhiShaderResourceBinding::VertexStage | QRhiShaderResourceBinding::FragmentStage,
                                                 ubuf.get())
    });
    srb->create();

    std::unique_ptr<QRhiGraphicsPipeline> ps(rhi->newGraphicsPipeline());
    QRhiGraphicsPipeline::TargetBlend premulAlphaBlend;
    premulAlphaBlend.enable = true;
    ps->setTargetBlends({ premulAlphaBlend });
    static auto getShader = [](const QString &name) {
        QFile f(name);
        return f.open(QIODevice::ReadOnly) ? QShader::fromSerialized(f.readAll()) : QShader();
    };
    ps->setShaderStages({
        { QRhiShaderStage::Vertex, getShader(QLatin1String(":/AnymateLab/shaders/color.vert.qsb")) },
        { QRhiShaderStage::Fragment, getShader(QLatin1String(":/AnymateLab/shaders/color.frag.qsb")) }
    });


    QRhiVertexInputLayout inputLayout;
    inputLayout.setBindings({
        { 5 * sizeof(float) }
    });
    inputLayout.setAttributes({
        { 0, 0, QRhiVertexInputAttribute::Float2, 0 },
        { 0, 1, QRhiVertexInputAttribute::Float3, 2 * sizeof(float) }
    });
    ps->setVertexInputLayout(inputLayout);
    ps->setShaderResourceBindings(srb.get());
    ps->setRenderPassDescriptor(rp.get());
    ps->create();

    QRhiCommandBuffer *cb;
    for (int frame = 0; frame < 20; ++frame) {
        rhi->beginOffscreenFrame(&cb);

        QRhiResourceUpdateBatch *u = rhi->nextResourceUpdateBatch();
        if (frame == 0)
            u->uploadStaticBuffer(vbuf.get(), vertexData);

        QMatrix4x4 mvp = viewProjection;
        mvp.rotate(rotation, 0, 1, 0);
        u->updateDynamicBuffer(ubuf.get(), 0, 64, mvp.constData());
        rotation += 5.0f;

        u->updateDynamicBuffer(ubuf.get(), 64, 4, &opacity);
        opacity += opacityDir * 0.2f;
        if (opacity < 0.0f || opacity > 1.0f) {
            opacityDir *= -1;
            opacity = qBound(0.0f, opacity, 1.0f);
        }

        const QColor clearColor = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f);
        cb->beginPass(rt.get(), clearColor, { 1.0f, 0 }, u);
        cb->setGraphicsPipeline(ps.get());
        cb->setViewport({ 0, 0, 1280, 720 });
        cb->setShaderResources();
        const QRhiCommandBuffer::VertexInput vbufBinding(vbuf.get(), 0);
        cb->setVertexInput(0, 1, &vbufBinding);
        cb->draw(3);
        QRhiReadbackResult readbackResult;
        u = rhi->nextResourceUpdateBatch();
        u->readBackTexture({ tex.get() }, &readbackResult);
        cb->endPass(u);

        rhi->endOffscreenFrame();

        QImage image(reinterpret_cast<const uchar *>(readbackResult.data.constData()),
                     readbackResult.pixelSize.width(),
                     readbackResult.pixelSize.height(),
                     QImage::Format_RGBA8888_Premultiplied);
        if (rhi->isYUpInFramebuffer())
            image = image.mirrored();
        image.save(QString::asprintf("frame%d.png", frame));
    }

}
