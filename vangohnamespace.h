#ifndef VANGOHNAMESPACE_H
#define VANGOHNAMESPACE_H

namespace Vangoh {

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
const float outlineSegmentPrecision = 1;
const float defaultPenWidth = 1;

}


#endif // VANGOHNAMESPACE_H
