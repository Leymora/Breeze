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
