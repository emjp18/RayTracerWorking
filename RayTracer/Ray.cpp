#include "Ray.h"

Ray::Ray(const Vec3& o, const Vec3& d)
    :m_direction(d), m_origin(o)
{
}

Ray::Ray()
{
}

Vec3& Ray::at(double t)
{
    return m_origin + (m_direction * t);
}


