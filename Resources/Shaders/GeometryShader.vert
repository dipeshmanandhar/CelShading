#version 330 core
layout (location = 0) in vec3 aPos;			// the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;		// the normal variable has attribute position 1
layout (location = 2) in vec2 aTexCoord;	// the texel variable has attribute position 2
layout (location = 3) in uvec4 aBoneIDs;	// the bone index variable has attribute position 3
layout (location = 4) in vec4 aBoneWeights;	// the bone weight variable has attribute position 4

out vec3 FragPos;	// output the position of the vertex in view coordinates to the fragment shader
out vec3 Normal;	// output a normal in view coordinates to the fragment shader
out vec2 TexCoord;	// output a texel to the fragment shader
out float visibility;

uniform mat4 mvp;
uniform mat4 mv;
uniform mat3 mvNormal;

#define MAX_WEIGHTS 4u
#define MAX_BONES 100u
uniform mat4 bones[MAX_BONES];

const float density = 0.007f;
const float gradient = 10.0f;

void main()
{
	mat4 boneTransform = mat4(0.0f);
	//float sum = 0.0f;
	vec3 color = vec3(0.0f);
	for(uint i = 0u; i < MAX_WEIGHTS; i++)
	{
		//sum += aBoneWeights[i];
		uint temp = aBoneIDs[i] % 3u;
		if(temp == 0u)
			color += aBoneWeights[i] * vec3(aBoneIDs[i] / 100.0f, 0.0f, 0.0f);
		else if(temp == 1u)
			color += aBoneWeights[i] * vec3(0.0f, aBoneIDs[i] / 100.0f, 0.0f);
		else// if(temp == 2u)
			color += aBoneWeights[i] * vec3(0.0f, 0.0f, aBoneIDs[i] / 100.0f);

		boneTransform += bones[aBoneIDs[i]] * aBoneWeights[i];
	}
	vec4 finalPos = boneTransform * vec4(aPos, 1.0);
	//vec4 finalPos =  vec4((boneTransform * vec4(aPos, 1.0)).xyz, 1.0f);
	//vec4 finalPos =  vec4(aPos, 1.0);

    // note that we read the multiplication from right to left
    gl_Position = mvp * finalPos;
	FragPos = vec3(mv * finalPos);
	Normal = mvNormal * mat3(boneTransform) * aNormal;
	//Normal = vec3(sum);
	//Normal = color;
	TexCoord = aTexCoord;	// set TexCoord to the input texel we got from the vertex data
	visibility = clamp(exp(-pow(gl_Position.z * density, gradient)), 0.0f, 1.0f);
}