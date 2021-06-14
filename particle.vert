#version 330 core
layout(location = 0) in vec3 vertex_position;
//layout(location = 1) in vec3 vertexColor;

uniform mat4 mvp_matrix;
//out vec3 fragmentColor;
uniform float size;

//out float point_size;

void main()
{
    //gl_Position.xyz = vertex_position;
    //gl_Position.w = 1.0;

    vec4 v = vec4(vertex_position, 1);
    gl_Position = mvp_matrix * v;
    gl_PointSize = size / gl_Position.w;
    //point_size = gl_PointSize * 10.0;

//    fragmentColor = vertexColor;
}
