#include "Material.h"
Vec3 Material::random_in_unit_sphere() {
    while (true) {
        auto p = Vec3::random(-1, 1);
        if (p.dot(p) >= 1) continue;
        return p;
    }
}

Vec3 Material::random_unit_vector() {
    Vec3 vec3 = random_in_unit_sphere();
    vec3.normalize();
    return vec3;
}