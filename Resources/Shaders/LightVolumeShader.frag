#version 330 core
out vec4 FragColor;
  
//in vec2 TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;

struct DirLight {
    vec3 Direction;	// in view coordinates
    vec3 Color;
};

struct PointLight {
    vec3 Position;	//in view coordinates
    vec3 Color;
	
    float Linear;
    float Quadratic;
};

uniform DirLight dirLight;
#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform int lightID; //-1 is directionalLight, >= 0 is a point light at same index

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SOBEL_OFFSET 1
#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

float flatten(float value, int numColors)
{
	float levels = numColors - 1;
	return round(value*levels)/levels;
}
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 Albedo, float Specular)
{
    // diffuse
    vec3 lightDir = normalize(-light.Direction);
    vec3 diffuse = flatten(max(dot(normal, lightDir), 0.0f), 3) * Albedo * light.Color;
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    vec3 specular = light.Color * flatten(spec * Specular, 2);

    return diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Albedo, float Specular)
{
    // attenuation
    float dist = length(light.Position - fragPos);
    float attenuation = 1.0 / (1.0 + light.Linear * dist + light.Quadratic * dist * dist);
	
    // diffuse
    vec3 lightDir = normalize(light.Position - fragPos);
    vec3 diffuse = flatten(max(dot(normal, lightDir), 0.0f) * attenuation, 3) * Albedo * light.Color;
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    vec3 specular = light.Color * flatten(spec * Specular * attenuation, 2);

    return diffuse + specular;
}

//bool isEdge()
//{
//	vec3 gx = texture(gNormal, vec2((gl_FragCoord.x + SOBEL_OFFSET) / SCREEN_WIDTH, (gl_FragCoord.y + SOBEL_OFFSET) / SCREEN_HEIGHT)).rgb +
//			  2 * texture(gNormal, vec2((gl_FragCoord.x + SOBEL_OFFSET) / SCREEN_WIDTH, (gl_FragCoord.y) / SCREEN_HEIGHT)).rgb +
//			  texture(gNormal, vec2((gl_FragCoord.x + SOBEL_OFFSET) / SCREEN_WIDTH, (gl_FragCoord.y - SOBEL_OFFSET) / SCREEN_HEIGHT)).rgb -
//			  ( texture(gNormal, vec2((gl_FragCoord.x - SOBEL_OFFSET) / SCREEN_WIDTH, (gl_FragCoord.y + SOBEL_OFFSET) / SCREEN_HEIGHT)).rgb +
//			  2 * texture(gNormal, vec2((gl_FragCoord.x - SOBEL_OFFSET) / SCREEN_WIDTH, (gl_FragCoord.y) / SCREEN_HEIGHT)).rgb +
//			  texture(gNormal, vec2((gl_FragCoord.x - SOBEL_OFFSET) / SCREEN_WIDTH, (gl_FragCoord.y - SOBEL_OFFSET) / SCREEN_HEIGHT)).rgb );
//	
//	vec3 gy = texture(gNormal, vec2((gl_FragCoord.x + SOBEL_OFFSET) / SCREEN_WIDTH, (gl_FragCoord.y + SOBEL_OFFSET) / SCREEN_HEIGHT)).rgb +
//			  2 * texture(gNormal, vec2((gl_FragCoord.x) / SCREEN_WIDTH, (gl_FragCoord.y + SOBEL_OFFSET) / SCREEN_HEIGHT)).rgb +
//			  texture(gNormal, vec2((gl_FragCoord.x - SOBEL_OFFSET) / SCREEN_WIDTH, (gl_FragCoord.y + SOBEL_OFFSET) / SCREEN_HEIGHT)).rgb -
//			  ( texture(gNormal, vec2((gl_FragCoord.x + SOBEL_OFFSET) / SCREEN_WIDTH, (gl_FragCoord.y - SOBEL_OFFSET) / SCREEN_HEIGHT)).rgb +
//			  2 * texture(gNormal, vec2((gl_FragCoord.x) / SCREEN_WIDTH, (gl_FragCoord.y - SOBEL_OFFSET) / SCREEN_HEIGHT)).rgb +
//			  texture(gNormal, vec2((gl_FragCoord.x - SOBEL_OFFSET) / SCREEN_WIDTH, (gl_FragCoord.y - SOBEL_OFFSET) / SCREEN_HEIGHT)).rgb );
//
//	//vec3 gradient = sqrt(gx * gx + gy * gy);
//	vec3 gradient = abs(gx) + abs(gy); // max value per component is 8, min is 0
//	float threshold = 4; // max per component * 50%
//	
//	return max(max(gradient.r, gradient.g), gradient.b) > threshold;
//}

void main()
{
	vec2 TexCoord = vec2(gl_FragCoord.x / SCREEN_WIDTH, gl_FragCoord.y / SCREEN_HEIGHT);
    // retrieve data from G-buffer
    vec3 Normal = texture(gNormal, TexCoord).rgb;
	if (Normal == vec3(-1.0f))
		discard;
	vec3 FragPos = texture(gPosition, TexCoord).rgb;
	vec4 colorSpec = texture(gColorSpec, TexCoord);
	if(colorSpec == vec4(0.0f))
	{
		FragColor = vec4(vec3(0.0f), 1.0f);
		return;
	}
    vec3 Albedo = colorSpec.rgb;
    float Specular = colorSpec.a;
	vec3 viewDir = normalize(-FragPos);

	vec3 result;
	
	//float z = -FragPos.z / FAR_PLANE;

	//gl_FragDepth = (1/-FragPos.z - 1/NEAR_PLANE) / (1/FAR_PLANE - 1/NEAR_PLANE);
	//gl_FragDepth = z;

	//TODO: move the if part to a separate shader, as this if conditional is poorly optimized by GLSL
	if(lightID < 0) //directional Light
	{
		//if(isEdge())
		//{
		//	FragColor = vec4(0.0f);
		//	for(int i = 0 ; i < 4 ; i++)
		//	{
		//		float z = texture(gPosition, vec2((gl_FragCoord.x + SOBEL_OFFSET * ((i - 2)%2) ) / SCREEN_WIDTH, (gl_FragCoord.y + SOBEL_OFFSET * ((i - 1)%2) ) / SCREEN_HEIGHT)).z;
		//		if(z != -1.0f)
		//		{
		//			gl_FragDepth = (1/-z - 1/NEAR_PLANE) / (1/FAR_PLANE - 1/NEAR_PLANE);
		//			return;
		//		}
		//	}
		//	for(int i = 0 ; i < 4 ; i++)
		//	{
		//		float z = texture(gPosition, vec2((gl_FragCoord.x + SOBEL_OFFSET * ((i - 2)%2) ) / SCREEN_WIDTH, (gl_FragCoord.y + SOBEL_OFFSET * ((i - 2)%2) ) / SCREEN_HEIGHT)).z;
		//		if(z != -1.0f)
		//		{
		//			gl_FragDepth = (1/-z - 1/NEAR_PLANE) / (1/FAR_PLANE - 1/NEAR_PLANE);
		//			return;
		//		}
		//	}
		//	return;
		//}
		//else
		//{
			//if (Normal == vec3(-1.0f))
			//	discard;
			// ambient
			result = Albedo * 0.1f; // hard-coded ambient component
		
			result += CalcDirLight(dirLight, Normal, viewDir, Albedo, Specular);
		//}
	}
	else
	{
		//if (Normal == vec3(-1.0f))
		//	discard;
		result = CalcPointLight(pointLights[lightID], Normal, FragPos, viewDir, Albedo, Specular);
	}
	
	FragColor = vec4(result / (result + vec3(1.0f)), 1.0f);
	//FragColor = vec4(vec3(gl_FragDepth), 1.0f);
}