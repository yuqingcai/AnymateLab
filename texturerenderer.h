#ifndef TEXTURERENDERER_H
#define TEXTURERENDERER_H

#include <QObject>

class TextureRenderer: public QObject
{
public:
    TextureRenderer();
    Q_INVOKABLE void render();
};

#endif // TEXTURERENDERER_H
