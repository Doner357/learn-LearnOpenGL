#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main() {
	
	// Basic data calculation
	//--------------------------
	// Fragment normal
	vec3 norm = normalize(Normal);
	// Light direction
	vec3 lightDir = normalize(lightPos - FragPos);

	// Lighting Calculation
	//--------------------------

	// --Ambient--
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// --Diffuse--
	float diff = max(dot(norm, lightDir), 0.0);   // 
	vec3 diffuse = diff * lightColor;

	// --Result--
	vec3 result = (ambient + diffuse) * objectColor;
	FragColor = vec4(result, 1.0);
}