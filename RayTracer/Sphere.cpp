#include "Sphere.h"

bool Sphere::hit(Ray& r, double t_min, double t_max, Material::hit_record& rec) const
{
    Vec3 oc = r.getOrigin() - g_center;
    auto a = r.getDirection().dot(r.getDirection());
    auto half_b = r.getDirection().dot(oc);
    auto c = oc.dot(oc) - (g_radius * g_radius);

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    Vec3 outward_normal = (rec.p - g_center) / g_radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = g_mat;
    return true;
}
