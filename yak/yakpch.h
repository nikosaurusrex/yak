#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <any>
#include <set>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

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

using string = std::string;

template <typename T>
using array = std::vector<T>;

template <typename K, typename V>
using map = std::unordered_map<K, V>;

template <typename A, typename B>
using pair = std::pair<A, B>;

template <typename T>
using set = std::set<T>;

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
