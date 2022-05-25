#include <iostream>

#include "rt_header.h"
#include "colour.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "chrono"

double  hit_sphere(const point3& center, double radius, const ray& ray);
colour ray_colour(const ray& r, const hittable& world, int depth);

int main() {

    //Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    const int samples = 100;
    const int max_depth = 100;

    //World
    hittable_list world;
    auto material_center = std::make_shared<metal>(colour(0.8, 0.0, 0.0), 0.3);
    auto material_ground = std::make_shared<lambertian>(colour(0.3,0.3,0.6));
    auto material_glass = std::make_shared<dieelectric>(1.5);
    auto material_right  = std::make_shared<metal>(colour(0.8, 0.6, 0.2), 0.0);

    world.add(std::make_shared<sphere>(point3(1.0,0.0,-1.0), 0.5, material_center));
    world.add(std::make_shared<sphere>(point3(-1.0,0.0,-1.0),-0.35, material_glass));
    world.add(std::make_shared<sphere>(point3(0.0,0.0,-1.0), 0.5, material_right));
    world.add(std::make_shared<sphere>(point3(0.0,-100.5,-1.0),100, material_ground));
    //world.add(std::make_shared<sphere>(point3(0,1,1),2));

    //Camera
    camera cam;

    // Rendering
    // file header
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    auto start = std::chrono::steady_clock::now();
    // pixel data
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr  << "\r Rows Remaining: " << j << "\n";
        for (int i = 0; i < image_width; ++i){
                colour pixel_colour(0,0,0);
                for (int s = 0; s < samples; ++s){
                    auto u = (i + random_double()) / (image_width-1);
                    auto v = (j + random_double()) / (image_height-1);
                    ray r = cam.get_ray(u,v);
                    pixel_colour += ray_colour(r, world, max_depth);
                }
                write_colour(std::cout, pixel_colour, samples);
            }
        }
    auto end  = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end-start;
    std::cout << "Elapsed time: " << elapsed.count() << std::endl;

    std::cerr << "\n Render Complete\n";
}

// Determines if a ray would hit the sphere using the discriminant
// either return -1 if not hit or the positive R where it did hit
double  hit_sphere(const point3& center, double radius, const ray& ray){
    vec3 oc = ray.origin() - center;
    auto a = ray.direction().length_squared();
    auto half_b = dot(oc, ray.direction());
    auto c = oc.length_squared() -radius*radius;
    // the discriminant whoa
    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0){
        return -1.0;
    }else  {
        return (-half_b - sqrt(discriminant) / a);
    }

}

colour ray_colour(const ray& r, const hittable& world, int depth){
    hit_record rec;

    if (depth<=0) return colour(0,0,0);

    if(world.hit(r,0.001,infty, rec)){
        ray scattered;
        colour attenuation;
        if (rec.mat_ptr->scatter(r, rec,attenuation, scattered))
            return attenuation * ray_colour(scattered, world, (depth-1));
        return colour(0,0,0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() +1.0);
    return (1.0-t)*colour(1.0,1.0,1.0) + t*colour(0.5,0.7,1.0);
}