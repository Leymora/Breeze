#include "object.h"

Object::Object()
{
    
}

bool Object::loadModel(const char* path)
{
    std::ifstream i(path);
    json j = json::parse(i);

    std::cout << j["meshes"][0]["primitives"][0]["indices"].get<int>();
 
    

    return true;
}