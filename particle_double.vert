#version 450 core

layout(location = 0) in vec3 vertex_position;

//struct Particle{
//    dvec3 pos;
//};

//layout(std430, binding=2) buffer particles{
//    Particle par[];
//};

uniform mat4 mvp_matrix;
uniform float size;

void main()
{
//    dvec3 p = par[gl_VertexID].pos;
    vec3 p = vertex_position;
    float x = float(p.x);
    float y = float(p.y);
    float z = float(p.z);

    vec4 v = vec4(x, y, z, 1.0);

    gl_Position = mvp_matrix * v;
    gl_PointSize = size / gl_Position.w;
}
