#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;

uniform float gamma;    // Used for gamma correction

void main() {

	vec3 result = lightColor;
	// Gamma correction
	float gam = gamma == 0.0 ? 1.0 : gamma;    // Avoid the 0 exponent
	result = pow(result, vec3(1.0 / gam));

	FragColor = vec4(result, 1.0);
}