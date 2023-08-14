#version 330 core
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;
uniform float gamma;

void main() {
	vec3 result = texture(screenTexture, TexCoords).rgb;

	float gam = (gamma == 0.0 ? 1.0 : gamma);    // Avoid 0 exponent
	result = pow(result, vec3(1.0 / gam));

	float alpha = texture(screenTexture, TexCoords).a;
	FragColor = vec4(result, alpha);
}