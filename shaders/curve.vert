#version 440

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 modelColumn0;
layout(location = 2) in vec4 modelColumn1;
layout(location = 3) in vec4 modelColumn2;
layout(location = 4) in vec4 modelColumn3;

layout(std140, binding = 0) uniform uniformBlock {
    mat4 view;
    mat4 projection;
    vec4 color_;
};


void main(void)
{
    mat4 model = mat4(modelColumn0, modelColumn1, modelColumn2, modelColumn3);
    gl_Position = projection * view * model * position;

}
