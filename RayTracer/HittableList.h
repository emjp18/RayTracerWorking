#pragma once
#include "Hittable.h"
class HittableList :
    public Hittable
{
public:
    HittableList(std::vector<Hittable*>& list) : g_list(list){}
    // Inherited via Hittable
    virtual bool hit(Ray& r, double t_min, double t_max, Material::hit_record& rec) const override;

    std::vector<Hittable*> g_list;
};

