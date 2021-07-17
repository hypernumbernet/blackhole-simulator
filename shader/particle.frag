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
    switch (gl_PrimitiveID % 7)
    {
    case 0: color = vec4(n.z      , n.z      , n.z      , 1.0); break;
    case 1: color = vec4(n.z * 0.5, n.z      , n.z      , 1.0); break;
    case 2: color = vec4(n.z      , n.z * 0.5, n.z      , 1.0); break;
    case 3: color = vec4(n.z      , n.z      , n.z * 0.5, 1.0); break;
    case 4: color = vec4(n.z      , n.z * 0.5, n.z * 0.5, 1.0); break;
    case 5: color = vec4(n.z * 0.5, n.z      , n.z * 0.5, 1.0); break;
    case 6: color = vec4(n.z * 0.5, n.z * 0.5, n.z      , 1.0); break;
    }

    // for debug
    //if (gl_PrimitiveID == 0)
    //    color = vec4(0.0, n.z, 0.0, 1.0);
}
