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

bool isEdge()
{
	vec3 gx = texture(gNormal, vec2((gl_FragCoord.x + 1) / 800, (gl_FragCoord.y + 1) / 600)).rgb +
			  2 * texture(gNormal, vec2((gl_FragCoord.x + 1) / 800, (gl_FragCoord.y) / 600)).rgb +
			  texture(gNormal, vec2((gl_FragCoord.x + 1) / 800, (gl_FragCoord.y - 1) / 600)).rgb -
			  ( texture(gNormal, vec2((gl_FragCoord.x - 1) / 800, (gl_FragCoord.y + 1) / 600)).rgb +
			  2 * texture(gNormal, vec2((gl_FragCoord.x - 1) / 800, (gl_FragCoord.y) / 600)).rgb +
			  texture(gNormal, vec2((gl_FragCoord.x - 1) / 800, (gl_FragCoord.y - 1) / 600)).rgb );

	vec3 gy = texture(gNormal, vec2((gl_FragCoord.x + 1) / 800, (gl_FragCoord.y + 1) / 600)).rgb +
			  2 * texture(gNormal, vec2((gl_FragCoord.x) / 800, (gl_FragCoord.y + 1) / 600)).rgb +
			  texture(gNormal, vec2((gl_FragCoord.x - 1) / 800, (gl_FragCoord.y + 1) / 600)).rgb -
			  ( texture(gNormal, vec2((gl_FragCoord.x + 1) / 800, (gl_FragCoord.y - 1) / 600)).rgb +
			  2 * texture(gNormal, vec2((gl_FragCoord.x) / 800, (gl_FragCoord.y - 1) / 600)).rgb +
			  texture(gNormal, vec2((gl_FragCoord.x - 1) / 800, (gl_FragCoord.y - 1) / 600)).rgb );

	//vec3 gradient = sqrt(gx * gx + gy * gy);
	vec3 gradient = abs(gx) + abs(gy); // max value per component is 8, min is 0
	float threshold = 4; // max per component * 50%
	return max(max(gradient.r, gradient.g), gradient.b) > threshold;
}

void main()
{
	vec2 TexCoord = vec2(gl_FragCoord.x / 800, gl_FragCoord.y / 600);
    // retrieve data from G-buffer
    vec3 Normal = texture(gNormal, TexCoord).rgb;
	if (Normal == vec3(0.0f))
		discard;
    vec3 FragPos = texture(gPosition, TexCoord).rgb;
    vec3 Albedo = texture(gColorSpec, TexCoord).rgb;
    float Specular = texture(gColorSpec, TexCoord).a;
	vec3 viewDir = normalize(-FragPos);

	vec3 result;

	//TODO: move the if part to a separate shader, as this if conditional is poorly optimized by GLSL
	if(lightID < 0) //directional Light
	{
		if(isEdge())
		{
			FragColor = vec4(0.0f);
			return;
		}
		else
		{
			// ambient
			result = Albedo * 0.1f; // hard-coded ambient component
		
			result += CalcDirLight(dirLight, Normal, viewDir, Albedo, Specular);
		}
	}
	else
		result = CalcPointLight(pointLights[lightID], Normal, FragPos, viewDir, Albedo, Specular);
	
	FragColor = vec4(result, 1.0f);
}