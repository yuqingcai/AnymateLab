#ifndef PEN_H
#define PEN_H

#include "anymatenamespace.h"
#include <vector>
#include <glm/glm.hpp>

namespace Anymate {

class Pen
{
public:
    Pen(PenStyle style=SolidLine, CapStyle cap=FlatCap,
        JoinStyle join=BevelJoin, float width=defaultPenWidth);
    virtual ~Pen();

    void setWidth(float width);
    void setPenStyle(PenStyle style);
    void setCapStyle(CapStyle style);
    void setJoinStyle(JoinStyle style);

    float getWidth();
    PenStyle getPenStyle();
    CapStyle getCapStyle();
    JoinStyle getJoinStyle();

protected:
    PenStyle _penStyle;
    CapStyle _capStyle;
    JoinStyle _joinStyle;
    float _width;
    std::vector<glm::vec3> _points;

};
}


#endif // PEN_H
