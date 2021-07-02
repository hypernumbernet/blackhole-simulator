#version 450 core

//layout(location = 0) in vec3 vertex_position;

struct Particle{
    vec3 pos;
};

layout(std430, binding=0) buffer particles{
    Particle par[];
};

uniform mat4 mvp_matrix;
uniform float size;

void main()
{
    vec4 v = vec4(par[gl_VertexID].pos, 1);

    gl_Position = mvp_matrix * v;
    gl_PointSize = size / gl_Position.w;
}
