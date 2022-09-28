#pragma once
#include "Common.h"
class Vec3
{
public:
	double g_x = 0.0;
	double g_y = 0.0;
	double g_z = 0.0;
	Vec3(double x, double y, double z):g_x(x), g_y(y), g_z(z){}
	Vec3() {}
	~Vec3() {}
	inline static Vec3 random() {
		return Vec3(random_double(), random_double(), random_double());
	}

	inline static Vec3 random(double min, double max) {
		return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}
	bool near_zero() const {
		// Return true if the vector is close to zero in all dimensions.
		const auto s = 1e-8;
		return (fabs(g_x) < s) && (fabs(g_y) < s) && (fabs(g_z) < s);
	}

	inline Vec3 operator*(const Vec3& v) {
		return Vec3(g_x * v.g_x, g_y * v.g_y, g_z * v.g_z);
	}

	Vec3 operator-() const { return Vec3(-g_x, -g_y, -g_z); }

	Vec3& operator+(const Vec3& v)
	{
		Vec3 vec(g_x + v.g_x, g_y + v.g_y, g_z + v.g_z);
		
		return vec;
	}
	Vec3& operator-(const Vec3& v)
	{
		Vec3 vec(g_x - v.g_x, g_y - v.g_y, g_z - v.g_z);

		return vec;
	}
	Vec3& operator*(const double t)
	{
		Vec3 vec(g_x * t, g_y * t,g_z * t);

		return vec;
	}
	Vec3& operator/(const double t)
	{
		Vec3 vec(g_x * 1.0/t, g_y * 1.0 / t, g_z * 1.0 / t);

		return vec;
	}
	Vec3& operator+=(const Vec3& v)
	{
		g_x += v.g_x;
		g_y += v.g_y;
		g_z += v.g_z;
		return *this;
	}
	Vec3& operator-=(const Vec3& v)
	{
		g_x -= v.g_x;
		g_y -= v.g_y;
		g_z -= v.g_z;
		return *this;
	}
	Vec3& operator*=(const double t)
	{
		g_x *= t;
		g_y *= t;
		g_z *= t;
		return *this;
	}
	Vec3& operator/=(const double t)
	{
		g_x *= 1.0 / t;
		g_y *= 1.0 / t;
		g_z *= 1.0 / t;
		return *this;
	}
	double length();
	void normalize();
	double dot(const Vec3& v);
	//this cross v
	Vec3& cross(const Vec3& v);
	//this is the incident ray.
	Vec3& reflect(const Vec3& n);
	Vec3& refract(const Vec3& n, double etai_over_etat);
};

using Color3 = Vec3;