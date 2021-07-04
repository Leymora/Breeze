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
private:

	zipManager zipper;

	struct Character
	{
		unsigned int	TextureID;
		glm::ivec2		Size;
		glm::ivec2	 	Bearing;
		unsigned int	Advance;
	};

	unsigned int textVAO, textVBO = 0;
	std::map<char, Character> Characters;

	unsigned char* unzippedFont = 0;
	int unzippedFontSize = 0;
	unsigned int fontTexture = 0;
	FT_Library ft;
	FT_Face defaultFont = FT_Face();

public:

	textRenderer();
	void renderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color);
	FT_Face getFont() const;
	void init(int textSize = 18);
};