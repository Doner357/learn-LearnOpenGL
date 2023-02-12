#version 330 core

in vec2 TexCoords;   // Texture coordinate
in vec3 FragPos;     // Fragment position
in vec3 Normal;      // Fragment normal(denormalized)

out vec4 FragColor;

// Define material attributes
struct Material {
	//vec3 ambient;      // This is same as diffuse color, we don't need this now since we get it from diffuse color
	sampler2D diffuse;   // Get the ambient and diffuse colors from texture called diffuse map
	sampler2D specular;  // Get specular color from specular map
	float shininess;     // shininess value
};

// Define light properties
struct Light {
	vec3 position;       // We use position since this time we implement point light
	//vec3 direction;    // Not for this time

	vec3 ambient;        // light's ambient  intensity
	vec3 diffuse;        // light's diffuse  intensity
	vec3 specular;       // light's specular intensity

	// Attenuation elements
	float constant;
	float linear;
	float quadratic;
};

uniform Material material;    // Material attributes
uniform Light light;          // Lighti properties
uniform vec3 viewPos;         // Viewer position

void main() {
	
	// Basic data calculation
	//--------------------------
	// Fragment normal
	vec3 norm = normalize(Normal);
	// Light direction
	vec3 lightDir = normalize(light.position - FragPos);   // Note that we negate the direction, it needs to point towards the light source
	// View direction
	vec3 viewDir = normalize(viewPos - FragPos);
	// Reflect direction
	vec3 reflectDir = reflect(-lightDir, norm);
	// Distance between point light and fragment
	float distance = length(light.position - FragPos);
	// Light attenuation
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// Lighting Calculation
	//--------------------------

	// --Ambient--
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// --Diffuse--
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	// --Specular--
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	// --Attenuation--
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	// --Result--
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}