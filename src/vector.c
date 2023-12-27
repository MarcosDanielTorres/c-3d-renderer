#include <math.h>
#include "vector.h"

vec3_t vec3_rotate_x(vec3_t v, float angle){
    vec3_t rotated_vector = {
        .x = v.x, 
        .y = v.y * cos(angle) - v.z * sin(angle), 
        .z = v.y * sin(angle) + v.z * cos(angle), 
    };
    return rotated_vector;
    
}

vec3_t vec3_rotate_y(vec3_t v, float angle){
    vec3_t rotated_vector = {
        .x = v.x * cos(angle) + v.z * sin(angle), 
        .y = v.y, 
        .z = -v.x * sin(angle) + v.z * cos(angle), 
    };
    // vec3_t rotated_vector = {
    //     .x = v.x * cos(angle) - v.z * sin(angle), 
    //     .y = v.y, 
    //     .z = v.x * sin(angle) + v.z * cos(angle), 
    // };
    return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle){
    vec3_t rotated_vector = {
        .x = v.x * cos(angle) - v.y * sin(angle), 
        .y = v.x * sin(angle) + v.y * cos(angle), 
        .z = v.z, 
    };
    return rotated_vector;

}
// Given vector: (1.0, 1.5, 2.0)
//
// 0.80, 1.25, 2.02 with matrix first x 45 and then y 30
// 1.20710678 1.85355339 1.35355339 with matrix first y 30 and then x 45 
//
// 0.6124, 0.7071, 0.3536


