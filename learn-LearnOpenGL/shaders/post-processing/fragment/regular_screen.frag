#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

// Temporarily control output color
uniform bool alpha;

void main() {
	vec4 color = texture(screenTexture, TexCoords);

	if (alpha)
		FragColor = vec4(color.a, color.a, color.a, 1.0);
	else
		FragColor = vec4(color.rgb, 1.0);
}