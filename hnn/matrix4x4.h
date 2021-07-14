#pragma once

namespace hnn // https://github.com/hypernumbernet
{

template <typename TYPE>
class Matrix4x4
{
    using T = TYPE;

public:
    inline Matrix4x4(){}

private:
    T m[4][4];
};

} // namespace
