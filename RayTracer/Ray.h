#pragma once
#include "Vec3.h"
class Ray
{
private:
	Vec3 m_origin;
	Vec3 m_direction;
public:
	Ray(const Vec3& o, const Vec3& d);
	Ray();
	Vec3& at(double t);
	Vec3& getOrigin() { return m_origin; }
	Vec3& getDirection() { return m_direction; }
	
};

