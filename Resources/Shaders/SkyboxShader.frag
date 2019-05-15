#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
	//if(gl_FragCoord.x < 400)
		FragColor = texture(skybox, TexCoords);
	//else
	//	FragColor = vec4(0.0f, 1.0f, 0.5f, 1.0f);
}