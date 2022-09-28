#include "Dielectric.h"

bool Dielectric::scatter(Ray& r_in, hit_record& rec, Color3& attenuation, Ray& scattered)
{
    attenuation = Color3(1.0, 1.0, 1.0);
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

    Vec3 unit_direction = (r_in.getDirection());
    unit_direction.normalize();
    double cos_theta = fmin(rec.normal.dot(-unit_direction), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    Vec3 direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
        direction = unit_direction.reflect(rec.normal);
    else
        direction = unit_direction.refract( rec.normal, refraction_ratio);

    scattered = Ray(rec.p, direction);
    return true;
}
