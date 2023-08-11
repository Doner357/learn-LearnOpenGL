#version 330 core
in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube cubemap;

uniform float gamma;    // Used for gamma correction

void main() {

	vec3 result = texture(cubemap, TexCoords).rgb;
	
	// Gamma correction
	float gam = gamma == 0.0 ? 1.0 : gamma;    // Avoid 0 as exponent
	result = pow(result, vec3(1.0 / gam));
	
	float alpha = texture(cubemap, TexCoords).a;
	FragColor = vec4(result, alpha);
}