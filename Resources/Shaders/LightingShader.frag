#version 330 core
out vec4 FragColor;

in vec3 FragPos; 
in vec3 normal;
in vec2 TexCoord;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};
struct DirLight {
    vec3 direction;		// in view coordinates
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight {
    vec3 position;		// in view coordinates
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct SpotLight {
    vec3  position;		// in view coordinates
    vec3  direction;	// in view coordinates
    float cutOff;
	float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform DirLight dirLight;
#define NR_POINT_LIGHTS 1  
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

//vec3 flatten(vec3 color)
//{
//	float numColors=4.0f;
//	return vec3(round(color.x*numColors)/numColors, round(color.y*numColors)/numColors, round(color.z*numColors)/numColors);
//}
float flatten(float value)
{
	float numColors=2.0f;
	return round(value*numColors)/numColors;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 diffuse  = light.diffuse  * flatten(diff) * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 specular = light.specular * flatten(spec) * vec3(texture(material.texture_specular1, TexCoord));
    //if(gl_FragCoord.x < 400)
		return (ambient + diffuse + specular);
	//else
	//	return ambient + diffuse;
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 diffuse  = light.diffuse  * flatten(diff*attenuation) * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 specular = light.specular * flatten(spec*attenuation) * vec3(texture(material.texture_specular1, TexCoord));
    ambient  *= flatten(attenuation);
    //diffuse  *= attenuation;
    //specular *= attenuation;
    //if(gl_FragCoord.x < 400)
		return (ambient + diffuse + specular);
	//else
	//	return ambient + diffuse;
}
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 
	
	// do lighting calculations
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance    = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
  					light.quadratic * (distance * distance));
	// combine results
	vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoord));
	vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoord));
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;
	diffuse *= intensity;
	specular *= intensity;

	return (ambient + diffuse + specular);
}

void main()
{
	// properties
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(-FragPos); // camera is positioned at <0,0,0>

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    //vec3 result=vec3(0.0f);
	// phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // phase 3: Spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
	
	if(result.x > 1.0f)
		result.x = 1.0f;
	if(result.y > 1.0f)
		result.y = 1.0f;
	if(result.z > 1.0f)
		result.z = 1.0f;

    FragColor = vec4(result, 1.0);
    //FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
	//FragColor = texture(material.texture_diffuse1, TexCoord);

	//FragColor = vec4(norm,1.0f); //test Normals
}