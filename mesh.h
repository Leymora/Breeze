#pragma once
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include "shader.h"

//OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using json = nlohmann::json;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	unsigned int id;
	std::string type;
};

class Mesh
{
public:

	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	std::vector<Texture>		textures;

	Mesh(std::string path);
	void Draw(Shader &shader);


private:

	//Render data
	unsigned int VAO, VBO, EBO;
	void loadMesh(std::string path);
	void setupMesh();
};