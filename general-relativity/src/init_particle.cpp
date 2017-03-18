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

//中が空洞の球
Vector3<double> rand_sphere(double r_min, double thickness)
{
	//移動方向の単位ベクトル
	Quaternion<double> u = Quaternion<double>(
		0.0,
		rand_0center(),
		rand_0center(),
		rand_0center()
		);
	u.Normalize();
	//それを回転させてシャッフル
	Quaternion<double> rot = Quaternion<double>(
		rand_0center(),
		rand_0center(),
		rand_0center(),
		rand_0center()
		);
	rot.Normalize();
	u = rot.Conjugate() * u * rot;
	Vector3<double> l = Vector3<double>(
		u.i1, u.i2, u.i3);
	//半径
	double a = ((double)rand() / (double)RAND_MAX) * thickness
		+ r_min;
	return l * a;
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
	zoom = 5.0e-8;
}

//中が空洞の球
void init_003()
{
	double r = 6.96e+8 * 1000; //半径
	double m = 1.9891e+30 * 1000000 / num_particle; //質量
	double s = 2.0e+3; //回転速度
	for (int i = 0; i < num_particle; ++i)
	{
		location[i] = rand_sphere(r * (1.0 - density), r * density);
		velocity[i] = Vector3<double>(
			s / r * location[i].z,
			0.0,
			-s / r * location[i].x
			);
		//velocity[i] = Vector3<double>(0.0, 0.0, 0.0);
		mass[i] = m;
	}
	zoom = 5.0e-12;
}

//円盤銀河
void init_004()
{
	double r = 9.46073e+20; //半径 10万光年
	double m = 1.9891e+30 * 2.0e+12 / num_particle; //質量
	double s = 1.0e+8; //回転速度
	for (int i = 1; i < num_particle; ++i)
	{
		location[i] = rand_sphere(r * (1.0 - density), r * density);
		location[i].y *= 0.01;
		location[i].x *= 0.1;
		velocity[i] = Vector3<double>(
			s / r * location[i].z,
			0.0,
			-s / r * location[i].x
			);
		mass[i] = m;
	}
	location[0] = Vector3<double>(0.0, 0.0, 0.0);
	velocity[0] = Vector3<double>(0.0, 0.0, 0.0);
	mass[0] = 1.9891e+30 * 1.0e+12;
	zoom = 5.0e-21;
}

int init_particle(int method)
{
	std::array<void(*)(), 9> init_func;
	init_func.fill(nullptr);
	init_func[0] = init_000;
	init_func[1] = init_001;
	init_func[2] = init_002;
	init_func[3] = init_003;
	init_func[4] = init_004;
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
