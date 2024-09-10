#version 440

layout(location = 0) in vec4 localCoord;

layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform uniformBlock {
    mat4 view;
    mat4 projection;
    int count;
    vec2 p[];
};

void main(void)
{
    // 曲线厚度
    float thickness = 0.01;
    // 采样 t 的值来生成三次贝塞尔曲线

    if (distance(localCoord.xy, p[0]) < 0.02){
        fragColor = vec4(1.0, 1.0, 0.0, 1.0);
        return;
    }

    if (distance(localCoord.xy, p[1]) < 0.02){
        fragColor = vec4(1.0, 1.0, 0.0, 1.0);
        return;
    }

    if (distance(localCoord.xy, p[2]) < 0.02){
        fragColor = vec4(1.0, 1.0, 0.0, 1.0);
        return;
    }

    if (distance(localCoord.xy, p[3]) < 0.02){
        fragColor = vec4(1.0, 1.0, 0.0, 1.0);
        return;
    }

    for (float t = 0.0; t <= 1.0; t += 0.001) {
        // 计算三次贝塞尔曲线的点
        vec2 B = (1.0 - t) * (1.0 - t) * (1.0 - t) * p[0] +
                 3.0 * (1.0 - t) * (1.0 - t) * t * p[1] +
                 3.0 * (1.0 - t) * t * t * p[2] +
                 t * t * t * p[3];
        // 判断当前像素是否接近曲线
        if (distance(localCoord.xy, B) < thickness) {
            fragColor = vec4(1.0, 0.0, 0.0, 1.0);
            return;
        }
    }

    // else {
    //     fragColor = vec4(0.5, 0.5, 0.5, 1.0);
    // }
}
