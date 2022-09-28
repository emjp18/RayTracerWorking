#pragma once
#include "Material.h"



class Hittable
{
public:
	virtual bool hit(Ray& r, double t_min, double t_max, Material::hit_record& rec) const = 0;
    std::shared_ptr<Material> g_mat;
};

