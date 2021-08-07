#pragma once

#include <iostream>
#include <filesystem>

//OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Engine Stuff
const std::string APP_DATA_PATH = std::filesystem::temp_directory_path().parent_path().parent_path().parent_path().string() += "\\Roaming\\BreezeEngine\\";
const std::string ENGINE_DEFAULTS_PATH = APP_DATA_PATH + "engineDefaults.bpf";
const std::string CURRENT_PATH = std::filesystem::current_path().string() += "\\";

const glm::dvec3 COL_BREEZE_DARK (46.0f/255.0f, 29.0f/255.0f, 34.0f/255.0f);
const glm::dvec3 COL_BREEZE_SKY_GREEN (111.0f/255.0f, 242.0f/255.0f, 149.0f/255.0f);
const glm::dvec3 COL_BREEZE_SKY (167.0f/255.0f, 207.0f/255.0f, 223.0f/255.0f);

const glm::dvec3 COL_X_AXIS (151.0f/255.0f, 59.0f/255.0f, 59.0f/255.0f);
const glm::dvec3 COL_Y_AXIS (83.0f/255.0f, 219.0f/255.0f, 137.0f/255.0f);
const glm::dvec3 COL_Z_AXIS (123.0f/255.0f, 171.0f/255.0f, 186.0f/255.0f);
