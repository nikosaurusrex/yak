#ifndef YMATH_H
#define YMATH_H

#include "yakpch.h"

struct mat4 {
    union {
        f32 data[4][4] = {0};

        struct {
            f32 m00, m01, m02, m03;
            f32 m10, m11, m12, m13;
            f32 m20, m21, m22, m23;
            f32 m30, m31, m32, m33;
        };
    };
    mat4();
    mat4(f32 value);

    static mat4 identity();
    static mat4 orthographic(f32 left, f32 right, f32 top, f32 bottom, f32 near, f32 far);
    static mat4 translation(f32 x, f32 y);
    static mat4 scale(f32 x, f32 y);
    static mat4 translation_scale(f32 tx, f32 ty, f32 sx, f32 sy);
};

struct vec2 {
    f32 x;
    f32 y;
};

#endif