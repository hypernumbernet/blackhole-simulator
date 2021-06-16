#version 450 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

uniform mat4 mvp_matrix;
out vec3 fragmentColor;

void main()
{
    vec4 v = vec4(vertexPosition_modelspace, 1);
    gl_Position = mvp_matrix * v;

    fragmentColor = vertexColor;
}
