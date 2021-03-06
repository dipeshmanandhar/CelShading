#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColorSpec;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in float visibility;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition.xyz = FragPos;
	// and the visibility (sue to fog) in first gbuffer texture's alpha component
	gPosition.w = visibility;
    
	// also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    
	// and the diffuse per-fragment color
    gColorSpec.rgb = texture(texture_diffuse1, TexCoord).rgb;
    //gColorSpec.rgb = mix(skyColor, gColorSpec.rgb, visibility);
	// store specular intensity in gAlbedoSpec's alpha component
    gColorSpec.a = texture(texture_specular1, TexCoord).r;
}