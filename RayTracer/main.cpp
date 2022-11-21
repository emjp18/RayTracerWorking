#define STB_IMAGE_IMPLEMENTATION
#include "stb_image\stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image\stb_image_write.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Camera.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"
const std::string NAME = "RayTracedImage";
constexpr int SAMPLES_PER_PIXEL =100;
constexpr int MAX_DEPTH = 50;
struct CAMERA
{
    double viewport_height = 2.0;
    double viewport_width = ASPECT_RATIO * viewport_height;
    double focal_length = 1.0;

    Vec3 origin;
    Vec3 horizontal = Vec3(viewport_width, 0, 0);
    Vec3 vertical = Vec3(0, viewport_height, 0);
    Vec3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);
};
Vec3 random_in_unit_sphere();
Vec3 random_unit_vector();
Color3 ray_color(Ray& r);
Color3 ray_color(Ray& r, HittableList& world, int depth);
void initHittables(std::vector<Hittable*>& list);
void initHittablesName(std::vector<Hittable*>& list);
void releaseHittables(std::vector<Hittable*>& list);
void image1(uint8_t* data, std::string& text);
void image2(uint8_t* data, std::string& text);
void image3(uint8_t* data, std::string& text, HittableList& list);

void writePPM(const char* data, size_t len, std::string name);
void writePNG(void* data, std::string name);
double hit_sphere( Vec3& center, double radius,  Ray& r);
int main()
{
    std::vector<Hittable*> hittables;
    //initHittables(hittables);
    initHittablesName(hittables);
    HittableList world(hittables);
    uint8_t* data = new uint8_t[(IMAGE_WIDTH * IMAGE_HEIGHT)*3];
    
    std::string text = "\n";
    //image2(data, text);
    image3(data, text, world);

    writePPM(text.c_str(), strlen(text.c_str()) + 1, NAME);
    writePNG(data, NAME);
    
    delete[] data;
    data = nullptr;
    releaseHittables(hittables);
    return 0;
}

void releaseHittables(std::vector<Hittable*>& list)
{
    for (int h = 0; h < list.size(); h++)
    {
        delete list[h];
        list[h] = nullptr;
    }
}

void image1(uint8_t* data, std::string& text)
{
    int c = 0;
    for (int j = IMAGE_HEIGHT - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < IMAGE_WIDTH; ++i)
        {
            auto r = double(i) / (IMAGE_WIDTH - 1);
            auto g = double(j) / (IMAGE_HEIGHT - 1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            data[c++] = ir;
            data[c++] = ig;
            data[c++] = ib;
            text += (std::to_string(ir) + " " + std::to_string(ig) + " " + std::to_string(ib) + "\n");
        }
    }
    std::cerr << "\nDone.\n";
}

void image2(uint8_t* data, std::string& text)
{
    CAMERA cam;
    int c = 0;
    for (int j = IMAGE_HEIGHT - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < IMAGE_WIDTH; ++i)
        {
            auto u = double(i) / (IMAGE_WIDTH - 1);
            auto v = double(j) / (IMAGE_HEIGHT - 1);
            Ray r(cam.origin, cam.lower_left_corner + (cam.horizontal * u) + (cam.vertical * v) - cam.origin);
            Color3 pixel_color = ray_color(r);
            text += (std::to_string((256 * clamp(pixel_color.g_x, 0.0, 0.999))) + " " + std::to_string((256 * clamp(pixel_color.g_y, 0.0, 0.999))) + " " + std::to_string((256 * clamp(pixel_color.g_z, 0.0, 0.999))) + "\n");
            data[c++] = (uint8_t)(255.999 * pixel_color.g_x);
            data[c++] = (uint8_t)(255.999 * pixel_color.g_y);
            data[c++] = (uint8_t)(255.999 * pixel_color.g_z);
        }
    }

    std::cerr << "\nDone.\n";
}

void image3(uint8_t* data, std::string& text, HittableList& list)
{
    Camera cam(90.0, Vec3(13, 2, 3), Vec3(0, 0, 0), Vec3(0, 1, 0));
    int c = 0;
    for (int j = IMAGE_HEIGHT - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < IMAGE_WIDTH; ++i)
        {
            auto u = double(i) / (IMAGE_WIDTH - 1);
            auto v = double(j) / (IMAGE_HEIGHT - 1);
            Color3 pixel_color;
            for (int s = 0; s < SAMPLES_PER_PIXEL; ++s) {
                auto u = (i + random_double()) / (IMAGE_WIDTH - 1);
                auto v = (j + random_double()) / (IMAGE_HEIGHT - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, list, MAX_DEPTH);
            }
            pixel_color *= 1.0 / (double)SAMPLES_PER_PIXEL;
            //gamma 2
            pixel_color.g_x = std::sqrt(pixel_color.g_x);
            pixel_color.g_y = std::sqrt(pixel_color.g_y);
            pixel_color.g_z = std::sqrt(pixel_color.g_z);
            text += (std::to_string((256 * clamp(pixel_color.g_x, 0.0, 0.999))) + " " + std::to_string((256 * clamp(pixel_color.g_y, 0.0, 0.999))) + " " + std::to_string((256 * clamp(pixel_color.g_z, 0.0, 0.999))) + "\n");
            data[c++] = (uint8_t)(256 * clamp(pixel_color.g_x, 0.0, 0.999));
            data[c++] = (uint8_t)(256 * clamp(pixel_color.g_y, 0.0, 0.999));
            data[c++] = (uint8_t)(256 * clamp(pixel_color.g_z, 0.0, 0.999));
        }
    }

    std::cerr << "\nDone.\n";
}





void writePPM(const char* data, size_t len, std::string name)
{
    std::ofstream file;

    file.open("../"+ name+".ppm");
    std::string headerPath = "P3\n" + std::to_string(IMAGE_WIDTH) + " " + std::to_string(IMAGE_HEIGHT) + "\n255";
    file.write(headerPath.c_str(), strnlen_s(headerPath.c_str(),9999)+1);
    file.write(data, len);
    
    file.close();

}

void writePNG(void* data, std::string name)
{
    std::string path = "../" + name + ".png";
    stbi_write_png(path.c_str(), IMAGE_WIDTH, IMAGE_HEIGHT, 3, data, IMAGE_WIDTH * 3);

}
double hit_sphere( Vec3& center, double radius,  Ray& r)
{
    Vec3 oc = r.getOrigin() - center;
    auto a = r.getDirection().dot(r.getDirection());
    auto b = 2.0 * (oc.dot(r.getDirection()));
    auto c = oc.dot(oc) - (radius * radius);
    auto discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}
Color3 ray_color(Ray& r)
{
    Vec3 c = Vec3(0, 0, -1);
    auto t = hit_sphere(c, 0.5, r);
    if (t > 0.0) {
        Vec3 N = Vec3(r.at(t) - Vec3(0, 0, -1));
        N.normalize();
        return  Color3(N.g_x + 1, N.g_y + 1, N.g_z + 1) * 0.5;
    }
    Vec3 unit_direction = r.getDirection();
    unit_direction.normalize();
    t = 0.5 * (unit_direction.g_y + 1.0);
    return (Color3(1.0, 1.0, 1.0)* (1.0 - t)) + (Color3(0.5, 0.7, 1.0) * t);
}


Color3 ray_color(Ray& r, HittableList& world, int depth)
{
    Material::hit_record rec;
    if (depth <= 0)
        return Color3(0, 0, 0);
    if (world.hit(r,0.001, infinity, rec)) {
        Ray scattered;
        Color3 attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return Color3(0, 0, 0);
    }
    Vec3 unit_direction = r.getDirection();
    unit_direction.normalize();
    auto t = 0.5 * (unit_direction.g_y + 1.0);
    return  (Color3(1.0, 1.0, 1.0) * (1.0 - t)) + (Color3(0.5, 0.7, 1.0) * t);
}

void initHittables(std::vector<Hittable*>& list)
{
    auto ground_material = std::make_shared<Lambertian>(Color3(0.5, 0.5, 0.5));
    Hittable* h = new Sphere(Vec3(0, -1000, 0), 1000, ground_material);
    list.push_back(h);
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color3::random() * Color3::random();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    Hittable* h2 = new Sphere(center, 0.2, sphere_material);
                    list.push_back(h2);
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color3::random(0.5, 1);
                    sphere_material = std::make_shared<Metal>(albedo);
                    Hittable* h2 = new Sphere(center, 0.2, sphere_material);
                    list.push_back(h2);
                }
                else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    Hittable* h2 = new Sphere(center, 0.2, sphere_material);
                    list.push_back(h2);
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    Hittable* h3 = new Sphere(Vec3(0, 1, 0), 1.0, material1);
    list.push_back(h3);
    auto material2 = std::make_shared<Lambertian>(Color3(0.4, 0.2, 0.1));
    Hittable* h4 = new Sphere(Vec3(-4, 1, 0), 1.0, material2);
    list.push_back(h4);
    auto material3 = std::make_shared<Metal>(Color3(0.7, 0.6, 0.5));
    Hittable* h5 = new Sphere(Vec3(4, 1, 0), 1.0, material3);
    list.push_back(h5);
}

void initHittablesName(std::vector<Hittable*>& list)
{
    auto ground_material = std::make_shared<Lambertian>(Color3(0.5, 0.5, 0.5));
    Hittable* h = new Sphere(Vec3(0, -1000, 0), 1000, ground_material);
    list.push_back(h);
   
    double xyarr[16][3] = { {0,0,0},{0,0.4,0.4},{0,0.8,0.4},{0,0.12,0.4},{0,0.16,0.4},{0.4,0,0.4},{0.4,0.4,0.4},{0.4,0.8,0}
    ,{0.4,0.16,0} ,{0.12,0.16,0} ,{0.16,0.16,0} ,{0.16,0.12,0} ,{0.16,0.8,0} ,{0.16,0.4,0} ,{0.12,0.2,0} ,{0.8,0.4,0} };
    int c = 0;
    for (int a = -2; a < 2; a++) {
        for (int b = -2; b < 2; b++) {
            auto choose_mat = random_double();
            Vec3 center(xyarr[c][0]+a*2, xyarr[c][1],xyarr[c][1]+b*2 );
            c++;
            std::shared_ptr<Material> sphere_material;
            center = center;
            if (choose_mat < 0.8) {
                // diffuse
                auto albedo = Color3::random() * Color3::random();
                sphere_material = std::make_shared<Lambertian>(albedo);
                Hittable* h2 = new Sphere(center, 0.2, sphere_material);
                list.push_back(h2);
            }
            else if (choose_mat < 0.95) {
                // metal
                auto albedo = Color3::random(0.5, 1);
                sphere_material = std::make_shared<Metal>(albedo);
                Hittable* h2 = new Sphere(center, 0.2, sphere_material);
                list.push_back(h2);
            }
            else {
                // glass
                sphere_material = std::make_shared<Dielectric>(1.5);
                Hittable* h2 = new Sphere(center, 0.2, sphere_material);
                list.push_back(h2);
            }
            if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
                
            }
        }
    }
    
    auto material1 = std::make_shared<Dielectric>(1.5);
    Hittable* h3 = new Sphere(Vec3(0, 1, 0), 1.0, material1);
    list.push_back(h3);
   
}

Vec3 random_in_unit_sphere() {
    while (true) {
        auto p = Vec3::random(-1, 1);
        if (p.dot(p) >= 1) continue;
        return p;
    }
}

Vec3 random_unit_vector() {
    Vec3 vec3 = random_in_unit_sphere();
    vec3.normalize();
    return vec3;
}