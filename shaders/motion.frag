#version 440

layout(location = 0) in vec4 localCoord;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform uniformBlock {
    mat4 view;
    mat4 projection;
    vec2 p0;
    vec2 p1;
    vec2 p2;
    vec2 p3;
};

layout(binding = 1) uniform sampler2D tex;

bool isPointInPolygon(vec2 point, vec2 polygon[4], int numPoints) {
    bool isInside = false;
    for (int i = 0, j = numPoints - 1; i < numPoints; j = i++) {
        if (((polygon[i].y > point.y) != (polygon[j].y > point.y)) &&
            (point.x < (polygon[j].x -
                        polygon[i].x) * (point.y - polygon[i].y) /
             (polygon[j].y - polygon[i].y) + polygon[i].x)) {
            isInside = !isInside;
        }
    }
    return isInside;
}

void main(void)
{
    vec2 polygon[4] = vec2[](p0, p1, p2, p3);
    if (isPointInPolygon(fragTexCoord, polygon, 4)) {
        vec4 c = texture(tex, fragTexCoord);
        fragColor = vec4(c.rgb,  c.a);
    }
    else {
        discard;
    }

    // if (
    //     // fragTexCoord.x * 360.0 <= t &&
    //     fragTexCoord.y * 360.0 <= t
    //     ) {
    //     vec4 c = texture(tex, fragTexCoord);
    //     fragColor = vec4(c.rgb,  c.a);
    // }
    // else {
    //         discard;
    // }
}
