#version 330 core
layout (location = 0) in vec3 aPos;			// the position variable has attribute position 0

uniform mat4 mvp;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = mvp * vec4(aPos, 1.0);
}  