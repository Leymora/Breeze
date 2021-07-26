#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using json = nlohmann::json;

class Object
{

public:

	Object();
	bool loadModel(const char* path);

private:

	

};