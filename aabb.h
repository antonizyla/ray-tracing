//
// Created by antoni on 11/23/22.
//

#ifndef RAY_TRACING_AABB_H
#define RAY_TRACING_AABB_H

#include "rt_header.h"

class aabb {
public:
    aabb() = default;

    aabb(const point3 &a, const point3 &b) {
        minimum = a;
        maximum = b;
    }

    [[nodiscard]] point3 min() const { return minimum; }

    [[nodiscard]] point3 max() const { return maximum; }

    [[nodiscard]] bool hit(const ray &r, double t_min, double t_max) const;

public:
    point3 minimum;
    point3 maximum;
};

inline bool aabb::hit(const ray &r, double t_min, double t_max) const {
    for (int a = 0; a < 3; a++) {
        auto invD = 1.0f / r.direction()[a];
        auto t0 = (min()[a] - r.origin()[a]) * invD;
        auto t1 = (max()[a] - r.origin()[a]) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min)
            return false;
    }
    return true;
}

aabb surrounding_box(aabb box0, aabb box1){
    point3 small(fmin(box0.min().x(), box1.min().x()),
                 fmin(box0.min().y(), box1.min().y()),
                 fmin(box0.min().z(), box1.min().z()));

    point3 big(fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));

    return {small,big};
}

#endif //RAY_TRACING_AABB_H
