#pragma once
#include "Material.h"
class Dielectric :
    public Material
{
public:
    Dielectric(double index_of_refraction) : ir(index_of_refraction) {}
    // Inherited via Material
    virtual bool scatter(Ray& r_in, hit_record& rec, Color3& attenuation, Ray& scattered) override;


public:
    double ir; // Index of Refraction

private:

    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

