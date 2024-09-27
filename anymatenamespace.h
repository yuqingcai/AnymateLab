#ifndef ANYMATENAMESPACE_H
#define ANYMATENAMESPACE_H

namespace Anymate {

enum PenStyle {
    NoPen,
    SolidLine,
    DashLine,
    DotLine,
    DashDotLine,
    DashDotDotLine,
};

enum CapStyle {
    FlatCap,
    SquareCap,
    RoundCap,
};

enum JoinStyle {
    NoJoin,
    BevelJoin,
    MiterJoin,
    RoundJoin,
};

enum FillStyle {
    NoFill,
    SolidFill,
    LinearGradientFill,
    RadialGradientFill
};

enum VertexType {
    ShapeVertex,
    BorderVertex,
};

const float borderVerticesZWeight = 0.0;
const float shapeVerticesZWeight = -0.5;
const float outlineSegmentPrecision = 13;
const float defaultPenWidth = 1;

}


#endif // ANYMATENAMESPACE_H
