#include "Vec3.h"

double Vec3::length()
{
    double s = std::pow(g_x, 2.0)+ std::pow(g_y, 2.0) + std::pow(g_z, 2.0);
    return std::sqrt(s);

}

void Vec3::normalize()
{
    this->operator/=(length());
}

double Vec3::dot(const Vec3& v)
{
    return (g_x * v.g_x) + (g_y * v.g_y) + (g_z * v.g_z);
}

Vec3& Vec3::cross(const Vec3& v)
{
    Vec3 crossed(g_y * v.g_z - g_z * v.g_y,
        g_z * v.g_x - g_x * v.g_z,
        g_x * v.g_y - g_y * v.g_x);

    return crossed;
}

Vec3& Vec3::reflect(const Vec3& n)
{
    Vec3 N = n;
    return *this - ((N * dot(N))*2);
}

Vec3& Vec3::refract(const Vec3& n, double etai_over_etat)
{
    Vec3 N = n;
    auto cos_theta = fmin(-this->dot(n), 1.0);
    Vec3 r_out_perp =  (*this + (N * cos_theta))* etai_over_etat;
    Vec3 r_out_parallel = N * -sqrt(fabs(1.0 - r_out_perp.dot(r_out_perp)));
    return r_out_perp + r_out_parallel;
}
