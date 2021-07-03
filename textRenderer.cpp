#include "textRenderer.h"

textRenderer::textRenderer()
{
	unsigned char* unzippedFont = 0;
	int unzippedFontSize = 0;
	FT_Library ft;
	FT_Face defaultFont;
	zipper.unZip(unzippedFont, unzippedFontSize, ENGINE_DEFAULTS_PATH, "ElaineSans-Regular.ttf");

	if (FT_Init_FreeType(&ft) == true) // FT_Init_FreeType returns false if it loads correctly
	{
		std::cout << "ERROR! :FREETYPE: Failed to initialize FreeType library" << std::endl;
	}
	if (FT_New_Memory_Face(ft, unzippedFont, unzippedFontSize, 0, &defaultFont) == true) // FT_New_Memory_Face returns false if it loads correctly
	{
		std::cout << "ERROR! :FREETYPE: Failed to load default font" << std::endl;
	}
	FT_Set_Pixel_Sizes(defaultFont, 0, 48);
	if (FT_Load_Char(defaultFont, 'X', FT_LOAD_RENDER) == true) // FT_Load_Char returns false if it loads correctly
	{
		std::cout << "ERROR! :FREETYPE: Failed to load Glyph" << std::endl;
	}
}

void textRenderer::renderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color)
{
    
}
