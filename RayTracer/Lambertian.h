#pragma once
#include "Material.h"
class Lambertian :
    public Material
{
public:
    Lambertian(const Color3& a) : albedo(a) {}

    

public:
    Color3 albedo;

    // Inherited via Material
    virtual bool scatter(Ray& r_in, hit_record& rec, Color3& attenuation, Ray& scattered)  override;
};

