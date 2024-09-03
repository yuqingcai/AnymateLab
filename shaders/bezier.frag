#version 440

layout(location = 0) in vec3 v_color;
layout(location = 1) in vec4 fragCoord;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform uniformBlock {
    mat4 view;
    mat4 projection;
    vec2 resolution;  // 屏幕分辨率
    vec2 cp0;          // 贝塞尔曲线的第一个控制点
    vec2 cp1;          // 贝塞尔曲线的第二个控制点
    vec2 cp2;          // 贝塞尔曲线的第三个控制点
};

void main()
{
    if (fragCoord.x > 50.0 && fragCoord.x < 60.0) {
        fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    else {
        // discard;
        fragColor = vec4(v_color, 1.0);
    }
}
