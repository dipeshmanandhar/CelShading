#version 330 core
layout (location = 0) in vec3 aPos;			// the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;		// the normal variable has attribute position 1
layout (location = 2) in vec2 aTexCoord;	// the texel variable has attribute position 2
  
out vec3 FragPos;	// output the position of the vertex in view coordinates to the fragment shader
out vec3 Normal;	// output a normal in view coordinates to the fragment shader
out vec2 TexCoord;	// output a texel to the fragment shader
out float visibility;

uniform mat4 mvp;
uniform mat4 mv;
uniform mat3 mvNormal;

const float density = 0.007f;
const float gradient = 10.0f;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = mvp * vec4(aPos, 1.0);
	FragPos = vec3(mv * vec4(aPos, 1.0));
	Normal = mvNormal * aNormal;
	TexCoord = aTexCoord;	// set TexCoord to the input texel we got from the vertex data
	visibility = clamp(exp(-pow(gl_Position.z * density, gradient)), 0.0f, 1.0f);
}