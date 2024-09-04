#version 440

layout(location = 0) in vec4 fragCoord;

layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform uniformBlock {
    mat4 view;
    mat4 projection;
    int shapeType;
    float radius;
    vec2 center;
    float thickness;
    float smoothness;
};

void main()
{
    vec2 p = fragCoord.xy - center;
    float dist = length(p);

    float outerEdge = radius + thickness * 0.5;
    float innerEdge = radius - thickness * 0.5;

    float edge0 = innerEdge + smoothness;
    float edge1 = outerEdge - smoothness;

    if (abs(fragCoord.y - center.y) > 80 &&
            abs(fragCoord.y - center.y) < 81 &&
            abs(fragCoord.x - center.x) <= 81) {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else if (abs(fragCoord.x - center.x) > 80 &&
             abs(fragCoord.x - center.x) < 81 &&
             abs(fragCoord.y - center.y) <= 81) {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else if (dist >= innerEdge && dist <= outerEdge) {
        float alpha = 1.0;
        if (dist < edge0) {
            alpha = smoothstep(innerEdge, edge0, dist);
        }
        else if (dist > edge1) {
            alpha = smoothstep(outerEdge, edge1, dist);
        }

        fragColor = vec4(1.0, 1.0, 0.0, alpha);
    }
    else {
        discard;
    }
}
