#version 440

layout(location = 0) in vec4 localCoord;

layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform uniformBlock {
    mat4 view;
    mat4 projection;
};

void main(void)
{
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
