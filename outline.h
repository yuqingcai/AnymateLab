#ifndef OUTLINE_H
#define OUTLINE_H

#include <vector>
#include <glm/glm.hpp>

namespace Vangoh {


class OutlinePoint
{
public:
    OutlinePoint(glm::vec2& position);
    virtual ~OutlinePoint();
    bool isCuspPoint();
    void setCuspPoint(bool flag);
    bool isBreakPoint();
    void setBreakPoint(bool flag);
    glm::vec2& getPosition();

protected:
    glm::vec2 _position;
    bool _isCuspPoint;
    bool _isBreakPoint;

};

class Outline
{
public:
    Outline();
    virtual ~ Outline();
    void reset();
    void appendPosition(glm::vec2& position);
    std::vector<OutlinePoint>& getPoints();
    bool isClosedPath();
    void print();
protected:
    std::vector<OutlinePoint> _points;
};


}


#endif // OUTLINE_H
