#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture1;

void main() {
	vec4 texColor = texture(texture1, TexCoords);
	// Discard the fragment if it is almost transparent
	if (texColor.a < 0.1)
		discard;
	FragColor = texColor;
}