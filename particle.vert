#version 450 core

layout(location = 0) in vec3 vertex_position;

uniform mat4 mvp_matrix;
uniform float size;

void main()
{
    vec4 v = vec4(vertex_position, 1);
    gl_Position = mvp_matrix * v;
    gl_PointSize = size / gl_Position.w;
}
