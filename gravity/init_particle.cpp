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
void init_000()
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
			rand_0center() * b + 1.0e+5,
			rand_0center() * b,
			rand_0center() * b
			);
		mass[i] = 1.0e-1;
	}
	zoom = 1.0e-7;
}

//2カ所に配置して正反対の速度
void init_001()
{
	double a = 1.0e+3;
	double b = 1.0e-3;
	for (int i = 0; i < num_particle; ++i)
	{
		if (i % 2 == 0) 
		{
			location[i] = Vector3<double>(
				rand_0center() * a,
				rand_0center() * a + 5.0e+7,
				rand_0center() * a
				);
			velocity[i] = Vector3<double>(
				rand_0center() * b + 1.0e+5,
				rand_0center() * b,
				rand_0center() * b
				);
			mass[i] = 4.0e-1;
		}
		else
		{
			location[i] = Vector3<double>(
				rand_0center() * a,
				rand_0center() * a - 5.0e+7,
				rand_0center() * a
				);
			velocity[i] = Vector3<double>(
				rand_0center() * b - 1.0e+5,
				rand_0center() * b,
				rand_0center() * b
				);
			mass[i] = 2.0e-1;
		}
	}
	zoom = 5.0e-8;
}

//太陽と水星
void init_002()
{
	location[0] = Vector3<double>(0.0, 0.0, 0.0);
	velocity[0] = Vector3<double>(0.0, 0.0, 0.0);
	mass[0] = 1.9891e+30;
	location[1] = Vector3<double>(5.791e+7, 0, 0);
	velocity[1] = Vector3<double>(0, 0, 4.78725e+1);
	mass[1] = 3.301e+23;
	zoom = 8.0e-8;
}

int init_particle(int method)
{
	std::array<void(*)(), 9> init_func;
	init_func.fill(nullptr);
	init_func[0] = init_000;
	init_func[1] = init_001;
	init_func[2] = init_002;
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
