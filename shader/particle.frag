#version 450 core

out vec4 color;

void main()
{
    vec3 n;
    n.xy = gl_PointCoord * 2.0 - 1.0;
    n.z = 1.0 - pow(dot(n.xy, n.xy), 1.2);
    if (n.z < 0.0)
        discard;
    color = vec4(n.z, n.z, n.z, 1.0);
}
