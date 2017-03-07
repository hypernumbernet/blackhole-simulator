// @author Tomohito Inoue <hypernumbernet@users.noreply.github.com>
#include "stdafx.h"
#include "gravity.h"

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

void init_particle(int method)
{
	int i;
	int n1, n2;
	double a, b;
	switch (method)
	{
		//----------------------------------------------------------------------------
		//原点を中心に立方体に配置
	case 1:
		a = 1.0e+3;
		b = 1.0e-4;
		for (i = 0; i < num_particle; ++i) 
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
			mass[i] = 10.0;
		}
		break;
		//----------------------------------------------------------------------------
		//原点とずれた位置に立方体に配置
	case 2:
		break;
		//----------------------------------------------------------------------------
		//回転する立方体
	case 3:
		break;
		//----------------------------------------------------------------------------
		//3ヶ所に立方体
	case 4:
		break;
		//----------------------------------------------------------------------------
		//中が空洞の球
	case 5:
		break;
		//----------------------------------------------------------------------------
		//一方向へ全体が移動
	case 6:
		break;
		//----------------------------------------------------------------------------
		//球内に均等に配置。立方体から球の外を除外
	case 7:
		break;
	}
}
