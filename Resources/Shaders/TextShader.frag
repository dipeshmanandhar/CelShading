#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D bitmapFont;

void main()
{
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    FragColor = texture(bitmapFont, TexCoord);
	
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