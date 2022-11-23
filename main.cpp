#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <thread>
#include <mutex>

#include "rt_header.h"
#include "colour.h"
#include "hittable_list.h"
#include "camera.h"
#include "chrono"
#include "random_scene.h"

double hit_sphere(const point3 &center, double radius, const ray &ray);

colour ray_colour(const ray &r, const hittable &world, int depth);

void trace_rays(int i, int j, const int samples, const int image_width, const int image_height, const camera &cam,
                const hittable_list &world,
                const int max_depth, std::string &pixel, int &active_threads);


int main(int argc, char **argv) {

    using namespace std::chrono_literals;

    bool multithreaded = false;
    if (argc >= 2) {
        multithreaded = true;
    }

    //Image
    const double aspect_ratio = 3.0 / 2.0;
    const int image_width = 200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples = 20;
    const int max_depth = 10;
    int active_threads = 0;

    //World
    const hittable_list world = random_scene();

    //Camera
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10;
//  auto dist_to_focus = (lookfrom-lookat).length();
    auto aperture = 0.1;

    const camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

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
            std::thread worker(trace_rays, i, j, samples, image_width, image_height, std::ref(cam), std::ref(world),
                               max_depth, std::ref(image[i][j]), std::ref(active_threads));
            if (multithreaded) {
                worker.detach();
                active_threads++;
            } else {
                worker.join();
            }
        }
        std::cerr << "\r Columns Remaining: " << image_width - i << "\n";
    }

    if (multithreaded) { // has to wait for all threads to finish
        std::cerr << "Waiting for all threads to finish (3s)" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    // ppm header
    oss << "P3\n" << image_width << " " << image_height << "\n255\n";
    // image data
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

void trace_rays(int i, int j, const int samples, const int image_width, const int image_height, const camera &cam,
                const hittable_list &world, const int max_depth, std::string &pixel, int &active_threads) {
    colour pixel_colour(0, 0, 0);
    for (int s = 0; s < samples; ++s) {
        auto u = (i + random_double()) / (image_width - 1);
        auto v = (j + random_double()) / (image_height - 1);
        ray r = cam.get_ray(u, v);
        pixel_colour += ray_colour(r, world, max_depth);
    }
    pixel = write_colour_str(pixel_colour, samples);
    active_threads--;
}
