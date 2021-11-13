#include <glad/glad.h>
#include <SDL/SDL.h>

#include "stb_image.h"
#include <iostream>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <ostream>
#include <sstream>
#include <map>
#include <chrono>

//OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Breeze Engine Includes
#include "engineSettings.h"
#include "zipManager.h"
#include "textRenderer.h"
#include "line.h"
#include "consts.h"
#include "shader.h"
#include "camera.h"
#include "breeze_timer.h"
#include "breeze_utilities.h"


//Prototypes

void toggleWireframeMode();

void processFrameInput(float deltaTime);

void processInput();

void key_callback(int key, int scancode, int action, int mods);

void mouse_callback(double xpos, double ypos);

void scroll_callback(double xoffset, double yoffset);

void checkArgs(int range, char* args[]); //Probably really unoptimized but shut up pls ( It only happens once at startup so it's fine >:c )

void setBuildNumber();

bool checkEngineResources();

bool SDL_IntializeAndCreateWindow();

void getFPS();

void render();

double hires_time_in_seconds();

bool hasCheckedArgs = false;
bool mainWindowRun = true;


time_t now = time(0);
tm ltm;
errno_t ffs = localtime_s(&ltm, &now);

std::string buildNumber = "";
std::string windowName = "Breeze Engine";

// Frames Per Second management
Breeze_Timer lockClock;
Breeze_Timer fpsCounterClock;
double startTime 		= 0.0f;
double currentTime		= 0.0f;
double frameTime		= 0.0f;
double deltaTime		= 0.0f;

//FPS Counter stuff
double startFPSTime		= 0.0f;
double currentFPSTime	= 0.0f;
double frameCounter 	= 0.0f;
double framesItTook		= 0.0f;
double msPerFrame		= 0.0f;


// Camera setup
Camera mainCam(glm::vec3(0.0f, 0.0f, 3.0f), 90.0f);

float lastMouseX = SCREEN_WIDTH / 2;
float lastMouseY = SCREEN_HEIGHT / 2;
bool firstMouse = true;
float mixValue = 0.0f;

glm::vec3 lightPos(-2.82f, 3.27f, -8.12f);

zipManager zipper;
textRenderer txtRndr; 

Coordinate_System CoordSys = Coordinate_System::BREEZE_ENGINE;

SDL_Window* window = nullptr;

int main(int argc, char *argv[])
{
	//Check for memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//Check Command Line Arguments
	if (argc >= 2 && hasCheckedArgs == false)
	{
		for (int i = 0; i < argc; i++)
			checkArgs(i, argv);
	}

	//Engine Startup
	setBuildNumber();
	if (!checkEngineResources()) //If one or more files in AppData/Roaming/BreezeEngine are not found then exit the engine
		return 69;
		
	if(!SDL_IntializeAndCreateWindow())
		return -1;


	//Shader defaultShader((CURRENT_PATH + "shaders/default_vertex.glsl").c_str(), (CURRENT_PATH + "shaders/default_fragment.glsl").c_str());
	Shader breathingShader((CURRENT_PATH + "shaders/default_vertex.glsl").c_str(), (CURRENT_PATH + "shaders/breathing_fragment.glsl").c_str());
	Shader defaultShader((CURRENT_PATH + "shaders/default_vertex.glsl").c_str(), (CURRENT_PATH + "shaders/default_fragment.glsl").c_str());
	Shader pointLightShader((CURRENT_PATH + "shaders/pointLight_vertex.glsl").c_str(), (CURRENT_PATH + "shaders/pointLight_fragment.glsl").c_str());
	Shader textShader((CURRENT_PATH + "shaders/text_vertex.glsl").c_str(), (CURRENT_PATH + "shaders/text_fragment.glsl").c_str());
	txtRndr.init(14);

	unsigned char* unzippedLogo = 0;
	int unzippedLogoSize = 0;
	int logoChannels = 0;
	zipper.unZip(unzippedLogo, unzippedLogoSize, ENGINE_DEFAULTS_PATH, "BreezeLogo.png");


	float cube[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] =
	{
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	// ######## Cube Setup ##########
	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	//Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Normal Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
	//Texture Attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

	// ######### pointLight Setup #########
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	//Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
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

	zipper.unZip(unzippedTexture, unzippedTextureSize, APP_DATA_PATH + "engineTextures.bpf", "TestSpec2.png");
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
	textShader.use();
	glm::mat4 textProjection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT));
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(textProjection));

	Line xLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), COL_X_AXIS);
	Line yLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f), COL_Y_AXIS);
	Line zLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.5f), COL_Z_AXIS);

	Line xLineBreeze(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), COL_X_AXIS);
	Line yLineBreeze(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -0.5f), COL_Y_AXIS);
	Line zLineBreeze(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f), COL_Z_AXIS);

	SDL_Event* e = nullptr;

	fpsCounterClock.start(); //Start a timer so FPS can be counted
	lockClock.start();

	//###############################################################################################################################################################
	//######################################################################### GAME LOOP ###########################################################################
	//###############################################################################################################################################################

	startTime = SDL_GetTicks();
	startFPSTime = SDL_GetTicks();

	defaultShader.use();
	defaultShader.setInt("material.diffuse", 0);
	defaultShader.setInt("material.specular", 1);

	while (mainWindowRun)
	{
		currentTime = SDL_GetTicks();
		frameTime = currentTime - startTime;
		msPerFrame = frameTime;
		startTime = currentTime;
		getFPS();
		//################################################# EVERYTHING THAT NEEDS TO BE TIED TO FRAMERATE ########################################################
		while (frameTime > 0.0f)
		{
			deltaTime = glm::min(frameTime, FRAME_TARGET);
			frameTime -= deltaTime;
			processFrameInput(deltaTime);
		}
		//################################################# END OF EVERYTHING THAT NEEDS TO BE TIED TO FRAMERATE #################################################
		
		processInput();
		

		if (WIREFRAME_MODE == true)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//Rendering
		glEnable(GL_DEPTH_TEST);
		glClearColor(COL_BREEZE_SKY.x, COL_BREEZE_SKY.y, COL_BREEZE_SKY.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0); // Activate the texture unit first before binding the texture
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		defaultShader.setFloat("mixValue", mixValue);

		// Model Matrix
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

		// View Matrix
		glm::mat4 view = mainCam.getViewMatrix();

		// Projection Matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(mainCam.cameraFOV), SCREEN_WIDTH / SCREEN_HEIGHT, 0.01f, 100.0f);


		defaultShader.use();
		defaultShader.setFloat("material.shininess", 32.0f);
		defaultShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		defaultShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		defaultShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		defaultShader.setFloat("light.constant", 1.0f);
		defaultShader.setFloat("light.linear", 0.09f);
		defaultShader.setFloat("light.quadratic", 0.032f);
		defaultShader.setVec3("light.position", lightPos);
		defaultShader.setVec3("viewPos", mainCam.position);

		defaultShader.setMat4("projection", projection);
		defaultShader.setMat4("view", view);

		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			defaultShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		

		pointLightShader.use();
		pointLightShader.setMat4("projection", projection);
		pointLightShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(1.0f));
		pointLightShader.setMat4("model", model);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);




		if (DEBUG_MODE == true)
		{
			txtRndr.renderText(textShader, "Breeze Engine Build: " + buildNumber, 4, SCREEN_HEIGHT - 14, 1, COL_BREEZE_DARK);
			txtRndr.renderText(textShader, "Ms/Frame: " + to_string_with_format(msPerFrame, 2), 4, SCREEN_HEIGHT - 30, 1, COL_BREEZE_DARK);
			txtRndr.renderText(textShader, "FPS: " + to_string_with_format(framesItTook, 2), 4, SCREEN_HEIGHT - 56, 1, COL_BREEZE_DARK);
			txtRndr.renderText(textShader, "Vsync: " + boolToString(IS_V_SYNC_ENABLED), 4, SCREEN_HEIGHT - 72, 1, COL_BREEZE_DARK);

			if (CoordSys == Coordinate_System::BREEZE_ENGINE)
			{
				xLineBreeze.setMVP(projection * view); xLineBreeze.draw();
				yLineBreeze.setMVP(projection * view); yLineBreeze.draw();
				zLineBreeze.setMVP(projection * view); zLineBreeze.draw();
				txtRndr.renderText(textShader, "Camera Pos X: " + to_string_with_format(mainCam.position.x, 2), SCREEN_WIDTH - 248, SCREEN_HEIGHT - 14, 1, COL_X_AXIS);
				txtRndr.renderText(textShader, "Camera Pos Y: " + to_string_with_format((mainCam.position.z * -1), 2), SCREEN_WIDTH - 248, SCREEN_HEIGHT - 14 * 2, 1, COL_Y_AXIS);
				txtRndr.renderText(textShader, "Camera Pos Z: " + to_string_with_format(mainCam.position.y, 2), SCREEN_WIDTH - 248, SCREEN_HEIGHT - 14 * 3, 1, COL_Z_AXIS);
			}
			else if (CoordSys == Coordinate_System::OPENGL_STANDARD)
			{
				xLine.setMVP(projection * view); xLine.draw();
				yLine.setMVP(projection * view); yLine.draw();
				zLine.setMVP(projection * view); zLine.draw();
				txtRndr.renderText(textShader, "Camera Pos X: " + to_string_with_format(mainCam.position.x, 2), SCREEN_WIDTH - 248, SCREEN_HEIGHT - 14, 1, COL_X_AXIS);
				txtRndr.renderText(textShader, "Camera Pos Y: " + to_string_with_format(mainCam.position.y, 2), SCREEN_WIDTH - 248, SCREEN_HEIGHT - 14 * 2, 1, COL_Y_AXIS);
				txtRndr.renderText(textShader, "Camera Pos Z: " + to_string_with_format(mainCam.position.z, 2), SCREEN_WIDTH - 248, SCREEN_HEIGHT - 14 * 3, 1, COL_Z_AXIS);
			}

		}

		SDL_GL_SwapWindow(window);

	}
	//###############################################################################################################################################################
	//######################################################################### END OF GAME LOOP ####################################################################
	//###############################################################################################################################################################



	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);

	SDL_Quit();
	return 0;

	//End of Main
}

void toggleWireframeMode()
{
		WIREFRAME_MODE = !WIREFRAME_MODE;
		if (WIREFRAME_MODE == true)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);			
		}
}

void processFrameInput(float deltaTime)
{
    //Keys that need to be checked continuously
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if(keystate[SDL_SCANCODE_W])
    {
		mainCam.keyboardInput(FORWARD, deltaTime);
    }
    if(keystate[SDL_SCANCODE_S])
    {
		mainCam.keyboardInput(BACKWARD, deltaTime);
    }
    if(keystate[SDL_SCANCODE_A])
    {
		mainCam.keyboardInput(LEFT, deltaTime);
    }
    if(keystate[SDL_SCANCODE_D])
    {
		mainCam.keyboardInput(RIGHT, deltaTime);
    }
	
}

void processInput()
{
    //Keys that do not need to be checked continuously
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{	
		if (e.type == SDL_KEYDOWN) // KEYBOARD INPUT
		{
			switch(e.key.keysym.sym)
			{
				case SDLK_ESCAPE: std::cout << "Ass" << std::endl; mainWindowRun = false; break;
				case SDLK_f: toggleWireframeMode(); break;
				default: break;
			}
		}
		if (e.type == SDL_MOUSEMOTION) // MOUSE MOVEMENT
		{
			int xpos = e.motion.xrel;
			int ypos = e.motion.yrel;

			if (firstMouse == true)
			{
				lastMouseX = xpos;
				lastMouseY = ypos;
				firstMouse = false;
			}
			mainCam.mouseInput(xpos, (ypos * -1));
		}
	}
	
}

void key_callback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
		toggleWireframeMode();
	if (key == GLFW_KEY_F8 && action == GLFW_PRESS)
		mainCam.cameraFOV = 90;
	
}


void mouse_callback(double xpos, double ypos)
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

void scroll_callback(double xoffset, double yoffset)
{
	mainCam.scrollInput(yoffset);
}

void checkArgs(int range, char* args[])
{
	if (std::string(args[range]) == "-d" || std::string(args[range]) == "--debug")
	{
		DEBUG_MODE = true; windowName = "Breeze Engine [Debug Mode]"; return;
	}
	if (std::string(args[range]) == "-w" || std::string(args[range]) == "--wireframe")
	{
		WIREFRAME_MODE = true; return;
	}
	if (std::string(args[range]) == "-os" || std::string(args[range]) == "--opengl-standard")
	{
		CoordSys = Coordinate_System::OPENGL_STANDARD; return;
	}
	if (std::string(args[range]) == "-vs" || std::string(args[range]) == "--vsync")
	{
		IS_V_SYNC_ENABLED = SDL_TRUE; return;
	}
}

void setBuildNumber()
{
	int timeDay = ltm.tm_mday;
	int timeMonth = 1 + ltm.tm_mon;
	int timeYear = ltm.tm_year - 100;

	buildNumber.append(std::to_string(timeDay) + "-");
	buildNumber.append(std::to_string(timeMonth) + "-");
	buildNumber.append(std::to_string(timeYear));
}

bool checkEngineResources()
{
	unsigned char* unzippedShader = 0;
	int unzippedShaderSize = 0;
	bool isResourcesFound = true;

	if (!std::filesystem::exists(APP_DATA_PATH))
		std::filesystem::create_directories(APP_DATA_PATH);
	
	if (!std::filesystem::exists(ENGINE_DEFAULTS_PATH))
	{
		std::cout << "ERROR! Missing Engine Defaults!\n\nFile: " << APP_DATA_PATH <<  "engineDefaults.bpf\nThis is a required file for Breeze Engine to run\n" << std::endl;
		isResourcesFound = false;
	}
	if (!std::filesystem::exists(CURRENT_PATH + "shaders\\default_vertex.glsl"))
	{
		std::cout << "ERROR! Missing File: " << CURRENT_PATH + "shaders\\default_vertex.glsl" << "\nA new file has been generated. Please close and start Breeze Engine again\n" << std::endl;
		
		unzippedShader = 0;
		unzippedShaderSize = 0;
		zipper.unZip(unzippedShader, unzippedShaderSize, ENGINE_DEFAULTS_PATH, "default_vertex.glsl");

		std::ofstream ofs(CURRENT_PATH + "shaders\\default_vertex.glsl");
		ofs.write(reinterpret_cast<const char*>(unzippedShader), unzippedShaderSize);
		ofs.close();
		isResourcesFound = false;
	}
	if (!std::filesystem::exists(CURRENT_PATH + "shaders\\default_fragment.glsl"))
	{
		std::cout << "ERROR! Missing File: " << CURRENT_PATH + "shaders\\default_fragment.glsl" << "\nA new file has been generated. Please close and start Breeze Engine again\n" << std::endl;
		
		unzippedShader = 0;
		unzippedShaderSize = 0;
		zipper.unZip(unzippedShader, unzippedShaderSize, ENGINE_DEFAULTS_PATH, "default_fragment.glsl");

		std::ofstream ofs(CURRENT_PATH + "shaders\\default_fragment.glsl");
		ofs.write(reinterpret_cast<const char*>(unzippedShader), unzippedShaderSize);
		ofs.close();
		isResourcesFound = false;
	}
	if (!std::filesystem::exists(CURRENT_PATH + "shaders\\text_vertex.glsl"))
	{
		std::cout << "ERROR! Missing File: " << CURRENT_PATH + "shaders\\text_vertex.glsl" << "\nA new file has been generated. Please close and start Breeze Engine again\n" << std::endl;
		
		unzippedShader = 0;
		unzippedShaderSize = 0;
		zipper.unZip(unzippedShader, unzippedShaderSize, ENGINE_DEFAULTS_PATH, "text_vertex.glsl");

		std::ofstream ofs(CURRENT_PATH + "shaders\\text_vertex.glsl");
		ofs.write(reinterpret_cast<const char*>(unzippedShader), unzippedShaderSize);
		ofs.close();
		isResourcesFound = false;
	}
	if (!std::filesystem::exists(CURRENT_PATH + "shaders\\text_fragment.glsl"))
	{
		std::cout << "ERROR! Missing File: " << CURRENT_PATH + "shaders\\text_fragment.glsl" << "\nA new file has been generated. Please close and start Breeze Engine again\n" << std::endl;
		
		unzippedShader = 0;
		unzippedShaderSize = 0;
		zipper.unZip(unzippedShader, unzippedShaderSize, ENGINE_DEFAULTS_PATH, "text_fragment.glsl");

		std::ofstream ofs(CURRENT_PATH + "shaders\\text_fragment.glsl");
		ofs.write(reinterpret_cast<const char*>(unzippedShader), unzippedShaderSize);
		ofs.close();
		isResourcesFound = false;
	}

	return isResourcesFound;
}

bool SDL_IntializeAndCreateWindow()
{
	bool didNotFail = true;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CreateWindow("Breeze Engine",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	if (window == NULL)
	{
		std::cout << "Ya dun fucked up lmao. SDL2 failed to create window" << std::endl;
		SDL_Quit();
		didNotFail = false;
	}
	SDL_GL_CreateContext(window);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GL_SetSwapInterval(IS_V_SYNC_ENABLED);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "ERROR! Failed to initialize GLAD" << std::endl;
		didNotFail = false;
	}

	return didNotFail;
}






void getFPS()
{
	frameCounter++;
	currentFPSTime = SDL_GetTicks();
	if (currentFPSTime >= startFPSTime + 1000) // Checks if one second has passed
	{
		framesItTook = frameCounter;
		frameCounter = 0;
		startFPSTime = currentFPSTime;
	}
}

void render()
{

}

double hires_time_in_seconds()
{
    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::duration<float, std::milli> duration;

    static clock::time_point start = clock::now();
    duration elapsed = clock::now() - start;
    return elapsed.count() / 1000;
}