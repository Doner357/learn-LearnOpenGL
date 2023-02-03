#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	vec4 texColor1 = texture(texture1, TexCoord);
	vec4 texColor2 = texture(texture2, TexCoord);
	FragColor = mix(texColor1, texColor2, 0.2 * texColor2.a);
}