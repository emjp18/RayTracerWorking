#pragma once
#include "Hittable.h"
class Sphere :
    public Hittable
{
    public:
        Vec3 g_center;
        double g_radius;
        Sphere(Vec3 center, double r, std::shared_ptr<Material> m) :g_center(center), g_radius(r) { g_mat = m; }
       
    // Inherited via Hittable
    virtual bool hit(Ray& r, double t_min, double t_max, Material::hit_record& rec) const override;
};

