#ifndef OUTLINE2D_H
#define OUTLINE2D_H

#include <vector>
#include <glm/glm.hpp>
#include "vangohnamespace.h"
#include "tools.h"

namespace Vangoh {


class OutlinePoint2D
{
public:
    OutlinePoint2D(glm::vec2& position);
    virtual ~OutlinePoint2D();
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

class Outline2D
{
public:
    Outline2D();
    virtual ~ Outline2D();
    void reset();
    void appendPosition(glm::vec2& position);
    std::vector<OutlinePoint2D>& getPoints();
    bool isClosedPath();
    void print();


    std::vector<glm::vec2> createFrames(float weight,
                                        JoinStyle joinStyle,
                                        CapStyle capStyle);
    std::vector<glm::vec2> createGuideLines(float weight,
                                            JoinStyle joinStyle,
                                        CapStyle capStyle);
protected:
    std::vector<glm::vec2> createFrame(int i,
                                       float weight,
                                       JoinStyle joinStyle,
                                       CapStyle capStyle);

    std::vector<glm::vec2> createGuideLine(int i,
                                           float weight,
                                           JoinStyle joinStyle,
                                           CapStyle capStyle);

    bool verticesOfFrame(int i,
                         float weight,
                         glm::vec2* pi0,
                         glm::vec2* pi1,
                         glm::vec2* pi2,
                         glm::vec2* pi3);

    bool verticesOfCuspFrame(int i,
                             float weight,
                             glm::vec2* pij0,
                             glm::vec2* pij1,
                             glm::vec2* pj0,
                             glm::vec2* pj1,
                             glm::vec2* pik0,
                             glm::vec2* pik1,
                             glm::vec2* pk0,
                             glm::vec2* pk1);

    std::vector<glm::vec2> verticesOfGap(JoinStyle joinStyle,
                                         glm::vec2& pi,
                                         glm::vec2& pij0,
                                         glm::vec2& pij1,
                                         glm::vec2& pj0,
                                         glm::vec2& pj1,
                                         glm::vec2& pik0,
                                         glm::vec2& pik1,
                                         glm::vec2& pk0,
                                         glm::vec2& pk1);

    std::vector<glm::vec2> verticesOfCap(CapPosition position,
                                         CapStyle capStyle,
                                         glm::vec2& pi0,
                                         glm::vec2& pi1,
                                         glm::vec2& pi2,
                                         glm::vec2& pi3);
    std::vector<OutlinePoint2D> _points;

};


class Outline2DMorph {

public:
    Outline2DMorph();
    ~Outline2DMorph();
    void setSource(Outline2D& source);
    void setTarget(Outline2D& target);
    void morphing(float t);

private:


    Outline2D _source;
    Outline2D _target;
};

}


#endif // OUTLINE2D_H
