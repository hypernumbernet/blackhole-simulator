// @author Tomohito Inoue <hypernumbernet@users.noreply.github.com>
#include "stdafx.h"
#include "gravity.h"
#include <array>
#include <iostream>

//プラスとマイナス領域均等の範囲の乱数
double rand_0center()
{
	if (rand() % 2 == 0)
	{
		return (double)rand();
	}
	else
	{
		return -(double)rand();
	}
}

//原点を中心に立方体に配置
void init_001()
{
	double a = 1.0e+3;
	double b = 1.0e-3;
	for (int i = 0; i < num_particle; ++i)
	{
		location[i] = Vector3<double>(
			rand_0center() * a,
			rand_0center() * a,
			rand_0center() * a
			);
		velocity[i] = Vector3<double>(
			rand_0center() * b,
			rand_0center() * b,
			rand_0center() * b
			);
		mass[i] = 1.0e-1;
	}
	zoom = 1.0e-7;
}

int init_particle(int method)
{
	std::array<void(*)(), 7> init_func;
	init_func.fill(nullptr);
	init_func[1] = init_001;
	if (method < 0 || method >= init_func.size())
	{
		return 1;
	}
	if (init_func[method] == nullptr) {
		return 1;
	}
	init_func[method]();
	return 0;
}
