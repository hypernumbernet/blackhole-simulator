// @author Tomohito Inoue <hypernumbernet@users.noreply.github.com>
//Vector3.h
//VERSION 0.31

#ifndef VECTOR3_H_
#define VECTOR3_H_

namespace hnn
{

template <typename T> 
class Vector3
{
public:
	// メンバ変数
	union
	{
		struct
		{
			T x;
			T y;
			T z;
		};
		T array[3];
	};
	inline Vector3(){}
	inline Vector3(T x_in, T y_in, T z_in) 
		: x(x_in), y(y_in), z(z_in){}
	inline explicit Vector3(const T* const a) 
		: x(a[0]), y(a[1]), z(a[2]){}

	inline void set(T x_in, T y_in, T z_in)
	{
		x = x_in;
		y = y_in;
		z = z_in;
	}
	// 加算
	inline Vector3 operator +(const Vector3& a) const
	{
		return Vector3(x + a.x, y + a.y, z + a.z);
	}
	// +演算子
	inline Vector3 operator +() const{ return *this; }
	// -演算子
	inline Vector3 operator -() const{ return Vector3( - x, - y, - z); }

	// 減算
	inline Vector3 operator -(const Vector3& a) const
	{
		return Vector3(x - a.x, y - a.y, z - a.z);
	}

	// 乗算
	template <typename X>
	inline Vector3 operator *(X a) const
	{
		return Vector3(x * a, y * a, z * a);
	}
	template <typename X>
	inline friend Vector3 operator *(X a, const Vector3& b)
	{
		return Vector3(a * b.x, a * b.y, a * b.z);
	}

	// 代入乗算
	template <typename X>
	inline Vector3& operator *=(X a)
	{
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}
	// 比較演算
	inline bool operator ==(const Vector3& a) const
	{
		return ((x == a.x) && (y == a.y) && (z == a.z));
	}
	// ノルム
	inline T Norm() const
	{
		return x * x + y * y + z * z;
	}
	// 絶対値
	inline T Abs() const
	{
		return sqrt(x * x + y * y + z * z);
	}
	// 正規化
	inline Vector3& Normalize()
	{
		T r = sqrt(x * x + y * y + z * z);
		if (r != 0.0)
		{
			x /= r;
			y /= r;
			z /= r;
		}
		return *this;
	}
	// 内積
	inline T Dot(const Vector3& a) const
	{
		return x * a.x + y * a.y + z * a.z;
	}

};

}

#endif
