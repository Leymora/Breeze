#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "consts.h"
#include "stb_image.h"
#include "shader.h"
#include "camera.h"
#include "zipManager.h"

#include <iostream>
#include <filesystem>
#include <map>

//OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// FreeType Text Rendering
#include <ft2build.h>
#include FT_FREETYPE_H

//Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void toggleWireframeMode(GLFWwindow* window);

void processInput(GLFWwindow* window);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void renderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color);

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

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

zipManager zipper;


int main()
{

	if(!std::filesystem::exists(APP_DATA_PATH))
		std::filesystem::create_directories(APP_DATA_PATH);
	
	if(!std::filesystem::exists(ENGINE_DEFAULTS_PATH))
	{
		std::cout << "ERROR! Missing Engine Defaults!\n\nFile: " << APP_DATA_PATH <<  "engineDefaults.bpf\nThis is a required file for Breeze Engine to run\n" << std::endl;
		std::cout << CURRENT_PATH << std::endl;
		return 69;
	}


	//Font Stuff



	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breeze One", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "ERROR! GLFW window creation failed" << std::endl;
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
		std::cout << "ERROR! Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader defaultShader((CURRENT_PATH + "shaders/default_vertex.glsl").c_str(), (CURRENT_PATH + "shaders/default_fragment.glsl").c_str());
	Shader breathingShader((CURRENT_PATH + "shaders/default_vertex.glsl").c_str(), (CURRENT_PATH + "shaders/breathing_fragment.glsl").c_str());
	Shader lightShader((CURRENT_PATH + "shaders/light_vertex.glsl").c_str(), (CURRENT_PATH + "shaders/light_fragment.glsl").c_str());
	Shader lightCubeShader((CURRENT_PATH + "shaders/lightCube_vertex.glsl").c_str(), (CURRENT_PATH + "shaders/lightCube_fragment.glsl").c_str());
	Shader textShader((CURRENT_PATH + "shaders/text_vertex.glsl").c_str(), (CURRENT_PATH + "shaders/text_fragment.glsl").c_str());

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

	glm::vec3 cubePositions[] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -4.0f)
	};

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	// ######## Cube Setup ##########
	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	//Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// ######### Light Cube Setup #########

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	//Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// All Texture setup
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* unzippedTexture = 0;
	int unzippedTextureSize = 0;
	unsigned char* textureData = 0;


	//Texture 1 ---------------------------------------------
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	zipper.unZip(unzippedTexture, unzippedTextureSize, ENGINE_DEFAULTS_PATH, "default_texture.png");

	textureData = stbi_load_from_memory(unzippedTexture, unzippedTextureSize, &width, &height, &nrChannels, 0);
	if(!textureData)
	{
		zipper.unZip(unzippedTexture, unzippedTextureSize, ENGINE_DEFAULTS_PATH, "fallback_texture.png");
		textureData = stbi_load_from_memory(unzippedTexture, unzippedTextureSize, &width, &height, &nrChannels, 0);
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


	zipper.unZip(unzippedTexture, unzippedTextureSize, APP_DATA_PATH + "engineTextures.bpf", "brick.jpg");

	textureData = stbi_load_from_memory(unzippedTexture, unzippedTextureSize, &width, &height, &nrChannels, 0);

	if(!textureData)
	{
		zipper.unZip(unzippedTexture, unzippedTextureSize, ENGINE_DEFAULTS_PATH, "fallback_texture.png");
		textureData = stbi_load_from_memory(unzippedTexture, unzippedTextureSize, &width, &height, &nrChannels, 0);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(textureData);


	//-------------------------------------------------------




	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(defaultFont, c, FT_LOAD_RENDER) == true) // FT_Load_Char returns false if it loads correctly
		{
			std::cout << "ERROR! :FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}

		unsigned int fontTexture;
		glGenTextures(1, &fontTexture);
		glBindTexture(GL_TEXTURE_2D, fontTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, defaultFont->glyph->bitmap.width, defaultFont->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, defaultFont->glyph->bitmap.buffer);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character =
		{
			fontTexture,
			glm::ivec2(defaultFont->glyph->bitmap.width, defaultFont->glyph->bitmap.rows),
			glm::ivec2(defaultFont->glyph->bitmap_left, defaultFont->glyph->bitmap_top),
			static_cast<unsigned int>(defaultFont->glyph->advance.x)
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(defaultFont);
	FT_Done_FreeType(ft);


	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	defaultShader.use();
	glUniform1i(glGetUniformLocation(defaultShader.ID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(defaultShader.ID, "texture2"), 1);

	textShader.use();
	glm::mat4 textProjection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT));
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(textProjection));


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



		
		//Matrices ---------------------------------------

		// Model Matrix
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

		// View Matrix
		glm::mat4 view = mainCam.getViewMatrix();

		// Projection Matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(mainCam.cameraFOV), SCREEN_WIDTH / SCREEN_HEIGHT, 0.01f, 100.0f);


		lightShader.use();
		lightShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[0]);
		model = glm::rotate(model, glm::radians((float)glfwGetTime() * 69), glm::vec3(1.0f, 0.3f, 0.5f));
		lightShader.setMat4("model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightCubeShader.setMat4("model", model);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		renderText(textShader, "I have AIDS", 25, 25, 1, glm::vec3(0.0f, 0.0470588235f, 0.0509803922f));

		//Swap the buffers and check/call events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//############# END OF GAME LOOP ###############



	//End of Main

	glDeleteVertexArrays(1, &cubeVAO);
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


void renderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color)
{
	s.use();
	s.setFloat3("textColor", color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		float vertices[6][4] =
		{
			{xpos, ypos + h,		0.0f, 0.0f},
			{xpos, ypos, 			0.0f, 1.0f},
			{xpos + w, ypos, 		1.0f, 1.0f},

			{xpos, ypos + h, 		0.0f, 0.0f},
			{xpos + w, ypos, 		1.0f, 1.0f},
			{xpos + w, ypos + h,	1.0f, 0.0f}
		};

	glBindTexture(GL_TEXTURE_2D, ch.TextureID);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	x += (ch.Advance >> 6) * scale;

	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}
