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
	vec3 position;       // light's position
	vec3 direction;      // light's direction (but this time it's for spotlight direction, not the real light direction)
	float cutOff;        // light's inner cut off
	float outerCutOff;   // light's outer cut off

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

	// --Basic Phong lighting elements--
	// Fragment normal
	vec3 norm = normalize(Normal);
	// Light direction
	vec3 lightDir = normalize(light.position - FragPos);
	// View direction
	vec3 viewDir = normalize(viewPos - FragPos);
	// Reflect direction
	vec3 reflectDir = reflect(-lightDir, norm);

	// --Attenuation elements--
	// Distance between point light and fragment
	float distance = length(light.position - FragPos);
	// Light attenuation
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// --Spot light elements--
	// The cosine of the angle between spotlight direction and light direction
	float theta = dot(lightDir, -light.direction);       // Note that we negate the spotlihgt direction, it needs to point towards the light source
	// Epsilon ( cosine difference between the inner and the outer cone)
	float epsilon = light.cutOff - light.outerCutOff;
	// Intensity ( For spotlight effects on fragment )
	// clamp function return the medium large number, note that the second number should bigger than the third one
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);


	// Lighting Calculation
	//--------------------------

	if(theta < light.cutOff) {
		// --Ambient--
		vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

		// --Diffuse--
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

		// --Specular--
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

		// --Spotlight (soft edges)--
		// we'll leave ambient unaffected so we always have a little light.
		diffuse  *= intensity;
		specular *= intensity;

		// --Attenuation--
		//ambient  *= attenuation;   // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branch
		diffuse  *= attenuation;
		specular *= attenuation;

		// --Result--
		vec3 result = ambient + diffuse + specular;
		FragColor = vec4(result, 1.0);
	}
	else {
		FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);
	}



}