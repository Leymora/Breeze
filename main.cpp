#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#include <iostream>


//Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void toggleWireframeMode(GLFWwindow* window);

void processInput(GLFWwindow* window);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

//User Settings
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;
bool bWireframe = false;

const char *vertexShaderSource = "#version 460 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"out vec3 vertexColor;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos, 1.0f);\n"
	"	vertexColor = aColor;\n"
	"}\0";


const char *fragmentShaderSource = "#version 460 core\n"
	"out vec4 FragColor;\n"
	"in vec3 vertexColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(vertexColor, 1.0f);\n"
	"}\0";

const char *fragmentShaderSource2 = "#version 460 core\n"
	"out vec4 FragColor;\n"
	"uniform vec4 ourColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = ourColor;\n"
	"}\0";


int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breeze One", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "GLFW window creation failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 1280, 720);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Shader defaultShader("shaders/default.vtxs", "shaders/default.frgs");
	Shader breathingShader("shaders/default.vtxs", "shaders/breathing.frgs");

	float triangleOne[] =
	{
		// Positions			// Colors
		-0.9f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, // Bottom Right
		-0.0f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, // Bottom Left
		-0.45f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f  // Top
	};

	float triangleTwo[] =
	{
		0.5f, -0.5f, 0.0f,
		1.0f, -0.5f, 0.0f,
		0.8f, 0.5f, 0.0f
	};

	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	// ######## Triangle One Setup ##########
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleOne), triangleOne, GL_STATIC_DRAW);
	//Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Color Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// ######## Triangle Two Setup ##########
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleTwo), triangleTwo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//############### RENDER LOOP #################
	while (!glfwWindowShouldClose(window))
	{
		//Check for user input (Keystrokes, mouse movement, etc..)
		processInput(window); // Inputs that needs to be held down (example: W,A,S,D for movement) gets checked here
		glfwSetKeyCallback(window, key_callback); // Inputs that need to be checked only once when pressed (example: ESC for opening the menu) gets checked here

		//Rendering
		glClearColor(0.5f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		defaultShader.use(); // Default shader
		defaultShader.setFloat("xOffset", (sin(glfwGetTime()) / 2.0f) + 0.5f);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Update shaderProgram2's uniform attribute
		float timeValue = glfwGetTime();
		float redValue = (sin(timeValue) / 2.0f) + 0.5f;
		breathingShader.use();
		breathingShader.setFloat4("ourColor",redValue, 0.0f, 1.0f, 1.0f);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Swap the buffers and check/call events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//############# END OF RENDER LOOP ###############



	//End of Main

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);

	glfwTerminate();
	return 0;

}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void toggleWireframeMode(GLFWwindow* window)
{
		bWireframe = !bWireframe;
		if (bWireframe == true)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glfwSetWindowTitle(window, "Brezee One - Wireframe mode");
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glfwSetWindowTitle(window, "Brezee One");			
		}
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		toggleWireframeMode(window);
	}
	
}