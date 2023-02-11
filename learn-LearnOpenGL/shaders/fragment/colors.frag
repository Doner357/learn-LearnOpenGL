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
	sampler2D emission;  // Get emission color from emission map
	float shininess;     // shininess value
};

// Define light properties
struct Light {
	vec3 position;     // light's position

	vec3 ambient;      // light's ambient  intensity
	vec3 diffuse;      // light's diffuse  intensity
	vec3 specular;     // light's specular intensity
};

uniform Material material;    // Material attributes
uniform Light light;          // Lighti properties
uniform vec3 viewPos;         // Viewer position

// Exercises part
uniform float texCoordOffset;   // The offset which will change overtime

void main() {
	
	// Basic data calculation
	//--------------------------
	// Fragment normal
	vec3 norm = normalize(Normal);
	// Light direction
	vec3 lightDir = normalize(light.position - FragPos);
	// View direction
	vec3 viewDir = normalize(viewPos - FragPos);
	// Reflect direction
	vec3 reflectDir = reflect(-lightDir, norm);

	// Lighting Calculation
	//--------------------------

	// --Ambient--
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// --Diffuse--
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// --Specular--
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	
	// --Emission--
	vec3 emission = vec3(texture(material.emission, vec2(TexCoords.x, TexCoords.y + texCoordOffset)));
	
	// --Result--
	vec3 result = ambient + diffuse + specular;
	if(vec3(texture(material.specular, TexCoords)).r < 0.1)
		result += emission;
	FragColor = vec4(result, 1.0);
}