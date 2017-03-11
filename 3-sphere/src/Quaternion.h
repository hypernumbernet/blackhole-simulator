// @author Tomohito Inoue <hypernumbernet@users.noreply.github.com>
//Quaternion.h
//VERSION 0.74

//このモジュールは、
//Lamp：ゲーム開発とその学習をサポートするミドルウェア
//http://lamp.sourceforge.jp/
//http://lamp.sourceforge.jp/lamp/reference/Quaternion_8h-source.html
//をテンプレートが使用できるように改造したものです。

#define ATANH(x) ((log(1 + (x)) - log(1 - (x))) / 2)

#include "Vector3.h"

#ifndef QUATERNION_H_
#define QUATERNION_H_

namespace hnn
{

template <typename T> 
class Quaternion
{
public:
	// メンバ変数
	union
	{
		struct
		{
			T i0;
			T i1;
			T i2;
			T i3;
		};
		T array[4];
	};
	// 定数
	static const Quaternion<T> Zero;
	static const Quaternion<T> Identity;
	// コンストラクタ
	inline Quaternion(){}
	//inline Quaternion(T real) 
	//	: i0(real){}
	inline Quaternion(T real, T imaginary1, T imaginary2, T imaginary3) 
		: i0(real), i1(imaginary1), i2(imaginary2), i3(imaginary3){}
	inline explicit Quaternion(const T* const a) 
		: i0(a[0]), i1(a[1]), i2(a[2]), i3(a[3]){}
	// 値の設定
	inline void set(T real, T imaginary1, T imaginary2, T imaginary3)
	{
		i0 = real;
		i1 = imaginary1;
		i2 = imaginary2;
		i3 = imaginary3;
	}
	inline void set(const T* const a)
	{
		i0 = a[0];
		i1 = a[1];
		i2 = a[2];
		i3 = a[3];
	}

	// 加算
	inline Quaternion operator +(const Quaternion& a) const
	{
		return Quaternion(i0 + a.i0, i1 + a.i1, i2 + a.i2, i3 + a.i3);
	}

	// 減算
	inline Quaternion operator -(const Quaternion& a) const
	{
		return Quaternion(i0 - a.i0, i1 - a.i1, i2 - a.i2, i3 - a.i3);
	}

	// 乗算
	inline Quaternion operator *(const Quaternion& a) const
	{
		return Quaternion(
			i0 * a.i0 - i1 * a.i1 - i2 * a.i2 - i3 * a.i3,
			i0 * a.i1 + i1 * a.i0 + i2 * a.i3 - i3 * a.i2,
			i0 * a.i2 - i1 * a.i3 + i2 * a.i0 + i3 * a.i1,
			i0 * a.i3 + i1 * a.i2 - i2 * a.i1 + i3 * a.i0
		);
	}
	template <typename X>
	inline Quaternion operator *(X a) const
	{
		return Quaternion(i0 * a, i1 * a, i2 * a, i3 * a);
	}
	template <typename X>
	inline friend Quaternion operator *(X a, const Quaternion& b)
	{
		return Quaternion(a * b.i0, a * b.i1, a * b.i2, a * b.i3);
	}
	// 除算
	inline Quaternion operator /(const Quaternion& a) const
	{
		return (*this) * a.Inverse();
	}
	template <typename X>
	inline Quaternion operator /(X a) const
	{
		return Quaternion(i0 / a, i1 / a, i2 / a, i3 / a);
	}
	template <typename X>
	inline friend Quaternion operator /(X a, const Quaternion& b)
	{
		return a * b.Inverse();
	}
	// +演算子
	inline Quaternion operator +() const{ return *this; }
	// -演算子
	inline Quaternion operator -() const{ return Quaternion(-i0, -i1, -i2, -i3); }
	// 代入加算
	inline Quaternion& operator +=(const Quaternion& a)
	{
		i0 += a.i0;
		i1 += a.i1;
		i2 += a.i2;
		i3 += a.i3;
		return *this;
	}
	// 代入減算
	inline Quaternion& operator -=(const Quaternion& a)
	{
		i0 -= a.i0;
		i1 -= a.i1;
		i2 -= a.i2;
		i3 -= a.i3;
		return *this;
	}
	// 代入乗算
	inline Quaternion& operator *=(const Quaternion& a)
	{
		set(
			i0 * a.i0 - i1 * a.i1 - i2 * a.i2 - i3 * a.i3,
			i0 * a.i1 + i1 * a.i0 + i2 * a.i3 - i3 * a.i2,
			i0 * a.i2 - i1 * a.i3 + i2 * a.i0 + i3 * a.i1,
			i0 * a.i3 + i1 * a.i2 - i2 * a.i1 + i3 * a.i0
		);
		return *this;
	}
	template <typename X>
	inline Quaternion& operator *=(X a)
	{
		i0 *= a;
		i1 *= a;
		i2 *= a;
		i3 *= a;
		return *this;
	}
	// 代入除算
	inline Quaternion& operator /=(const Quaternion& a)
	{
		Quaternion ans = (*this) * a.Inverse();
		set(ans.i0, ans.i1, ans.i2, ans.i3);
		return *this;
	}
	template <typename X>
	inline Quaternion& operator /=(X a)
	{
		i0 /= a;
		i1 /= a;
		i2 /= a;
		i3 /= a;
		return *this;
	}
	// 比較演算
	inline bool operator ==(const Quaternion& a) const
	{
		return ((i0 == a.i0) && (i1 == a.i1) && (i2 == a.i2) && (i3 == a.i3));
	}
	inline bool operator !=(const Quaternion& a) const
	{
		return ((i0 != a.i1) || (i1 != a.i1) || (i2 != a.i2) || (i3 != a.i3));
	}
	// ノルム
	inline T Norm() const
	{
		return i0 * i0 + i1 * i1 + i2 * i2 + i3 * i3;
	}
	// 絶対値
	inline T Abs() const
	{
		return sqrt(i0 * i0 + i1 * i1 + i2 * i2 + i3 * i3);
	}
	// 正規化
	inline Quaternion& Normalize()
	{
		return (*this) /= sqrt(i0 * i0 + i1 * i1 + i2 * i2 + i3 * i3);
	}
	// 共役
	inline Quaternion Conjugate() const
	{
		return Quaternion(i0, -i1, -i2, -i3);
	}
	// 逆数
	inline Quaternion Inverse() const
	{
		T norm = i0 * i0 + i1 * i1 + i2 * i2 + i3 * i3;
		return Quaternion(i0 / norm, -i1 / norm, -i2 / norm, -i3 / norm);
	}
	// 偏角
	inline T Arg() const
	{
		return acos(i0 / sqrt(i0 * i0 + i1 * i1 + i2 * i2 + i3 * i3));
	}
	// 指数関数
	inline Quaternion Exp() const
	{
		T n = sqrt(i1 * i1 + i2 * i2 + i3 * i3);
		T e = exp(i0);
		T a;
		if (n == 0)
		{
			a = e;
		}
		else
		{
			a = e * sin(n) / n;
		}
		return Quaternion(e * cos(n), i1 * a, i2 * a, i3 * a);
	}
	// 指数関数 - 虚数部のみ
	inline Quaternion ExpIm() const
	{
		T n = sqrt(i1 * i1 + i2 * i2 + i3 * i3);
		T a;
		if (n == 0)
		{
			a = 0;
		}
		else
		{
			a = sin(n) / n;
		}
		return Quaternion(cos(n), i1 * a, i2 * a, i3 * a);
	}
	// 指数関数 - 虚数部のみ
	inline static Quaternion Exp(T x, T y, T z)
	{
		T n = sqrt(x * x + y * y + z * z);
		T a;
		if (n == 0)
		{
			a = 0;
		}
		else
		{
			a = sin(n) / n;
		}
		return Quaternion(cos(n), x * a, y * a, z * a);
	}
	// 指数関数 - 双曲線関数使用
	inline static Quaternion Exph(T x, T y, T z)
	{
		T n = sqrt(x * x + y * y + z * z);
		T a;
		if (n == 0)
		{
			a = 0;
		}
		else
		{
			a = sinh(n) / n;
		}
		return Quaternion(cosh(n), x * a, y * a, z * a);
	}
	// 対数関数
	//先立って1に正規化すること
	inline Quaternion Ln() const
	{
		T n = sqrt(i1 * i1 + i2 * i2 + i3 * i3);
		if (n == 0)
		{
			return Quaternion::Zero;
		}
		n = atan2(n, i0) / n;
		//scalar part
		//0.5 * log(a.i0 * a.i0 + a.i1 * a.i1 + a.i2 * a.i2 + a.i3 * a.i3)
		return Quaternion(0, i1 * n, i2 * n, i3 * n);
	}
	// 対数関数 - Vector3
	inline Vector3<T> LnV3() const
	{
		T n = sqrt(i1 * i1 + i2 * i2 + i3 * i3);
		if (n == 0)
		{
			return Vector3<T>(0, 0, 0);
		}
		n = atan2(n, i0) / n;
		return Vector3<T>(i1 * n, i2 * n, i3 * n);
	}
	// 対数関数 - 双曲線関数使用
	/*
	inline Quaternion Lnh() const
	{
		T n = sqrt(i1 * i1 + i2 * i2 + i3 * i3);
		if (n == 0) 
			return Quaternion::Zero;
		n = boost::math::atanh(n / i0) / n;
		//n = atanh(n / i0) / n;
		return Quaternion(0, i1 * n, i2 * n, i3 * n);
	}
	*/
	inline Vector3<T> LnhV3() const
	{
		T n = sqrt(i1 * i1 + i2 * i2 + i3 * i3);
		if (n == 0)
		{
			return Vector3<T>(0, 0, 0);
		}
		//n = boost::math::atanh(n) / n;
		n = ATANH(n) / n;
		return Vector3<T>(i1 * n, i2 * n, i3 * n);
	}
	inline Quaternion Rot() const
	{
		T c1 = cos(i1);
		T c2 = cos(i2);
		T c3 = cos(i3);
		T s1 = sin(i1);
		T s2 = sin(i2);
		T s3 = sin(i3);
		return Quaternion(
			c1 * c2 * c3 - s1 * s2 * s3,
			s1 * c2 * c3 + c1 * s2 * s3,
			c1 * s2 * c3 - s1 * c2 * s3,
			s1 * s2 * c3 + c1 * c2 * s3
		);
	}
	inline static Quaternion Rot(T x, T y, T z)
	{
		T c1 = cos(x);
		T c2 = cos(y);
		T c3 = cos(z);
		T s1 = sin(x);
		T s2 = sin(y);
		T s3 = sin(z);
		return Quaternion(
			c1 * c2 * c3 - s1 * s2 * s3,
			s1 * c2 * c3 + c1 * s2 * s3,
			c1 * s2 * c3 - s1 * c2 * s3,
			s1 * s2 * c3 + c1 * c2 * s3
		);
	}
	inline static Quaternion Roth(T x, T y, T z)
	{
		T c1 = cosh(x);
		T c2 = cosh(y);
		T c3 = cosh(z);
		T s1 = sinh(x);
		T s2 = sinh(y);
		T s3 = sinh(z);
		return Quaternion(
			c1 * c2 * c3 - s1 * s2 * s3,
			s1 * c2 * c3 + c1 * s2 * s3,
			c1 * s2 * c3 - s1 * c2 * s3,
			s1 * s2 * c3 + c1 * c2 * s3
		);
	}
	// 内積
	inline T Dot(const Quaternion& a) const
	{
		return i0 * a.i0 + i1 * a.i1 + i2 * a.i2 + i3 * a.i3;
	}
	//七次元の外積の右４つから計算された左３つ
	inline Quaternion Cross7(const Quaternion& a) const
	{
		return Quaternion(
			0,
			-i0 * a.i1 + i1 * a.i0 - i2 * a.i3 + i3 * a.i2,
			-i0 * a.i2 + i1 * a.i3 + i2 * a.i0 - i3 * a.i1,
			-i0 * a.i3 - i1 * a.i2 + i2 * a.i1 + i3 * a.i0
		);
	}
	inline Vector3<T> Cross7V3(const Quaternion& a) const
	{
		return Vector3<T>(
			-i0 * a.i1 + i1 * a.i0 - i2 * a.i3 + i3 * a.i2,
			-i0 * a.i2 + i1 * a.i3 + i2 * a.i0 - i3 * a.i1,
			-i0 * a.i3 - i1 * a.i2 + i2 * a.i1 + i3 * a.i0
			);
	}
	//八元数の右４つを左４つで共役積した結果の右４つ
	inline Quaternion Rot8(const Quaternion& rot) const
	{
		return Quaternion(
			(rot.i0 * i0 - rot.i1 * i1 - rot.i2 * i2 - rot.i3 * i3) * rot.i0
			- (rot.i0 * i1 + rot.i1 * i0 + rot.i2 * i3 - rot.i3 * i2) * rot.i1
			- (rot.i0 * i2 - rot.i1 * i3 + rot.i2 * i0 + rot.i3 * i1) * rot.i2
			- (rot.i0 * i3 + rot.i1 * i2 - rot.i2 * i1 + rot.i3 * i0) * rot.i3
			,
			(rot.i0 * i0 - rot.i1 * i1 - rot.i2 * i2 - rot.i3 * i3) * rot.i1
			+ (rot.i0 * i1 + rot.i1 * i0 + rot.i2 * i3 - rot.i3 * i2) * rot.i0
			- (rot.i0 * i2 - rot.i1 * i3 + rot.i2 * i0 + rot.i3 * i1) * rot.i3
			+ (rot.i0 * i3 + rot.i1 * i2 - rot.i2 * i1 + rot.i3 * i0) * rot.i2
			,
			(rot.i0 * i0 - rot.i1 * i1 - rot.i2 * i2 - rot.i3 * i3) * rot.i2
			+ (rot.i0 * i1 + rot.i1 * i0 + rot.i2 * i3 - rot.i3 * i2) * rot.i3
			+ (rot.i0 * i2 - rot.i1 * i3 + rot.i2 * i0 + rot.i3 * i1) * rot.i0
			- (rot.i0 * i3 + rot.i1 * i2 - rot.i2 * i1 + rot.i3 * i0) * rot.i1
			,
			(rot.i0 * i0 - rot.i1 * i1 - rot.i2 * i2 - rot.i3 * i3) * rot.i3
			- (rot.i0 * i1 + rot.i1 * i0 + rot.i2 * i3 - rot.i3 * i2) * rot.i2
			+ (rot.i0 * i2 - rot.i1 * i3 + rot.i2 * i0 + rot.i3 * i1) * rot.i1
			+ (rot.i0 * i3 + rot.i1 * i2 - rot.i2 * i1 + rot.i3 * i0) * rot.i0
		);
	}
	//八元数の右４つを左４つで共役積した結果の右４つ
	//Identityを回転
	inline Quaternion Rot8Identity() const
	{
		return Quaternion(
			i0 * i0 - i1 * i1 - i2 * i2 - i3 * i3,
			i0 * i1 + i1 * i0 - i2 * i3 + i3 * i2,
			i0 * i2 + i1 * i3 + i2 * i0 - i3 * i1,
			i0 * i3 - i1 * i2 + i2 * i1 + i3 * i0
		);
	}
	//Cross7を正規化した四元数から回転四元数を計算
	template <typename X>
	inline Quaternion& MakeRotation(X theta)
	{
		X c = cos(theta);
		X s = sin(theta);
		i0 = c;
		i1 *= s;
		i2 *= s;
		i3 *= s;
		return (*this);
	}
	//MakeRotationとRot8を合わせた関数。vは正規化しておくこと
    inline Quaternion RotMove(const T theta, const Vector3<T>& v) const
    {
		T c = cos(theta);
		T s = sin(theta);
		return Quaternion(
			i0 * c - (v.x * i1 + v.y * i2 + v.z * i3) * s,
			i1 * c + (v.x * i0 + v.y * i3 - v.z * i2) * s,
			i2 * c + (v.y * i0 + v.z * i1 - v.x * i3) * s,
			i3 * c + (v.z * i0 - v.y * i1 + v.x * i2) * s
		);
	}
};
// ゼロ四元数
template <typename T>
const Quaternion<T> Quaternion<T>::Zero(0, 0, 0, 0);

// 恒等四元数
template <typename T>
const Quaternion<T> Quaternion<T>::Identity(1, 0, 0, 0);

}//namespace hnn

#endif // End of QUATERNION_H_
