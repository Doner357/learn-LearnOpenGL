#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float shininess;
};

struct DirLight {       // size  ali
	vec3 direction;     // 12    0

	vec3 ambient;       // 12    12 + 4 = 16
	vec3 diffuse;       // 12    28 + 4 = 32
	vec3 specular;      // 12    44 + 4 = 48
}; // total 60 + 4 = 64 bytes
#define NR_DIRECT_LIGHTS 4

struct PointLight {		// size  ali
	vec3 position;		// 12    0

	float constant;		//  4    12
	float linear;       //  4    16
	float quadratic;    //  4    20
	
	vec3 ambient;       // 12    24 + 8 = 32
	vec3 diffuse;       // 12    44 + 4 = 48
	vec3 specular;      // 12    60 + 4 = 64
}; // total 76 + 4 = 80 bytes
#define NR_POINT_LIGHTS 32

struct SpotLight {      // size  ali
	vec3 position;      // 12    0
	vec3 direction;     // 12    12 + 4 = 16

	float innerCutOff;  //  4    28
	float outerCutOff;  //  4    32

	float constant;     //  4    36
	float linear;       //  4    40
	float quadratic;    //  4    44
	
	vec3 ambient;       // 12    48
	vec3 diffuse;       // 12    60 + 4 = 64
	vec3 specular;      // 12    76 + 4 = 80
}; // total 92 + 4 = 96 bytes
#define NR_SPOT_LIGHTS 8

#define NO_LIGHT vec3(0.0)


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


uniform vec3 viewPos;
uniform Material material;
layout (std140) uniform GlobalLights {            // size      ali
	DirLight dirLights[NR_DIRECT_LIGHTS];         //  256        0
	PointLight pointLights[NR_POINT_LIGHTS];      // 2560	   256
	SpotLight spotLight[NR_SPOT_LIGHTS];          //  768     2816
}; // total 3584

void main() {
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = vec3(0.0);
	
	for(int i = 0; i < NR_DIRECT_LIGHTS; i++)
		result += dirLights[i].direction == vec3(0.0) ? vec3(0.0) : CalcDirLight(dirLights[i], norm, viewDir);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += pointLights[i].position == vec3(0.0) ? vec3(0.0) : CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	for(int i = 0; i < NR_SPOT_LIGHTS; i++)
		result += spotLight[i].direction == vec3(0.0) ? vec3(0.0) : CalcSpotLight(spotLight[i], norm, FragPos, viewDir);	
	

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {

	// Basic data calculation
	//--------------------------
	vec3 lightDir = normalize(-light.direction);   // light's direction

	// Lighting Calculation
	//--------------------------
	// --ambient--
	vec3 ambient = light.ambient * material.ambient;

	// --diffuse--
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * material.diffuse;

	// --specular--
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * material.specular;

	// Result
	//--------------------------
	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	// Basic data calculation
	//--------------------------
	vec3 lightDir = normalize(light.position - fragPos);   // light direction

	// Lighting Calculation
	//--------------------------
	// --ambient--
	vec3 ambient = light.ambient * material.ambient;

	// --diffuse--
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * material.diffuse;

	// --specular--
	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * material.specular;

	// --attenuation--
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	// Result
	//--------------------------
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;
	return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	// Basic data calculation
	//--------------------------
	vec3 lightDir = normalize(light.position - fragPos);        // light direction

	
	// Lighting Calculation
	//--------------------------
	// --ambient--
	vec3 ambient = light.ambient * material.ambient;

	// --diffuse--
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * material.diffuse;

	// --specular--
	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * material.specular;

	// --attenuation--
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	// --spotlight intensity--
	float theta     = dot(lightDir, normalize(-light.direction));   // the cosine of the angle between spotlight direction and light direction
	float epsilon   = light.innerCutOff - light.outerCutOff;      // the cosine difference between the inner cone and outer cone
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    // the intensity of spotlight


	// Result
	//--------------------------
	ambient  *= attenuation;
	diffuse  *= attenuation * intensity;
	specular *= attenuation * intensity;

	return ambient + diffuse + specular;
}