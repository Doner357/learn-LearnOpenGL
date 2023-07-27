#version 330 core

in vec2 TexCoords;   // Texture coordinate
in vec3 FragPos;     // Fragment position
in vec3 Normal;      // Fragment normal(denormalized)

out vec4 FragColor;

// Properties struct
//------------------------------
// --material--
struct Material {
	//vec3 ambient;      // This is same as diffuse color, we don't need this now since we get it from diffuse color
	sampler2D diffuse;   // Get the ambient and diffuse colors from texture called diffuse map
	sampler2D specular;  // Get specular color from specular map
	float shininess;     // shininess value
};

// --direction light--
struct DirLight {
	vec3 direction;      // light's direction

	vec3 ambient;        // ambient  light's color & intensity
	vec3 diffuse;        // diffuse  light's color & intensity
	vec3 specular;       // specular light's color & intensity
};

// --point light--
struct PointLight {
	vec3 position;       // light's position

	float constant;      // constant for attenuation
	float linear;        // linear for attenuation
	float quadratic;     // quadratic for attenuation

	vec3 ambient;        // ambient  light's color & intensity
	vec3 diffuse;        // diffuse  light's color & intensity
	vec3 specular;       // specular light's color & intensity
};
#define NR_POINT_LIGHTS 4   // Use pre-processor directive in GLSL to define the number of point lights

struct SpotLight {
	vec3 position;       // light's position
	vec3 direction;      // light's direction

	float innerCutOff;   // inner cone cosine cut off for spot light's soft edges
	float outerCutOff;   // outer cone cosine cut off for spot light's soft edges
	
	float constant;      // constant for attenuation
	float linear;        // linear for attenuation
	float quadratic;     // quadratic for attenuation

	vec3 ambient;        // ambient  light's color & intensity
	vec3 diffuse;        // diffuse  light's color & intensity
	vec3 specular;       // specular light's color & intensity
};

#define NO_LIGHT vec3(0.0)


// Lighting calculation function
//------------------------------
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);                     // Directional light calculation function
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);   // Point light calculation function
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);     // Spot light calculation function


// Uniform
//------------------------------
uniform Material material;                         // Material attributes
uniform DirLight dirLight;                         // Directional light
uniform PointLight pointLights[NR_POINT_LIGHTS];   // Point light
uniform SpotLight spotLight;                       // Spot light
uniform vec3 viewPos;                              // Viewer position

void main() {
	// Properties
	vec3 norm = normalize(Normal);
	vec3 viewerDir = normalize(viewPos - FragPos);
	vec3 result = vec3(0.0);

	// Phase 1: Directional lighting
	result += dirLight.direction == NO_LIGHT ? vec3(0.0) : CalcDirLight(dirLight, norm, viewerDir);
	// Phase 2: Point lights
	for(int i = 0; i < NR_POINT_LIGHTS; i++) {
		if (pointLights[i].constant == 0)
			break;
		result += CalcPointLight(pointLights[i], norm, FragPos, viewerDir);
	}
	// Phase 3: Spot light
	result += spotLight.direction == NO_LIGHT ? vec3(0.0) : CalcSpotLight(spotLight, norm, FragPos, viewerDir);

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {

	// Basic data calculation
	//--------------------------
	vec3 lightDir = normalize(-light.direction);   // light's direction

	// Lighting Calculation
	//--------------------------
	// --ambient--
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// --diffuse--
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	// --specular--
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

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
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// --diffuse--
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	// --specular--
	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

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
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// --diffuse--
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	// --specular--
	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

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