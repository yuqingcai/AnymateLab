#version 440

layout(location = 0) in vec4 localCoord;

layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform uniformBlock {
    mat4 view;
    mat4 projection;
    int shapeType;
    float radius;
    vec2 center;
    float thickness;
    float smoothness;
    float angle;
};

void drawRectangle()
{
    vec4 backgroundColor = vec4(1.0, 1.0, 1.0, 1.0);

    if (abs(localCoord.y - center.y) > 80 &&
        abs(localCoord.y - center.y) < 83 &&
        abs(localCoord.x - center.x) <= 83) {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else if (abs(localCoord.x - center.x) > 80 &&
             abs(localCoord.x - center.x) < 83 &&
             abs(localCoord.y - center.y) <= 83) {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else {
        discard;
    }
}

void drawCircle()
{
    vec2 p = localCoord.xy - center;
    float dist = length(p);
    vec4 backgroundColor = vec4(1.0, 1.0, 1.0, 1.0);

    float outerEdge = radius + thickness * 0.5;
    float innerEdge = radius - thickness * 0.5;

    float edge0 = innerEdge + smoothness;
    float edge1 = outerEdge - smoothness;

    if (abs(localCoord.y - center.y) > radius + thickness &&
            abs(localCoord.y - center.y) <= radius + thickness + 2 &&
            abs(localCoord.x - center.x) <= radius + thickness + 2) {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else if (abs(localCoord.x - center.x) > radius + thickness &&
             abs(localCoord.x - center.x) <= radius + thickness + 2 &&
             abs(localCoord.y - center.y) <= radius + thickness + 2) {
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


void main()
{
    if (shapeType == 1) {
        drawRectangle();
    }
    else if (shapeType == 2) {
        drawCircle();
    }
}
