#include "HittableList.h"

bool HittableList::hit(Ray& r, double t_min, double t_max, Material::hit_record& rec) const
{
    Material::hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : g_list) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
