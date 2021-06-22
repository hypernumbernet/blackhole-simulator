#version 450 core
out vec4 color;
//in vec3 fragmentColor;

//in float point_size;

void main()
{
    //color = fragmentColor;
    //vec4 col = vec4(fragmentColor, 0.5);
    //color = vec4(1.0, 1.0, 1.0, 0.5);

    vec3 n;
    n.xy = gl_PointCoord * 2.0 - 1.0;
    n.z = 1.0 - pow(dot(n.xy, n.xy), 1.2);
    if (n.z < 0.0)
        discard;
    //n.z = pow(n.z * 1.1, 3.0)+1.0;
    color = vec4(n.z, n.z, n.z, 1.0);
}
