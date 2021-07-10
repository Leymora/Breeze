#pragma once
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Line
{

public:

	int shaderProgram;
	unsigned int VBO, VAO;
	std::vector<float> vertices;
	glm::vec3 startPoint;
	glm::vec3 endPoint;
	glm::mat4 MVP;
	glm::vec3 color;

	Line(glm::vec3 start, glm::vec3 end, glm::vec3 color);
	~Line();
	void setMVP(glm::mat4 mvp);
	void setColor(glm::vec3 color);
	void setPos(glm::vec3 start, glm::vec3 end);
	int draw();

private:


};