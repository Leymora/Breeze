#include "textRenderer.h"

textRenderer::textRenderer()
{
}

void textRenderer::renderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color)
{

	s.use();
	s.setFloat3("textColor", color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		float vertices[6][4] =
		{
			{xpos, ypos + h,		0.0f, 0.0f},
			{xpos, ypos, 			0.0f, 1.0f},
			{xpos + w, ypos, 		1.0f, 1.0f},

			{xpos, ypos + h, 		0.0f, 0.0f},
			{xpos + w, ypos, 		1.0f, 1.0f},
			{xpos + w, ypos + h,	1.0f, 0.0f}
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.Advance >> 6) * scale;

	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

FT_Face textRenderer::getFont() const
{
	return this->defaultFont;
}

void textRenderer::init(int textSize)
{

	zipper.unZip(unzippedFont, unzippedFontSize, ENGINE_DEFAULTS_PATH, "ElaineSans-Regular.ttf");

	if (FT_Init_FreeType(&ft) == true) // FT_Init_FreeType returns false if it loads correctly
	{
		std::cout << "ERROR! :FREETYPE: Failed to initialize FreeType library" << std::endl;
	}
	if (FT_New_Memory_Face(ft, unzippedFont, unzippedFontSize, 0, &defaultFont) == true) // FT_New_Memory_Face returns false if it loads correctly
	{
		std::cout << "ERROR! :FREETYPE: Failed to load default font" << std::endl;
	}
	FT_Set_Pixel_Sizes(defaultFont, 0, textSize);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(defaultFont, c, FT_LOAD_RENDER) == true) // FT_Load_Char returns false if it loads correctly
		{
			std::cout << "ERROR! :FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}

		glGenTextures(1, &fontTexture);
		glBindTexture(GL_TEXTURE_2D, fontTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, defaultFont->glyph->bitmap.width, defaultFont->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, defaultFont->glyph->bitmap.buffer);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character =
		{
			fontTexture,
			glm::ivec2(defaultFont->glyph->bitmap.width, defaultFont->glyph->bitmap.rows),
			glm::ivec2(defaultFont->glyph->bitmap_left, defaultFont->glyph->bitmap_top),
			static_cast<unsigned int>(defaultFont->glyph->advance.x)
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(defaultFont);
	FT_Done_FreeType(ft);


	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
