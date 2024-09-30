#include "pen.h"

namespace Vangoh {

Pen::Pen(PenStyle style, CapStyle cap, JoinStyle join, float width)
    :
    _penStyle(style),
    _capStyle(cap),
    _joinStyle(join),
    _width(width)
{

}

Pen::~Pen()
{

}

void Pen::setWidth(float width)
{
    _width = width;
}


void Pen::setPenStyle(PenStyle style)
{
    _penStyle = style;
}

void Pen::setCapStyle(CapStyle style)
{
    _capStyle = style;
}

void Pen::setJoinStyle(JoinStyle style)
{
    _joinStyle = style;
}

float Pen::getWidth()
{
    return _width;
}

PenStyle Pen::getPenStyle()
{
    return _penStyle;
}

CapStyle Pen::getCapStyle()
{
    return _capStyle;
}

JoinStyle Pen::getJoinStyle()
{
    return _joinStyle;
}

}
