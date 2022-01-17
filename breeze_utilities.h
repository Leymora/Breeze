#pragma once
#include <string>
#include <sstream>

std::string boolToString(bool theBool);
std::string to_string_with_format(float variable, int nrOfDecimals);
long long breezeRandomInt();
float breezeRandomFloat();
float breezeRandomFloatRange(int min, int max);
long long breezeRandomIntRange(int min, int max);