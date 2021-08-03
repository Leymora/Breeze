
// glTF is really amazing but holy shit it makes zero sense to me as I'm writing this.
// Big thanks to Syoyo for their tinygltfloader that I studied and sometimes straight up copied in order
// for this shit to work. Check out their work: https://github.com/syoyo/tinygltf
#pragma once
#include <cassert>
#include <map>
#include <string>
#include <vector>
#include <cmath>    //std::fabs
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <array>
#include <limits>
#include <functional>


#include "stb_image.h"
#include "nlohmann/json.hpp"


#define DEFAULT_METHODS(x)              \
  ~x() = default;                       \
  x(const x &) = default;               \
  x(x &&) GLTF_NOEXCEPT = default;      \
  x &operator=(const x &) = default;    \
  x &operator=(x &&) GLTF_NOEXCEPT = default;

namespace glTFimporter
{

#define GLTF_MODE_POINTS            (0)
#define GLTF_MODE_LINE              (1)
#define GLTF_MODE_LINE_LOOP         (2)
#define GLTF_MODE_LINE_LOOP_STRIP   (3)
#define GLTF_MODE_TRIANGLES         (4)
#define GLTF_MODE_TRIANGLE_STRIP    (5)
#define GLTF_MODE_TRIANGLE_FAN      (6)

#define GLTF_COMPONENT_TYPE_BYTE                (5120)
#define GLTF_COMPONENT_TYPE_UNSIGNED_BYTE       (5121)
#define GLTF_COMPONENT_TYPE_SHORT               (5122)
#define GLTF_COMPONENT_TYPE_UNSIGNED_SHORT      (5123)
#define GLTF_COMPONENT_TYPE_INT                 (5124)
#define GLTF_COMPONENT_TYPE_UNSIGNED_INT        (5125)
#define GLTF_COMPONENT_TYPE_FLOAT               (5126)
#define GLTF_COMPONENT_TYPE_DOUBLE              (5130)

#define GLTF_TEXTURE_FILTER_NEAREST                 (9728)
#define GLTF_TEXTURE_FILTER_LINEAR                  (9729)
#define GLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST  (9984)
#define GLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST   (9985)
#define GLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR   (9986)
#define GLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR    (9987)

#define GLTF_TEXTURE_WRAP_REPEAT            (10497)
#define GLTF_TEXTURE_WRAP_CLAMP_TO_EDGE     (33071)
#define GLTF_TEXTURE_WRAP_MIRRORED_REPEAT   (33648)

//Parameters
#define GLTF_PARAMETER_TYPE_BYTE                (5120)
#define GLTF_PARAMETER_TYPE_UNSIGNED_BYTE       (5121)
#define GLTF_PARAMETER_TYPE_SHORT               (5122)
#define GLTF_PARAMETER_TYPE_UNSIGNED_SHORT      (5123)
#define GLTF_PARAMETER_TYPE_INT                 (5124)
#define GLTF_PARAMETER_TYPE_UNSIGNED_INT        (5125)
#define GLTF_PARAMETER_TYPE_FLOAT               (5126)
#define GLTF_PARAMETER_TYPE_DOUBLE              (5130)

#define GLTF_PARAMETER_TYPE_FLOAT_VEC2      (35664)
#define GLTF_PARAMETER_TYPE_FLOAT_VEC3      (35665)
#define GLTF_PARAMETER_TYPE_FLOAT_VEC4      (35666)

#define GLTF_PARAMETER_TYPE_INT_VEC2        (35667)
#define GLTF_PARAMETER_TYPE_INT_VEC3        (35668)
#define GLTF_PARAMETER_TYPE_INT_VEC4        (35669)

#define GLTF_PARAMETER_TYPE_BOOL            (35670)           
#define GLTF_PARAMETER_TYPE_BOOL_VEC2       (35671)           
#define GLTF_PARAMETER_TYPE_BOOL_VEC3       (35672)           
#define GLTF_PARAMETER_TYPE_BOOL_VEC4       (35673)           

#define GLTF_PARAMETER_TYPE_FLOAT_MAT2      (35674)
#define GLTF_PARAMETER_TYPE_FLOAT_MAT3      (35675)
#define GLTF_PARAMETER_TYPE_FLOAT_MAT4      (35676)
#define GLTF_PARAMETER_TYPE_SAMPLER_2D      (35678)


#define GLTF_TYPE_VEC2      (2)
#define GLTF_TYPE_VEC3      (3)
#define GLTF_TYPE_VEC4      (4)
#define GLTF_TYPE_MAT2      (32 + 2)
#define GLTF_TYPE_MAT3      (32 + 3)
#define GLTF_TYPE_MAT4      (32 + 4)
#define GLTF_TYPE_SCALAR    (64 + 1)
#define GLTF_TYPE_VECTOR    (64 + 4)
#define GLTF_TYPE_MATRIX    (64 + 16)
#define GLTF_TYPE_FRAGMENT_SHADER   (35632)
#define GLTF_TYPE_VERTEX_SHADER     (35633)

#define GLTF_IMAGE_FORMAT_JPEG  (0)
#define GLTF_IMAGE_FORMAT_PNG   (1)
#define GLTF_IMAGE_FORMAT_BMP   (2)
#define GLTF_IMAGE_FORMAT_GIF   (3)

#define GLTF_TEXTURE_FORMAT_ALPHA       (6406)
#define GLTF_TEXTURE_FORMAT_RGB         (6407)
#define GLTF_TEXTURE_FORMAT_RGBA        (6408)
#define GLTF_TEXTURE_FORMAT_LUM         (6409)
#define GLTF_TEXTURE_FORMAT_LUM_ALPHA   (6410)

#define GLTF_TEXTURE_TARGET_TEXTURE2D       (3553)
#define GLTF_TEXTURE_TYPE_UNSIGNED_BYTE     (5121)

#define GLTF_TARGET_ARRAY_BUFFER            (34962)
#define GLTF_TARGET_ELEMENT_ARRAY_BUFFER    (34963)

#define GLTF_DOUBLE_EPS     (1.e-12)
#define GLTF_DOUBLE_EQUAL   (a, b) (std::fabs((b) - (a) < GLTF_DOUBLE_EPS))

typedef enum
{
    NULL_TYPE,
    REAL_TYPE,
    INT_TYPE,
    BOOL_TYPE,
    STRING_TYPE,
    ARRAY_TYPE,
    BINARY_TYPE,
    OBJECT_TYPE
} Type;

static inline int32_t GetComponentSizeInBytes(uint32_t type)
{
    switch(type)
    {
        case GLTF_COMPONENT_TYPE_BYTE:              return 1; break;
        case GLTF_COMPONENT_TYPE_UNSIGNED_BYTE:     return 1; break;
        case GLTF_COMPONENT_TYPE_SHORT:             return 2; break;
        case GLTF_COMPONENT_TYPE_UNSIGNED_SHORT:    return 2; break;
        case GLTF_COMPONENT_TYPE_INT:               return 4; break;
        case GLTF_COMPONENT_TYPE_UNSIGNED_INT:      return 4; break;
        case GLTF_COMPONENT_TYPE_FLOAT:             return 4; break;
        case GLTF_COMPONENT_TYPE_DOUBLE:            return 8; break;
        default:                                    return -1; break;                           
    }
}
static inline int32_t GetNumOfComponentsInType(uint32_t typeToCheck)
{
    switch(typeToCheck)
    {
        case GLTF_TYPE_SCALAR:  return 1; break;
        case GLTF_TYPE_VEC2:    return 2; break;
        case GLTF_TYPE_VEC3:    return 3; break;
        case GLTF_TYPE_VEC4:    return 4; break;
        case GLTF_TYPE_MAT2:    return 4; break;
        case GLTF_TYPE_MAT3:    return 9; break;
        case GLTF_TYPE_MAT4:    return 16; break;
        default:                return -1; break;                           
    }
}

//Move these fokken functions to glTFImporter class when done
bool IsDataURI(const std::string &in);
bool DecodeDataURI(std::vector<unsigned char> *out, std::string &mime_type, const std::string &in, size_t reqBytes, bool checkSize);


class Value
{

public:
    typedef std::vector<Value> Array;
    typedef std::map<std::string, Value> Object;

    Value() 
        : type_(NULL_TYPE), 
          int_value_(0), 
          real_value_(0.0), 
          bool_value_(false) {}
    
    explicit Value(bool b) : type_(BOOL_TYPE) {bool_value_ = b;}
    explicit Value(int i) : type_(INT_TYPE) {int_value_ = i; real_value_ = i;}
    explicit Value(double n) : type_(REAL_TYPE) {real_value_ = n;}
    explicit Value(const std::string &s) : type_(STRING_TYPE) {string_value_ = s;}
    explicit Value(std::string &&s) : type_(STRING_TYPE), string_value_(std::move(s)){}
    explicit Value();
    explicit Value();
    explicit Value();
    explicit Value();
    explicit Value();
    explicit Value();

protected:
    int type_ = NULL_TYPE;
    int int_value_ = 0;
    double real_value_ = 0;
    std::string string_value_ = "";
    std::vector<unsigned char> binary_value_;
    Array array_value_;
    Object object_value_;
    bool bool_value_
};

}

