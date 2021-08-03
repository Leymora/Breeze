
// glTF is really amazing but holy shit it makes zero sense to me as I'm writing this.
// Big thanks to Syoyo for their tinygltfloader that I studied and sometimes straight up copied in order
// for this shit to work. Check out their work: https://github.com/syoyo/tinygltfloader/
#pragma once
#include <cassert>
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "stb_image.h"
#include "picojson.h"

namespace glTFimporter
{

#define GLTF_MODE_POINTS            (0)
#define GLTF_MODE_LINE              (1)
#define GLTF_MODE_LINE_LOOPS        (2)
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
#define GLTF_COMPONENT_TYPE_DOUBLE              (5127)

#define GLTF_TEXTURE_FILTER_NEAREST                 (9728)
#define GLTF_TEXTURE_FILTER_LINEAR                  (9729)
#define GLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST  (9984)
#define GLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST   (9985)
#define GLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR   (9986)
#define GLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR    (9987)

//Parameters
#define GLTF_PARAMETER_TYPE_BYTE                (5120)
#define GLTF_PARAMETER_TYPE_UNSIGNED_BYTE       (5121)
#define GLTF_PARAMETER_TYPE_SHORT               (5122)
#define GLTF_PARAMETER_TYPE_UNSIGNED_SHORT      (5123)
#define GLTF_PARAMETER_TYPE_INT                 (5124)
#define GLTF_PARAMETER_TYPE_UNSIGNED_INT        (5125)
#define GLTF_PARAMETER_TYPE_FLOAT               (5126)
#define GLTF_PARAMETER_TYPE_DOUBLE              (5127)

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

    class scene
    {
        std::string name;
    };

    class glTFImporter
    {

    public:
        glTFImporter();

    };

}

