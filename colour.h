//
// Created by antoni on 21/05/22.
//

#ifndef RAY_TRACING_COLOUR_H
#define RAY_TRACING_COLOUR_H

#include "vec3.h"
#include <iostream>
#include "vector"


void write_colour(std::ostream &out, colour pixel_colour, int samples) {

    auto r = pixel_colour.x();
    auto g = pixel_colour.y();
    auto b = pixel_colour.z();

    auto scale = 1.0 / samples;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    out << static_cast<int>(256 * clamp(r, 0, 0.999)) << " "
        << static_cast<int>(256 * clamp(g, 0, 0.999)) << " "
        << static_cast<int>(256 * clamp(b, 0, 0.999)) << std::endl;
}

std::string write_colour_str(colour pixel_colour, int samples) {
    auto r = pixel_colour.x();
    auto g = pixel_colour.y();
    auto b = pixel_colour.z();

    auto scale = 1.0 / samples;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    std::ostringstream str;
    str << static_cast<int>(256 * clamp(r, 0, 0.999)) << " "
        << static_cast<int>(256 * clamp(g, 0, 0.999)) << " "
        << static_cast<int>(256 * clamp(b, 0, 0.999));
    return str.str();
}

#endif //RAY_TRACING_COLOUR_H
