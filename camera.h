//
// Created by antoni on 25/05/22.
//

#ifndef RAY_TRACING_CAMERA_H
#define RAY_TRACING_CAMERA_H

#include "rt_header.h"

class camera {
    public:
        camera(
                point3 lookfrom,
                point3 lookat,
                vec3 vup,
                double v_fov,
                double aspect_ratio,
                double aperture,
                double focus_dist,
                double _t1 = 0,
                double _t2 = 0
                ) {
            auto theta = deg_to_rad(v_fov);
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

            lens_radius = aperture / 2;
            time0 = _t1;
            time1 = _t2;
        }

        ray get_ray(double s, double t) const {
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();

            return ray(
                    origin + offset,
                    lower_left_corner + s*horizontal + t*vertical - origin - offset,
                    random_double(time0, time1)
            );}

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lens_radius;
        double time0, time1;  // shutter open/close times
};

#endif //RAY_TRACING_CAMERA_H
