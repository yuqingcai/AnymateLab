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

float sdCircle( in vec2 p, in float r )
{
    return length(p)-r;
}

void main()
{
    if (shapeType == 1) {
        drawRectangle();
    }
    else if (shapeType == 2) {
        drawCircle();
    }

    // vec2 iResolution = vec2(800.0, 600.0);
    // vec3 iMouse = vec3(0.0, 0.0, 1.0);
    // vec2 fragCoord = localCoord.xy;

    // vec2 p = (2.0*fragCoord-iResolution.xy)/iResolution.y;
    // vec2 m = (2.0*iMouse.xy-iResolution.xy)/iResolution.y;

    // float d = sdCircle(p,0.5);

    // // coloring
    // vec3 col = (d>0.0) ? vec3(0.9,0.6,0.3) : vec3(0.65,0.85,1.0);
    // col *= 1.0 - exp(-6.0*abs(d));
    //     col *= 0.8 + 0.2*cos(150.0*d);
    //     col = mix( col, vec3(1.0), 1.0-smoothstep(0.0,0.01,abs(d)) );

    // if( iMouse.z>0.001 )
    // {
    //     d = sdCircle(m,0.5);
    //     col = mix(col, vec3(1.0,1.0,0.0), 1.0-smoothstep(0.0, 0.005, abs(length(p-m)-abs(d))-0.0025));
    //     col = mix(col, vec3(1.0,1.0,0.0), 1.0-smoothstep(0.0, 0.005, length(p-m)-0.015));
    // }

    // fragColor = vec4(col,1.0);

}
