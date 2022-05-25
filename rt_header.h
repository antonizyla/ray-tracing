//
// Created by antoni on 24/05/22.
//

#ifndef RAY_TRACING_RT_HEADER_H
#define RAY_TRACING_RT_HEADER_H

#include "cmath"
#include "limits"
#include "memory"
#include "random"

const double infty = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double deg_to_rad(double degrees){
    return degrees * pi / 180.0;
}

inline double random_double(){
    static std::uniform_real_distribution<double> dist(0.0,1.0);
    static std::mt19937  generator;
    return dist(generator);
}

#include "ray.h"
#include "vec3.h"

#endif //RAY_TRACING_RT_HEADER_H
