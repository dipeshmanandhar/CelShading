//Dipesh Manandhar 5/15/2019

//Created H Files
#include "../../Headers/Renderer/TextRenderer.h"

//public:
	
/* Constructors */

Renderer::TextRenderer::TextRenderer()
{
	//do nothing
	VAO = VBO = 0;
	bitmapFont = 0;
}

Renderer::TextRenderer::TextRenderer(const char* bitmapFontFile) : TextRenderer()
{
	initialize(bitmapFontFile);
}

Renderer::TextRenderer::~TextRenderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &bitmapFont);
}

/*  Functions  */

void Renderer::TextRenderer::initialize(const char* bitmapFontFile)
{
	stbi_set_flip_vertically_on_load(true);
	bitmapFont = Loader::TextureFromFile(bitmapFontFile, "Resources/Fonts");
	stbi_set_flip_vertically_on_load(false);

	glGenBuffers(1, &VBO);	// Objects in C are structs, thus in OpenGL, int ids "point to" object memory
	glGenVertexArrays(1, &VAO);

	shader.initialize("Resources/Shaders/TextShader.vert", "Resources/Shaders/TextShader.frag");
}

// (xPos, yPos) is the NDC top left corner of the text to be rendered (default value = top left corner of NDC screen)
// width and height are the respective dimension sizes to use per character
void Renderer::TextRenderer::Draw(const char* text, float xpos, float ypos, float width, float height) const
{
	//const unsigned int numVertices = text.length() * 24; // 2 triangles per character, 3 points per triangle,  4 floats (2 for position, 2 for texel) per point
	vector<float> vertices;
	//cout << text << endl;

	//create vertex data for each quad, in counterclockwise winding order
	for (unsigned int i = 0; text[i] != '\0'; i++)
	{
		//bottom-left triangle
		//top-left vertex
		vertices.push_back(xpos + width*i);	//position data
		vertices.push_back(ypos);
		vertices.push_back((text[i] % horizontalChars) * texelCellWidth);	//texel data
		vertices.push_back(1 - (text[i] / horizontalChars) * texelCellHeight); //the 1 - () is because the y axis of texels is flipped from the rows going down
		//bottom-left vertex
		vertices.push_back(vertices[i * 24 + 0]);	//position data
		vertices.push_back(vertices[i * 24 + 1] - height);
		vertices.push_back(vertices[i * 24 + 2]);	//texel data
		vertices.push_back(vertices[i * 24 + 3] - texelCellHeight); //remember, the y axis of texels is flipped
		//bottom-right vertex
		vertices.push_back(vertices[i * 24 + 0] + width);	//position data
		vertices.push_back(vertices[i * 24 + 5]);
		vertices.push_back(vertices[i * 24 + 2] + texelCellWidth);	//texel data
		vertices.push_back(vertices[i * 24 + 7]);
			
		//top-right triangle
		//bottom-right vertex
		vertices.push_back(vertices[i * 24 + 8]);	//position data
		vertices.push_back(vertices[i * 24 + 9]);
		vertices.push_back(vertices[i * 24 + 10]);	//texel data
		vertices.push_back(vertices[i * 24 + 11]);
		//top-right vertex
		vertices.push_back(vertices[i * 24 + 8]);	//position data
		vertices.push_back(vertices[i * 24 + 1]);
		vertices.push_back(vertices[i * 24 + 10]);	//texel data
		vertices.push_back(vertices[i * 24 + 3]);
		//top-left vertex
		vertices.push_back(vertices[i * 24 + 0]);	//position data
		vertices.push_back(vertices[i * 24 + 1]);
		vertices.push_back(vertices[i * 24 + 2]);	//texel data
		vertices.push_back(vertices[i * 24 + 3]);
	}

	//for (int i=0;i<vertices.size();i+=5)
	//	cout << vertices[i] << " " << vertices[i+1] << " " << vertices[i+2] << endl << vertices[i+3] << " " << vertices[i+4] << endl << endl;
	//cout << "---------------------------------------------------------------------" << endl;

	//bind vertex data to VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy vertex array data into VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &(vertices[0]), GL_STATIC_DRAW);
	// set the vertex attributes (only position and texel data)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// set the texture uniform in the Shader
	shader.use();
	glActiveTexture(GL_TEXTURE0);
	shader.setInt("bitmapFont", 0);
	glBindTexture(GL_TEXTURE_2D, bitmapFont);

	// draw mesh
	//glBindVertexArray(VAO); //already binded above
	glDrawArrays(GL_TRIANGLES, 0, vertices.size()/4);
	glBindVertexArray(0);

	//delete[] vertices;
}


