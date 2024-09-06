#version 440

layout(location = 0) in vec4 localCoord;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform uniformBlock {
    mat4 view;
    mat4 projection;
};

layout(binding = 1) uniform sampler2D tex;

void main(void)
{
    vec4 c = texture(tex, fragTexCoord);
    fragColor = vec4(c.rgb,  c.a);
}
