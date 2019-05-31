//Dipesh Manandhar 5/29/2019

//External Libraries for OpenGL (GLFW and GLAD)
#include <glad/glad.h> // NOTE: always include GLAD before GLFW
#include <GLFW/glfw3.h>

//External Library for Matrix Math (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//External Library For loading images (STB)
#include "../../Headers/Renderer/stb_image.h"

// C++ Libraries
#include <vector>
#include <string>

//Created H Files
#include "../../Headers/Renderer/Terrain.h"
#include "../../Headers/Renderer/Shader.h"
#include "../../Headers/Renderer/Mesh.h"
#include "../../Headers/Renderer/Loader.h"
#include "../../Headers/Renderer/VertexStructs.h"

using namespace std;

//public:
/*  Constructors  */

Renderer::Terrain::Terrain(float terScale, float terHeight, float terImageTiles) :
	TERRAIN_SCALE(terScale), TERRAIN_HEIGHT(terHeight), TERRAIN_IMAGE_TILES(terImageTiles)
{
	VAO = VBO = EBO = 0;
}

Renderer::Terrain::Terrain(const string& heightmapFile, const vector<string>& textureFiles, float terScale, float terHeight, float terImageTiles) : 
	TERRAIN_SCALE(terScale), TERRAIN_HEIGHT(terHeight), TERRAIN_IMAGE_TILES(terImageTiles)
{
	// set up VAO, VBO and EBO
	setupVertexData(heightmapFile);
	setupTextures(textureFiles);
}

/*  Functions  */

void Renderer::Terrain::initialize(Shader& s, glm::mat4& v, glm::mat4& p)
{
	view = &v;
	projection = &p;
	shader = &s;
}

void Renderer::Terrain::Draw() const
{
	shader->use();

	// the model matrix is the identity matrix (i.e., the terrain cannot be rotated, translated, or scaled on the fly)
	shader->setMat4("mv", *view);
	shader->setMat4("mvp", *projection * *view);
	shader->setMat3("mvNormal", transpose(inverse(glm::mat3(*view))));

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		//TODO: add ambient texture?

		shader->setInt((name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// draw vertices
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

float Renderer::Terrain::getHeightAt(const glm::vec3& position) const
{
	unsigned int x = (unsigned int) roundf(position.x * (float)width / TERRAIN_SCALE - 0.5f);
	unsigned int z = (unsigned int)roundf(position.z * (float)height / TERRAIN_SCALE - 0.5f);

	return vertices[z * width + x].Position.y;
}

void Renderer::Terrain::setupVertexData(const string& heightmapFile)
{
	int w, h, nrComponents;
	unsigned char* data = stbi_load(heightmapFile.c_str(), &w, &h, &nrComponents, 1); //load in grayscale

	width = (unsigned int)w;
	height = (unsigned int)h;

	if (data)
	{
		// set up vertices data
		for (unsigned int z = 0; z < (unsigned int)height; z++)
		{
			for (unsigned int x = 0; x < (unsigned int)width; x++)
			{
				Vertex v;

				glm::vec3 point = glm::vec3( ((float)x + 0.5f) / (float)width, (float)data[z * width + x] / 255.0f, ((float)z + 0.5f) / (float)height);

				// position data
				v.Position = glm::vec3(TERRAIN_SCALE * point.x, TERRAIN_HEIGHT * point.y, TERRAIN_SCALE * point.z);

				// normal data
				v.Normal = glm::vec3(0.0f);
				
				//approximation - no longer used
				/*
				// read neighbor heights using an arbitrary small offset
				float hL = (float)data[z * width + x - 1] / 255.0f;
				float hR = (float)data[z * width + x + 1] / 255.0f;
				float hD = (float)data[(z + 1) * width + x] / 255.0f;
				float hU = (float)data[(z - 1) * width + x] / 255.0f;
				// deduce terrain normal
				v.Normal.x = hL - hR;
				v.Normal.y = 0.02f;
				v.Normal.z = hD - hU;
				v.Normal = glm::normalize(v.Normal);

				//cout << "(" << v.Normal.x << ", " << v.Normal.y << ", " << v.Normal.z << ")" << endl; //This line is AIDS
				*/

				// texture data
				v.TexCoords = TERRAIN_IMAGE_TILES * glm::vec2(point.x, point.z);

				vertices.push_back(v);
			}
		}

		const glm::vec3 ERROR_HEIGHT = glm::vec3(-1.0f);
		//claculate smooth normals for each vertex
		for (unsigned int i = 0; i < vertices.size(); i++)
		{
			// first sample 5 points
			glm::vec3 mid = vertices[i].Position;
			glm::vec3 left = (i % width == 0) ? ERROR_HEIGHT : vertices[i - 1].Position;
			glm::vec3 right = ((i+1) % width == 0) ? ERROR_HEIGHT : vertices[i + 1].Position;
			glm::vec3 down = (i / width == height - 1) ? ERROR_HEIGHT : vertices[i + width].Position; // as in positive z, one row down (plus)
			glm::vec3 up = (i / width == 0) ? ERROR_HEIGHT : vertices[i - width].Position; // as in negative z, one row up(minus)

			glm::vec3 toUp = up - mid;
			glm::vec3 toLeft = left - mid;
			glm::vec3 toDown = down - mid;
			glm::vec3 toRight = right - mid;

			if (left != ERROR_HEIGHT && up != ERROR_HEIGHT)
				vertices[i].Normal += glm::normalize(glm::cross(toUp, toLeft));
			if (left != ERROR_HEIGHT && down != ERROR_HEIGHT)
				vertices[i].Normal += glm::normalize(glm::cross(toLeft, toDown));
			if (right != ERROR_HEIGHT && down != ERROR_HEIGHT)
				vertices[i].Normal += glm::normalize(glm::cross(toDown, toRight));
			if (right != ERROR_HEIGHT && up != ERROR_HEIGHT)
				vertices[i].Normal += glm::normalize(glm::cross(toRight, toDown));

			vertices[i].Normal = glm::normalize(vertices[i].Normal);
		}

		// set up indices data (in counter clockwise winding order for front faces)

		for (unsigned int r = 0; r < (unsigned int)height - 1; r++)
		{
			for (unsigned int c = 0; c < (unsigned int)width - 1; c++)
			{
				//bottom left triangle
				indices.push_back(c + r * width);
				indices.push_back(c + (r + 1) * width);
				indices.push_back(c + (r + 1) * width + 1);

				//top right triangle
				indices.push_back(c + r * width);
				indices.push_back(c + (r + 1) * width + 1);
				indices.push_back(c + r * width + 1);
			}
		}

		// set up VAO
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
	else
		cout << "Heightmap failed to load at path: " << heightmapFile << endl;
	stbi_image_free(data);
}

void Renderer::Terrain::setupTextures(const vector<string>& textureFiles)
{
	for (string textureFile : textureFiles)
	{
		Texture t;
		t.id = Loader::TextureFromFile(textureFile);
		t.path = textureFile;
		t.type = "texture_diffuse";

		textures.push_back(t);
	}
}

//private:

glm::mat4* Renderer::Terrain::view, * Renderer::Terrain::projection;
Renderer::Shader* Renderer::Terrain::shader;
