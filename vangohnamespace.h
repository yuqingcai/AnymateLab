#ifndef VANGOHNAMESPACE_H
#define VANGOHNAMESPACE_H

#include <glm/glm.hpp>

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

const float outlinePrecision = 1.0;
const float defaultPenWidth = 1;

}


#endif // VANGOHNAMESPACE_H
