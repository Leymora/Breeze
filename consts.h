#pragma once
#include <iostream>
#include <filesystem>

//Engine Stuff
const std::string APP_DATA_PATH = std::filesystem::temp_directory_path().parent_path().parent_path().parent_path().string() += "\\Roaming\\BreezeEngine\\";
const std::string ENGINE_DEFAULTS_PATH = APP_DATA_PATH + "engineDefaults.bpf";
const std::string CURRENT_PATH = std::filesystem::current_path().string() += "\\";

//User Settings
const float SCREEN_WIDTH = 1280.0f;
const float SCREEN_HEIGHT = 720.0f;