#version 450 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer position_block
{
    dvec3 position[];
};

layout(std430, binding = 1) buffer velocity_block
{
    dvec3 velocity[];
};

void main()
{
    //const uint i = gl_WorkGroupID.x;
    const uint i = gl_GlobalInvocationID.x;

    position[i] +=  velocity[i];
}
