#include "Lambertian.h"

bool Lambertian::scatter(Ray& r_in, hit_record& rec, Color3& attenuation, Ray& scattered) 
{
    Vec3 scatter_direction = rec.normal + random_unit_vector();
    // Catch degenerate scatter direction
    if (scatter_direction.near_zero())
        scatter_direction = rec.normal;
    scattered = Ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
}
