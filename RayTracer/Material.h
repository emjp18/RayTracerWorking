#pragma once
#include "Ray.h"


class Material
{
public:
    struct hit_record {
        Vec3 p;
        Vec3 normal;
        double t;
        bool front_face;
        std::shared_ptr<Material> mat;
        inline void set_face_normal(Ray& r, Vec3& outward_normal) {
            front_face = r.getDirection().dot(outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
    };

public:
    virtual bool scatter(
         Ray& r_in,  hit_record& rec, Color3& attenuation, Ray& scattered
    )  = 0;

protected:
    Vec3 random_in_unit_sphere();
    Vec3 random_unit_vector();
};

