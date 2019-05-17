#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

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
#define NR_POINT_LIGHTS 1  
uniform PointLight pointLights[NR_POINT_LIGHTS];

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


void main()
{             
    // retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoord).rgb;
    vec3 Normal = texture(gNormal, TexCoord).rgb;
    vec3 Albedo = texture(gColorSpec, TexCoord).rgb;
    float Specular = texture(gColorSpec, TexCoord).a;
    

	
    // then calculate lighting as usual
    // ambient
	vec3 result = Albedo * 0.1f; // hard-coded ambient component
    vec3 viewDir = normalize(-FragPos);
	
	result += CalcDirLight(dirLight, Normal, viewDir, Albedo, Specular);
	
    for(int i = 0; i < NR_POINT_LIGHTS; ++i)
		result += CalcPointLight(pointLights[i], Normal, FragPos, viewDir, Albedo, Specular);

	FragColor = vec4(result, 1.0f);
}