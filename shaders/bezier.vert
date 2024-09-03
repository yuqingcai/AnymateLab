#version 440

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec4 modelColumn0;
layout(location = 3) in vec4 modelColumn1;
layout(location = 4) in vec4 modelColumn2;
layout(location = 5) in vec4 modelColumn3;

layout(std140, binding = 0) uniform uniformBlock {
    mat4 view;
    mat4 projection;
    vec2 resolution;  // 屏幕分辨率
    vec2 p0;          // 贝塞尔曲线的第一个控制点
    vec2 P1;          // 贝塞尔曲线的第二个控制点
    vec2 p2;          // 贝塞尔曲线的第三个控制点
};

layout(location = 0) out vec3 v_color;
layout(location = 1) out vec4 fragCoord;

void main()
{
    v_color = color;
    mat4 model = mat4(modelColumn0, modelColumn1, modelColumn2, modelColumn3);
    gl_Position = projection * view * model * position;
    fragCoord  = model * position;
}
