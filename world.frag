#version 450 core
out vec3 color;
in vec3 fragmentColor;

void main()
{
    //color =vec3(1.0, 0.0, 0.0);
    color = fragmentColor;
}
