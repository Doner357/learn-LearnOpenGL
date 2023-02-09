#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;   // Cube color
uniform vec3 lightColor;    // Light color
uniform vec3 lightPos;      // Light position
uniform vec3 viewPos;       // Viewer position

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform int shininess;

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
	vec3 ambient = ambientStrength * lightColor;

	// --Diffuse--
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * diffuseStrength * lightColor;

	// --Specular--
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = spec * specularStrength * lightColor;

	// --Result--
	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}