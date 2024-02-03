#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

// Used for gamma correction
layout (std140) uniform GammaCorrection {
	float gamma;    // 4 bytes
};

void main() {
	vec3 color = texture(screenTexture, TexCoords).rgb;

    // Gamma correction
    color = pow(color, vec3(1.0 / gamma));

	FragColor = vec4(color, 1.0);
}