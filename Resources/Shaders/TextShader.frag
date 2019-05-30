#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D bitmapFont;
uniform vec3 TextColor = vec3(1.0f);
uniform vec3 OutlineColor = vec3(0.0f);

void main()
{
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    FragColor = mix(vec4(OutlineColor, 0.0f) , vec4(TextColor, 1.0f), texture(bitmapFont, TexCoord));
	
	// for rgba (32 bit) textures
	if(FragColor.a < 0.25)
		discard;
	
	//for rgb (24 bit) textures, where black is discarded, white is kept
	//if(FragColor.r < 0.01f) //black
	//	discard;
	//else if(FragColor.r > 0.6f) //white
	//	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//else //border color
	//	FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}