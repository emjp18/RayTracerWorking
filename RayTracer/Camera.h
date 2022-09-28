#pragma once
#include "Ray.h"
class Camera
{
public:
    Camera(double vFov, Vec3 lookfrom,
        Vec3 lookat,
        Vec3   vup ) //fov is in degrees
    {
        auto theta = degrees_to_radians(vFov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = ASPECT_RATIO * viewport_height;
        auto focal_length = 1.0;

        auto w = (lookfrom - lookat);
        w.normalize();
        auto u = vup.cross(w);
        u.normalize();
        auto v = w.cross(u);

        origin = lookfrom;
        horizontal =  u* viewport_width;
        vertical =  v* viewport_height;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
    }

    Ray& get_ray(double s, double t) 
    {
        Ray r = Ray(origin, lower_left_corner +  (horizontal*s) +  (vertical*t) - origin);
        return r;
    }

private:
    Vec3 origin;
    Vec3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
};

