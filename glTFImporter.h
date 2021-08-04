
// glTF is really amazing but holy shit it makes zero sense to me as I'm writing this.
// Big thanks to Syoyo for their tinygltfl code that I studied and sometimes (a lot of times) straight up 
// copied in order for this shit to work. Check out their work: https://github.com/syoyo/tinygltf
// As well as KhronosGroup's glTF 2.0 specifications github: https://github.com/KhronosGroup/glTF/tree/master/specification/2.0

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

#define GLTF_NOEXCEPT noexcept
#define DEFAULT_METHODS(x)              \
  ~x() = default;                       \
  x(const x &) = default;               \
  x(x &&) GLTF_NOEXCEPT = default;      \
  x &operator=(const x &) = default;    \
  x &operator=(x &&) GLTF_NOEXCEPT = default;

namespace glTFimp
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

    Value() : type_(NULL_TYPE), int_value_(0), real_value_(0.0), bool_value_(false) {}
    
    explicit Value(bool b) : type_(BOOL_TYPE) { bool_value_ = b; }
    explicit Value(int i) : type_(INT_TYPE) { int_value_ = i; real_value_ = i; }
    explicit Value(double n) : type_(REAL_TYPE) { real_value_ = n; }
    explicit Value(const std::string &s) : type_(STRING_TYPE) { string_value_ = s; }
    explicit Value(std::string &&s) : type_(STRING_TYPE), string_value_(std::move(s)){ }
    explicit Value(const unsigned char *p, size_t n) : type_(BINARY_TYPE) { binary_value_.resize(n); memcpy(binary_value_.data(), p, n); }
    explicit Value(std::vector<unsigned char> &&v) noexcept : type_(BINARY_TYPE), binary_value_(std::move(v)) { }
    explicit Value(const Array &a) : type_(ARRAY_TYPE) { array_value_ = a; }
    explicit Value(Array &&a) noexcept : type_(ARRAY_TYPE), array_value_(std::move(a)) { }
    explicit Value(const Object &o) : type_(OBJECT_TYPE) { object_value_ = o; }
    explicit Value(Object &&o) noexcept : type_(OBJECT_TYPE), object_value_(std::move(o)) { }

    DEFAULT_METHODS(Value)

    char Type() const { return static_cast<char>(type_); }

    bool IsBool() const { return (type_ == BOOL_TYPE); }
    bool IsInt() const { return (type_ == INT_TYPE); }
    bool IsReal() const { return (type_ == REAL_TYPE); }
    bool IsNumber() const { return (type_ == REAL_TYPE) || (type_ == INT_TYPE); }
    bool IsString() const { return (type_ == STRING_TYPE); }
    bool IsBinary() const { return (type_ == BINARY_TYPE); }
    bool IsArray() const { return (type_ == ARRAY_TYPE); }
    bool IsObject() const { return (type_ == OBJECT_TYPE); }

    double IntToDouble() const
    {
        if (type_ == INT_TYPE)
            return double(int_value_);
        else
            return real_value_;
    }

    int DoubleToInt() const
    {
        if (type_ == REAL_TYPE)
            return int(real_value_);
        else
            return int_value_;
    }


    //######################################## ACCESSOR ########################################
    template <typename T>
    const T &Get() const;
    template <typename T>
    T &Get();


    //Lookup a value from an array
    const Value &Get(int index) const
    {
        static Value null_value;
        assert(IsArray());
        assert(index >= 0);
        return (static_cast<size_t>(index) < array_value_.size()) ? array_value_[static_cast<size_t>(index)] : null_value;
    }

    //Lookup a value from a key-value pair
    const Value &Get(const std::string &key) const
    {
        static Value null_value;
        assert(IsObject());
        Object::const_iterator it = object_value_.find(key);
        return (it != object_value_.end()) ? it->second : null_value;
    }

    size_t ArrayLength() const
    {
        if (!IsArray())
            return 0;

        return array_value_.size();
    }

    bool Has(const std::string &key) const
    {
        if (!IsObject())
            return false;
        
        Object::const_iterator it = object_value_.find(key);
        return (it != object_value_.end()) ? true : false;
    }

    //List those keys!
    std::vector<std::string> Keys() const
    {
        std::vector<std::string> keys;
        if (!IsObject())
            return keys;

        for (Object::const_iterator it = object_value_.begin(); it != object_value_.end(); ++it)
        {
            keys.push_back(it->first);
        }

        return keys;
    }

    size_t Size() const { return (IsArray() ? ArrayLength() : Keys().size()); }

    bool operator==(const glTFimp::Value &other) const;

protected:
    int type_ = NULL_TYPE;
    int int_value_ = 0;
    double real_value_ = 0;
    std::string string_value_ = "";
    std::vector<unsigned char> binary_value_;
    Array array_value_;
    Object object_value_;
    bool bool_value_ = false;
};

    #define GLTF_VALUE_GET(ctype, var)              \
    template <>                                     \
    inline const ctype &Value::Get<ctype>() const   \
    {                                               \
        return var;                                 \
    }                                               \
    template <>                                     \
    inline ctype &Value::Get<ctype>()               \
    {                                               \
        return var;                                 \
    }                                               
    GLTF_VALUE_GET(bool, bool_value_);
    GLTF_VALUE_GET(double, real_value_);
    GLTF_VALUE_GET(int, int_value_);
    GLTF_VALUE_GET(std::string, string_value_);
    GLTF_VALUE_GET(std::vector<unsigned char>, binary_value_);
    GLTF_VALUE_GET(Value::Array, array_value_);
    GLTF_VALUE_GET(Value::Object, object_value_);


    //Object for respresenting a color
    using ColorValue = std::array<double, 4>;

    //Skipping paremeters class here#######
    //#####################################

    typedef std::map<std::string, Value> ExtensionMap;

    struct AnimationChannel
    {
        int sampler;
        int target_node;
        std::string target_path;

        Value extras;
        ExtensionMap extensions;
        ExtensionMap target_extensions;

        //If SetStoreOriginalJSONForExtrasAndExtensions is enabled
        std::string extras_json_string;
        std::string extensions_json_string;
        std::string target_extensions_json_string;

        AnimationChannel() : sampler(-1), target_node(-1) { }
        DEFAULT_METHODS(AnimationChannel)
        bool operator==(const AnimationChannel &) const;
    };

    struct AnimationSampler
    {
        int input;
        int output;
        std::string interpolation;

        Value extras;
        ExtensionMap extensions;

        //If SetStoreOriginalJSONForExtrasAndExtensions is enabled
        std::string extras_json_string;
        std::string extensions_json_string;

        AnimationSampler() : input(-1), output(-1), interpolation("LINEAR") { }
        DEFAULT_METHODS(AnimationSampler)
        bool operator==(const AnimationSampler &) const;
    };

    struct Animation
    {
        std::string name;
        std::vector<AnimationChannel> channels;
        std::vector<AnimationSampler> samples;

        Value extras;
        ExtensionMap extensions;

        //If SetStoreOriginalJSONForExtrasAndExtensions is enabled
        std::string extras_json_string;
        std::string extensions_json_string;

        Animation() = default;
        DEFAULT_METHODS(Animation)
        bool operator==(const Animation &) const;
    };

    struct Skin
    {
        std::string name;
        int inverseBindMatrices;
        int skeleton;
        std::vector<int> joints;

        Value extras;
        ExtensionMap extensions;

        //If SetStoreOriginalJSONForExtrasAndExtensions is enabled
        std::string extras_json_string;
        std::string extensions_json_string;

        Skin()
        {
            inverseBindMatrices = -1;
            skeleton = -1;
        }
        DEFAULT_METHODS(Skin)
        bool operator==(const Skin &) const;
    };

    struct Sampler
    {
        std::string name;
        int magFilter;
        int minFilter;
        int wrapS;
        int wrapT;
        
        Value extras;
        ExtensionMap extensions;

        //If SetStoreOriginalJSONForExtrasAndExtensions is enabled
        std::string extras_json_string;
        std::string extensions_json_string;

        Sampler() : magFilter(-1), minFilter(-1), wrapS(GLTF_TEXTURE_WRAP_REPEAT), wrapT(GLTF_TEXTURE_WRAP_REPEAT) { }
        DEFAULT_METHODS(Sampler)
        bool operator==(const Sampler &) const;

    };

    struct Image
    {
        std::string name;
        int width;
        int height;
        int component;
        int bits;
        int pixelType;
        
        std::vector<unsigned char> image;
        int bufferView;
        std::string mimeType; //Can only be jpeg or png
        std::string uri;
                
        Value extras;
        ExtensionMap extensions;

        //If SetStoreOriginalJSONForExtrasAndExtensions is enabled
        std::string extras_json_string;
        std::string extensions_json_string;

        bool as_is;

        Image() : as_is(false)
        {
            bufferView = -1;
            width = -1;
            height = -1;
            component = -1;
            bits = -1;
            pixelType = -1;
        }
        DEFAULT_METHODS(Image)
        bool operator==(const Image &) const;
    };

    struct Texture
    {
        std::string name;
        int sampler;
        int source;
                        
        Value extras;
        ExtensionMap extensions;

        //If SetStoreOriginalJSONForExtrasAndExtensions is enabled
        std::string extras_json_string;
        std::string extensions_json_string;

        Texture() : sampler(-1), source(-1) { }
        DEFAULT_METHODS(Texture)
        bool operator==(const Texture &) const;
    };


    //https://github.com/syoyo/tinygltf/blob/master/tiny_gltf.h  Rad 417
}