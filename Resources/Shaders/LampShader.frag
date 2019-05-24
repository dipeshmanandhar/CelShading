#version 330 core
out vec4 FragColor;

uniform vec3 LampColor;

void main()
{
	FragColor = vec4(LampColor / (LampColor + vec3(1.0f)), 1.0f);
}