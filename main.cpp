#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>

#include "rt_header.h"
#include "colour.h"
#include "hittable_list.h"
#include "camera.h"
#include "chrono"
#include "random_scene.h"

double hit_sphere(const point3 &center, double radius, const ray &ray);

colour ray_colour(const ray &r, const hittable &world, int depth);

int main() {
    //Image
    const auto aspect_ratio = 1.5;
    const int image_width = 300;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples = 50;
    const int max_depth = 50;

    //World
    auto world = random_scene();

    //Camera
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10;
//    auto dist_to_focus = (lookfrom-lookat).length();
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Files
    std::ofstream file("../Image.ppm");
    std::ostringstream oss;

    // Rendering
    auto start = std::chrono::steady_clock::now();
    // size of each pixel will be 12 bytes as a string
    // doing everything at once
    std::string image[image_width][image_height];
    for (int i = 0; i < image_width; i++) {
        for (int j = 0; j < image_height; j++) {
            image[i][j].resize(12);
            colour pixel_colour(0, 0, 0);
            for (int s = 0; s < samples; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_colour += ray_colour(r, world, max_depth);
            }
            image[i][j] = write_colour_str(pixel_colour, samples);
        }
        std::cerr << "\r cols Remaining: " << image_width - i << "\n";
    }

    // ppm header
    oss << "P3\n" << image_width << " " << image_height << "\n255\n";
    for (int i = image_height - 1; i >= 0; --i) {
        for (int j = 0; j < image_width; j++) {
            oss << image[j][i] << std::endl;
        }
    }
    file << oss.str();


    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cerr << "\nRender Complete\n";
    std::cerr << "Elapsed time: " << elapsed.count() << "s" << std::endl;
    file.close();
}

// Determines if a ray would hit the sphere using the discriminant
// either return -1 if not hit or the positive R where it did hit
double hit_sphere(const point3 &center, double radius, const ray &ray) {
    vec3 oc = ray.origin() - center;
    auto a = ray.direction().length_squared();
    auto half_b = dot(oc, ray.direction());
    auto c = oc.length_squared() - radius * radius;
    // the discriminant whoa
    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant) / a);
    }

}

colour ray_colour(const ray &r, const hittable &world, int depth) {
    hit_record rec;

    if (depth <= 0) return colour(0, 0, 0);

    if (world.hit(r, 0.001, infty, rec)) {
        ray scattered;
        colour attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_colour(scattered, world, (depth - 1));
        return colour(0, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * colour(1.0, 1.0, 1.0) + t * colour(0.5, 0.7, 1.0);
}
