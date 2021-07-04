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

const glm::fvec3 COL_BREEZE_DARK (69.0f/255.0f, 79.0f/255.0f, 68.0f/255.0f);