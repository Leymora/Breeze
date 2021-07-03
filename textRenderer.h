#pragma once

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader.h"
#include "consts.h"
#include "zipManager.h"

#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class textRenderer
{
public:

	struct Character
	{
		unsigned int	TextureID;
		glm::ivec2		Size;
		glm::ivec2	 	Bearing;
		unsigned int	Advance;
	};
	unsigned int textVAO, textVBO;
	std::map<char, Character> Characters;
	zipManager zipper;

	textRenderer();

private:
	void renderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color);
};