#version 330 core
layout (location = 0) in vec2 aPos;			// the position variable has attribute position 0
layout (location = 1) in vec2 aTexCoord;	// the texel variable has attribute position 2

out vec2 TexCoord;	// output a texel to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 0.0f, 1.0f);
	TexCoord = aTexCoord;	// set TexCoord to the input texel we got from the vertex data
}