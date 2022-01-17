#include "breeze_utilities.h"



std::string boolToString(bool theBool)
{
	std::string toReturn = "false";

	if (theBool == true)
		toReturn = "true";

	return toReturn;
}
std::string to_string_with_format(float variable, int nrOfDecimals)
{
	std::ostringstream out;
    out.precision(nrOfDecimals);
    out << std::fixed << variable;
    return out.str();
}

// For Random Number Generation ----------------------------------------------------------------------------------
long long BREEZE_m = std::powf(2, 32);	// Modulus
int BREEZE_a = 22695477;			// Multiplier
int BREEZE_c = 1;				// Increment
long long BREEZE_RANDOM_SEED = 1;	    // Seed

long long breezeRandomInt()
{
	BREEZE_RANDOM_SEED = (BREEZE_a * BREEZE_RANDOM_SEED + BREEZE_c) % BREEZE_m;
	return BREEZE_RANDOM_SEED;
}
float breezeRandomFloat()
{
	return breezeRandomInt() / static_cast<float>(BREEZE_m);
}
float breezeRandomFloatRange(int min, int max)
{
	return min + breezeRandomFloat() * ((max+1) - min);
}
long long breezeRandomIntRange(int min, int max)
{
	return floor(breezeRandomFloatRange(min, max));
}
//----------------------------------------------------------------------------------------------------------------
