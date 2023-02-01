#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixValue;       // This is used to control the mix of the two textures

void main() {
	vec4 textColor1 = texture(texture1, TexCoord);
	vec4 textColor2 = texture(texture2, TexCoord);
	FragColor = mix(textColor1, textColor2, mixValue * textColor2.a);  // If the texture2's alpha is 0, then the color will 100% be the texture1's color
}