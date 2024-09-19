#version 440

layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform uniformBlock {
    mat4 view;
    mat4 projection;
    vec4 color_;
};

void main(void)
{
    fragColor = color_;
}
