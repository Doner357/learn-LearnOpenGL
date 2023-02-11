#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

// Define material attributes
struct Material {
	vec3 ambient;             // ambient  color
	vec3 diffuse;             // diffuse  color
	vec3 specular;            // specular color
	float shininess;          // shininess value
};

uniform Material material;    // Material attributes
uniform vec3 lightColor;      // Light color
uniform vec3 lightPos;        // Light position
uniform vec3 viewPos;         // Viewer position

void main() {
	
	// Basic data calculation
	//--------------------------
	// Fragment normal
	vec3 norm = normalize(Normal);
	// Light direction
	vec3 lightDir = normalize(lightPos - FragPos);
	// View direction
	vec3 viewDir = normalize(viewPos - FragPos);
	// Reflect direction
	vec3 reflectDir = reflect(-lightDir, norm);

	// Lighting Calculation
	//--------------------------

	// --Ambient--
	vec3 ambient = lightColor * material.ambient;

	// --Diffuse--
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightColor * (diff * material.diffuse);

	// --Specular--
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = lightColor * (spec * material.specular);

	// --Result--
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}