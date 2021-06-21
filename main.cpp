
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include "shader.h"
#include "camera.h"

#include <iostream>
#include <filesystem>
#include <zip.h>

//OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void toggleWireframeMode(GLFWwindow* window);

void processInput(GLFWwindow* window);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void unZip(unsigned char* &contents, int &stSize, std::string zipFile, std::string file);


//Engine Stuff
const std::string APP_DATA_PATH = std::filesystem::temp_directory_path().parent_path().parent_path().parent_path().string() += "\\Roaming\\BreezeEngine\\";
const std::string ENGINE_DEFAULTS_PATH = APP_DATA_PATH + "engineDefaults.bpf";
const std::string CURRENT_PATH = std::filesystem::current_path().string() += "\\";

//User Settings
const float SCREEN_WIDTH = 1280.0f;
const float SCREEN_HEIGHT = 720.0f;
bool bWireframe = false;

// Frames Per Second management
float currentFrame 	= 0.0f;	// Calculate deltatime with this value
float deltaTime 	= 0.0f; // Time it took to calculate one frame
float lastFrame 	= 0.0f; // The last frame's time


float mixValue = 0.0f;

// Camera setup
Camera mainCam(glm::vec3(0.0f, 0.0f, 3.0f), 90.0f);

float lastMouseX = SCREEN_WIDTH / 2;
float lastMouseY = SCREEN_HEIGHT / 2;
bool firstMouse = true;



int main()
{

	if(!std::filesystem::exists(APP_DATA_PATH))
		std::filesystem::create_directories(APP_DATA_PATH);
	
	if(!std::filesystem::exists(ENGINE_DEFAULTS_PATH))
	{
		std::cout << "Missing Engine Defaults!\n\nFile: " << APP_DATA_PATH <<  "engineDefaults.bpf\nThis is a required file for Breeze Engine to run\n" << std::endl;
		std::cout << CURRENT_PATH << std::endl;
		return 69;
	}

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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide and lock the mouse cursor to the center of the window
	


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	
	Shader defaultShader((CURRENT_PATH + "shaders/default_vertex.glsl").c_str(), (CURRENT_PATH + "shaders/default_fragment.glsl").c_str());
	Shader breathingShader((CURRENT_PATH + "shaders/default_vertex.glsl").c_str(), (CURRENT_PATH + "shaders/breathing_fragment.glsl").c_str());

	float cube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// ######## Cube Setup ##########
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	//Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Color Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5* sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	//Tex Attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// All Texture setup
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* unzippedTexture = 0;
	int unzippsedTextureSize = 0;
	unsigned char* textureData = 0;


	//Texture 1 ---------------------------------------------
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unZip(unzippedTexture, unzippsedTextureSize, ENGINE_DEFAULTS_PATH, "default_texture.png");

	textureData = stbi_load_from_memory(unzippedTexture, unzippsedTextureSize, &width, &height, &nrChannels, 0);
	if(!textureData)
	{
		unZip(unzippedTexture, unzippsedTextureSize, ENGINE_DEFAULTS_PATH, "fallback_texture.png");
		textureData = stbi_load_from_memory(unzippedTexture, unzippsedTextureSize, &width, &height, &nrChannels, 0);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);



	stbi_image_free(textureData);
	//-------------------------------------------------------

	//Texture 2 ---------------------------------------------
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	unZip(unzippedTexture, unzippsedTextureSize, APP_DATA_PATH + "engineTextures.bpf", "brick.jpg");

	textureData = stbi_load_from_memory(unzippedTexture, unzippsedTextureSize, &width, &height, &nrChannels, 0);

	if(!textureData)
	{
		unZip(unzippedTexture, unzippsedTextureSize, ENGINE_DEFAULTS_PATH, "fallback_texture.png");
		textureData = stbi_load_from_memory(unzippedTexture, unzippsedTextureSize, &width, &height, &nrChannels, 0);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(textureData);


	//-------------------------------------------------------


	defaultShader.use();
	glUniform1i(glGetUniformLocation(defaultShader.ID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(defaultShader.ID, "texture2"), 1);





	//############### GAME LOOP #################
	while (!glfwWindowShouldClose(window))
	{

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Check for user input (Keystrokes, mouse movement, etc..)
		processInput(window); // Inputs that needs to be held down (example: W,A,S,D for movement) gets checked here
		glfwSetKeyCallback(window, key_callback); // Inputs that need to be checked only once when pressed (example: ESC for opening the menu) gets checked here

		//Rendering
		glEnable(GL_DEPTH_TEST);
		glClearColor((float)111/255, (float)242/255, (float)149/255, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glActiveTexture(GL_TEXTURE0); // Activate the texture unit first before binding the texture
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		defaultShader.setFloat("mixValue", mixValue);


		defaultShader.use();
		
		//Matrices ---------------------------------------

		// Model Matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

		// View Matrix
		glm::mat4 view = mainCam.getViewMatrix();

		// Projection Matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(mainCam.cameraFOV), SCREEN_WIDTH / SCREEN_HEIGHT, 0.01f, 100.0f);


		int modelLoc = glGetUniformLocation(defaultShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		int viewLoc = glGetUniformLocation(defaultShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		
		int projectionLoc = glGetUniformLocation(defaultShader.ID, "projection");
		defaultShader.setMat4("projection", projection);


		glDrawArrays(GL_TRIANGLES, 0, 36);
			

		//Swap the buffers and check/call events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//############# END OF GAME LOOP ###############



	//End of Main

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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
			glfwSetWindowTitle(window, "Breeze One - Wireframe mode");
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glfwSetWindowTitle(window, "Breeze One");			
		}
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.01f;
		if (mixValue >= 1.0f)
		{
			mixValue = 1.0f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.01f;
		if (mixValue <= 0.0f)
		{
			mixValue = 0.0f;
		}
	}


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		mainCam.keyboardInput(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		mainCam.keyboardInput(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		mainCam.keyboardInput(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		mainCam.keyboardInput(RIGHT, deltaTime);

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
		toggleWireframeMode(window);
	if (key == GLFW_KEY_F8 && action == GLFW_PRESS)
		mainCam.cameraFOV = 90;
	
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse == true)
	{
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}

	float mouseXoffset = xpos - lastMouseX;
	float mouseYoffset = lastMouseY - ypos; // Y-Offset is inverted as the Y-axis goes from bottom to top
	lastMouseX = xpos;
	lastMouseY = ypos;

	mainCam.mouseInput(mouseXoffset, mouseYoffset);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mainCam.scrollInput(yoffset);
}

void unZip(unsigned char* &contents, int &stSize, std::string zipFile, std::string file)
{
	int zipError = 0;
	zip* z = zip_open(zipFile.c_str(), 0, &zipError);

	const char* name = file.c_str();
	struct zip_stat st;
	zip_stat_init(&st);
	zip_stat(z, name, 0, &st);

	contents = new unsigned char[st.size];

	zip_file* f = zip_fopen(z, name, 0);
	if (f != NULL)
	{
		zip_fread(f, contents, st.size);
		zip_fclose(f);
		zip_close(z);
	}


	stSize = st.size;

}
