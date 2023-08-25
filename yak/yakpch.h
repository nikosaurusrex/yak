#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string_view>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <any>
#include <utility>

#ifdef _WIN32
#define GLEW_STATIC
    #include <glew/glew.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

#include <glfw/glfw3.h>

#include "utility/log.h"

using string = std::string_view;

template <typename T>
using array = std::vector<T>;

template <typename K, typename V>
using map = std::unordered_map<K, V>;

template <typename A, typename B>
using pair = std::pair<A, B>;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int64_t  s64;
typedef int32_t  s32;
typedef int16_t  s16;
typedef int8_t   s8;

typedef float   f32;
typedef double  f64;