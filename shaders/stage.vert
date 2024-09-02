#version 440

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec4 aMatCol0;
layout(location = 3) in vec4 aMatCol1;
layout(location = 4) in vec4 aMatCol2;
layout(location = 5) in vec4 aMatCol3;

layout(std140, binding = 0) uniform viewProjectionBlock {
    mat4 view;
    mat4 projection;
};

layout(location = 0) out vec3 v_color;

void main()
{
    v_color = color;
    mat4 model = mat4(aMatCol0, aMatCol1, aMatCol2, aMatCol3);
    gl_Position = projection * view * model * position;

    // gl_Position = projection * view * position;
}
