#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform vec3 skyColor;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

uniform float viewY;
const float lowerLimit = 0.5f;
const float upperLimit = 0.75f;

float flatten(float value, int numColors)
{
	float levels = numColors - 1;
	return round(value*levels)/levels;
}
void main()
{
	//if(gl_FragCoord.x < 400)
		FragColor = texture(skybox, TexCoords);
	//else
	//	FragColor = vec4(0.0f, 1.0f, 0.5f, 1.0f);

	float brightness = (FragColor.r + FragColor.g + FragColor.b) / 3.0f;
	brightness = flatten(brightness, 10);

	FragColor = vec4(skyColor * brightness, 1.0f);
	
	float visibility = ( (gl_FragCoord.y / SCREEN_HEIGHT + viewY) - lowerLimit) / (upperLimit - lowerLimit);
	visibility = clamp(visibility, 0.0f, 1.0f);

	FragColor = mix(vec4(skyColor, 1.0f), FragColor, visibility);
}