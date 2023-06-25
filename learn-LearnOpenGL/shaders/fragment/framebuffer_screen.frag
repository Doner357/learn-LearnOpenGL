#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main() {
	FragColor = texture(screenTexture, vec2(1.0 - TexCoords.x, TexCoords.y));   // Invert the x-coordinate to simulate mirror reflection
}