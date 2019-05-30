//Dipesh Manandhar 5/15/2019

#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

//External Libraries for OpenGL (GLFW and GLAD)
#include <glad/glad.h> // NOTE: always include GLAD before GLFW
#include <GLFW/glfw3.h>

//C++ Libraries
#include <vector>
#include <string>

//Created H Files
#include "Shader.h"
#include "Loader.h"
#include "Screen.h"

using namespace std;

namespace Renderer
{
	class TextRenderer {
	public:
		/*  TextRenderer Data  */

		/* Constructors */

		/*
		TextRenderer();

		TextRenderer(const char* bitmapFontFile);

		~TextRenderer();
		*/

		/*  Functions  */

		static void initialize(const char* bitmapFontFile);

		static void clean();

		// (xPos, yPos) is the NDC top left corner of the text to be rendered (default value = top left corner of NDC screen)
		// width and height are the respective dimension sizes to use per character
		static void Draw(const char* text, float xpos = -1.0f, float ypos = 1.0f, float width = 50.0f / SCREEN_WIDTH, float height = 50.0f / SCREEN_HEIGHT);

		static void setTextColor(glm::vec3 interior, glm::vec3 exterior);

	private:
		/*  Render data  */

		static unsigned int bitmapFont, VAO, VBO;
		static Shader shader;
		static const unsigned int horizontalChars;
		static const unsigned int verticalChars;
		static const float texelCellWidth;
		static const float texelCellHeight;

		/*  Functions    */

	};
}

#endif // !TEXTRENDERER_H

