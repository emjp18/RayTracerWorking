#pragma once
#include "Material.h"
class Metal :
    public Material
{
public:
    Metal(const Color3& a) : albedo(a) {}
    // Inherited via Material
    virtual bool scatter(Ray& r_in, hit_record& rec, Color3& attenuation, Ray& scattered) override;

public:
    Color3 albedo;
};

