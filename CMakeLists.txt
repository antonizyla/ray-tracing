cmake_minimum_required(VERSION 3.22)
project(ray_tracing)

set(CMAKE_CXX_STANDARD 23)

add_executable(ray_tracing main.cpp vec3.h colour.h ray.h hittable.h sphere.h hittable_list.h rt_header.h camera.h material.h random_scene.h)
