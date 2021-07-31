#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include "mesh.h"

using json = nlohmann::json;


class Model
{

public:
    Model(std::string const& path, bool gamma = false);
    void Draw(Shader &shader);

private:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    void loadModel(std::string const& path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma);
};

