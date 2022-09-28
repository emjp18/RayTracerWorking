#include "Metal.h"

bool Metal::scatter(Ray& r_in, hit_record& rec, Color3& attenuation, Ray& scattered)
{
    r_in.getDirection().normalize();
    Vec3 reflected = r_in.getDirection().reflect(rec.normal);
    
    scattered = Ray(rec.p, reflected);
    attenuation = albedo;
    return (scattered.getDirection().dot(rec.normal) > 0);
}
