// @author Tomohito Inoue <hypernumbernet@users.noreply.github.com>
#include "stdafx.h"
#include "gravity.h"

//プラスとマイナス領域均等の範囲の乱数
double rand_0center()
{
	//rand() / ((double)RAND_MAX) * 2 - 1
	//rand() - ((double)RAND_MAX) / 2
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
	double a;
	switch (method)
	{
		//----------------------------------------------------------------------------
		//原点を中心に立方体に配置
	case 1:
		for (i = 0; i < num_particle; ++i)
		{
			//実数部の割合を大きくすると中心部に集まる
			location[i] = Quaternion<double>(
				rand() % 64 + (density),
				rand() % 64 - 32,
				rand() % 64 - 32,
				rand() % 64 - 32
			);
			location[i].Normalize();
			momentum[i] = Quaternion<double>::Exp(
				momentum_avrg * (rand() % 32 - 16),
				momentum_avrg * (rand() % 32 - 16),
				momentum_avrg * (rand() % 32 - 16)
			);
			locat_ln_r[i] = location[i].LnV3() * zoom;
		}
		break;
		//----------------------------------------------------------------------------
		//原点とずれた位置に立方体に配置
	case 2:
		for (i = 0; i < num_particle; ++i)
		{
			//実数部の割合を大きくすると中心部に集まる
			location[i] = Quaternion<double>(
				rand() % 64 + (density),
				rand() % 64,
				rand() % 64,
				rand() % 64
			);
			location[i].Normalize();
			momentum[i] = Quaternion<double>::Exp(
				momentum_avrg * (rand() % 32 - 16),
				momentum_avrg * (rand() % 32 - 16),
				momentum_avrg * (rand() % 32 - 16)
			);
			locat_ln_r[i] = location[i].LnV3() * zoom;
		}
		break;
		//----------------------------------------------------------------------------
		//回転する立方体
	case 3:
		zoom = 128;
		for (i = 0; i < num_particle; ++i)
		{
			location[i] = Quaternion<double>(
				rand() % 64 + 1024,
				rand() % 64 - 32,
				rand() % 64 - 32,
				rand() % 64 - 32
			);
			location[i].Normalize();
			//回転
			momentum[i] = Quaternion<double>::Exp(
				-location[i].i3 * 0.6,
				0,
				location[i].i1 * 0.6
			);
			locat_ln_r[i] = location[i].LnV3() * zoom;
		}
		break;
		//----------------------------------------------------------------------------
		//3ヶ所に立方体
	case 4:
		zoom = 8;
		n1 = (num_particle - (num_particle % 3)) / 3;
		n2 = n1 * 2;

		a = 0.4;

		for (i = 0; i < n2; ++i)
		{
			location[i] = Quaternion<double>(
				rand() % 64 + density,
				rand() % 64 - 32,
				rand() % 64 - 32,
				rand() % 64 - 32
			);
			location[i].Normalize();
		}

		for (i = 0; i < n1; ++i)
		{
			location[i] *= Quaternion<double>::Exp(
				0.0,
				0.0,
				a
			);
		}
		for (i = n1; i < n2; ++i)
		{
			location[i] *= Quaternion<double>::Exp(
				0.0,
				0.0,
				-a
			);
		}
		for (i = n2; i < num_particle; ++i)
		{
			location[i] = Quaternion<double>(
				rand() % 8000,
				rand() % 64 - 32,
				rand() % 64 - 32,
				rand() % 64 - 32
			);
			location[i].Normalize();
		}
		for (i = n2; i < num_particle; ++i)
		{
			location[i] *= Quaternion<double>::Exp(
				a,
				0.0,
				0.0
			);
		}

		for (i = 0; i < num_particle; ++i)
		{
			momentum[i] = Quaternion<double>::Exp(
				momentum_avrg * (rand() % 32 - 16),
				momentum_avrg * (rand() % 32 - 16),
				momentum_avrg * (rand() % 32 - 16)
			);
			//momentum[i] = Quaternion<double>::Exp(-locat[i].i3, 0, locat[i].i1);
			locat_ln_r[i] = location[i].LnV3() * zoom;
		}
		break;
		//----------------------------------------------------------------------------
		//中が空洞の球
	case 5:
		for (i = 0; i < num_particle; ++i)
		{
			//回転方向の単位ベクトル
			location[i] = Quaternion<double>(
				0,
				rand_0center(),
				rand_0center(),
				rand_0center()
			);
			location[i].Normalize();
			//それを回転させてシャッフル
			Quaternion<double> rot = Quaternion<double>(
				rand_0center(),
				rand_0center(),
				rand_0center(),
				rand_0center()
			);
			rot.Normalize();
			location[i] = rot.Conjugate() * location[i] * rot;
			//それを回転させてシャッフル　2度目
			rot = Quaternion<double>(
				rand_0center(),
				rand_0center(),
				rand_0center(),
				rand_0center()
			);
			rot.Normalize();
			location[i] = rot.Conjugate() * location[i] * rot;
			//それを回転させてシャッフル　3度目
			rot = Quaternion<double>(
				rand_0center(),
				rand_0center(),
				rand_0center(),
				rand_0center()
			);
			rot.Normalize();
			location[i] = rot.Conjugate() * location[i] * rot;
			//回転角を決める
			a = ((double)rand() / (double)RAND_MAX) * 0.02 + 0.05;
			location[i].MakeRotation(a);
			//運動量
			momentum[i] = Quaternion<double>::Exp(
				momentum_avrg * (rand() % 32 - 16),
				momentum_avrg * (rand() % 32 - 16),
				momentum_avrg * (rand() % 32 - 16)
			);
			locat_ln_r[i] = location[i].LnV3() * zoom;
		}
		break;
		//----------------------------------------------------------------------------
		//一方向へ全体が移動
	case 6:
		for (i = 0; i < num_particle; ++i)
		{
			//実数部の割合を大きくすると中心部に集まる
			location[i] = Quaternion<double>(
				rand() % 64 + density,
				rand() % 64 - 32,
				rand() % 64 - 32,
				rand() % 64 - 32
			);
			location[i].Normalize();
			momentum[i] = Quaternion<double>::Exp(
				momentum_avrg * 2000.0,
				momentum_avrg * rand_0center() / ((double)RAND_MAX),
				momentum_avrg * rand_0center() / ((double)RAND_MAX)
			);
			locat_ln_r[i] = location[i].LnV3() * zoom;
		}
		break;
		//----------------------------------------------------------------------------
		//球内に均等に配置。立方体から球の外を除外
	case 7:
		double di = 1.0 / (double)density;
		for (i = 0; i < num_particle; ++i)
		{
			Vector3<double> v = Vector3<double>(
				rand_0center() / ((double)RAND_MAX) * di,
				rand_0center() / ((double)RAND_MAX) * di,
				rand_0center() / ((double)RAND_MAX) * di
			);
			if (v.Abs() > di)
			{
				--i;
				continue;
			}
			location[i] = Quaternion<double>::Exp(
				v.x,
				v.y,
				v.z
			);
			//----------------------------------------------------------------------------
			momentum[i] = Quaternion<double>::Exp(
				momentum_avrg * rand_0center() / ((double)RAND_MAX) * 32.0,
				momentum_avrg * rand_0center() / ((double)RAND_MAX) * 32.0,
				momentum_avrg * rand_0center() / ((double)RAND_MAX) * 32.0
			);
			//回転
			//momentum[i] = Quaternion<double>::Exp
			//(
			//	- v.z * /*abs(v.z) **/ 0.2
			//,	0
			//,	v.x * /*abs(v.x) **/ 0.2
			//);
			//----------------------------------------------------------------------------
			locat_ln_r[i] = location[i].LnV3() * zoom;
		}
		break;
	}
}
