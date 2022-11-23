//
// Created by antoni on 21/05/22.
//

#ifndef RAY_TRACING_SPHERE_H
#define RAY_TRACING_SPHERE_H

#include <utility>

#include "vec3.h"
#include "hittable.h"

class sphere : public hittable {
public :
    sphere() = default;

    sphere(point3 cen, double r, std::shared_ptr<material> mat) : center(cen), radius(r), mat_ptr(std::move(mat)) {};

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    virtual bool bounding_box(double t0, double t1, aabb &output_box) const override;

public:
    point3 center;
    double radius{};
    std::shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return false;
    }
    auto sqrtd = sqrt(discriminant);

    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || root > t_max) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || root > t_max) {
            return false;
        }
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    return true;
}

bool sphere::bounding_box(double t0, double t1, aabb &output_box) const {
    output_box = aabb(
            center - vec3(radius, radius, radius),
            center + vec3(radius, radius, radius));
    return true;
}

#endif //RAY_TRACING_SPHERE_H
