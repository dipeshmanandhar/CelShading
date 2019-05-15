#version 330 core
layout (location = 0) in vec3 aPos;			// the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;		// the normal variable has attribute position 1
//layout (location = 2) in vec2 aTexCoord;	// the texel variable has attribute position 2

uniform mat4 mvp;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = mvp * vec4(aPos + aNormal*0.01, 1.0);
}