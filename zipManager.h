#pragma once
#include <zip.h>
#include <iostream>
#include <filesystem>

class zipManager
{
public:
	zipManager();
	void unZip(unsigned char*& contents, int& stSize, std::string zipFile, std::string file);
};