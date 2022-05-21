//
// Created by antoni on 21/05/22.
//

#ifndef RAY_TRACING_COLOUR_H
#define RAY_TRACING_COLOUR_H

#include "vec3.h"
#include <iostream>

void write_colour(std::ostream &out, colour pixel_colour){
    out << static_cast<int>(255.999 * pixel_colour.x()) << " "
            << static_cast<int>(255.999 * pixel_colour.y()) << " "
            << static_cast<int>(255.999 * pixel_colour.z()) << "\n";
}

#endif //RAY_TRACING_COLOUR_H
