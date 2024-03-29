//
// Created by antoni on 11/23/22.
//

#ifndef RAY_TRACING_MOVING_SPHERE_H
#define RAY_TRACING_MOVING_SPHERE_H

#include "hittable.h"
#include "rt_header.h"
#include "aabb.h"

class moving_sphere : public hittable {
public:
    moving_sphere();

    moving_sphere(
            point3 cen0, point3 cen1, double _time0, double _time1, double r, std::shared_ptr <material> m)
            : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m) {};

    bool hit(
            const ray &r, double t_min, double t_max, hit_record &rec) const override;

    virtual bool bounding_box(double t0, double t1, aabb &output_box) const override;

    point3 center(double time) const;

public:
    point3 center0, center1;
    double time0, time1;
    double radius;
    std::shared_ptr <material> mat_ptr;
};

point3 moving_sphere::center(double time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    vec3 oc = r.origin() - center(r.time());
    // get discriminant to sphere
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - std::pow(radius, 2);
    auto discriminant = std::pow(half_b, 2) - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = std::sqrt(discriminant);

    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    auto outward_normal = (rec.p - center(r.time())) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    return true;
}

bool moving_sphere::bounding_box(double t0, double t1, aabb &output_box) const {
    aabb box0(
            center(t0) - vec3(radius, radius, radius),
            center(t0) + vec3(radius, radius, radius));
    aabb box1(
            center(t1) - vec3(radius, radius, radius),
            center(t1) + vec3(radius, radius, radius));
    output_box = surrounding_box(box0, box1);
    return true;
}

#endif //RAY_TRACING_MOVING_SPHERE_H
