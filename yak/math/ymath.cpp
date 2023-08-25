#include "ymath.h"

mat4::mat4() {
}

mat4::mat4(f32 value) {
    m00 = value;
    m11 = value;
    m22 = value;
    m33 = value;
}

mat4 mat4::identity() {
    return mat4(1.0);
}

mat4 mat4::orthographic(f32 left, f32 right, f32 top, f32 bottom, f32 near, f32 far) {
    mat4 matrix = mat4::identity();

    matrix.m00 = 2.0f / (right - left);
    matrix.m11 = 2.0f / (top - bottom);
    matrix.m22 = -2.0f / (far - near);
    matrix.m30 = -(right + left) / (right - left);
    matrix.m31 = -(top + bottom) / (top - bottom);
    matrix.m32 = -(far + near) / (far - near);

    return matrix;
}

mat4 mat4::translation(f32 x, f32 y) {
    mat4 matrix = mat4::identity();
    matrix.m03 = x;
    matrix.m13 = y;
    return matrix;
}

mat4 mat4::scale(f32 x, f32 y) {
    mat4 matrix = mat4::identity();
    matrix.m00 = x;
    matrix.m11 = y;
    return matrix;
}

mat4 mat4::translation_scale(f32 tx, f32 ty, f32 sx, f32 sy) {
    mat4 matrix = mat4::identity();
    matrix.m00 = sx;
    matrix.m11 = sy;
    matrix.m30 = tx;
    matrix.m31 = ty;
    return matrix;
}
