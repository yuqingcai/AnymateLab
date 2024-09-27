#ifndef COLOR_H
#define COLOR_H

#include <vector>
#include <glm/glm.hpp>

namespace Anymate {
class LinearGradient
{
public:
    LinearGradient();
    LinearGradient(std::initializer_list<glm::vec3> colors);
    ~LinearGradient();

private:
    std::vector<glm::vec4> _colors;
};

class RadialGradient
{
public:
    RadialGradient();
    RadialGradient(std::initializer_list<glm::vec3> colors);
    ~RadialGradient();

private:
    std::vector<glm::vec4> _colors;
};
}


#endif // COLOR_H
