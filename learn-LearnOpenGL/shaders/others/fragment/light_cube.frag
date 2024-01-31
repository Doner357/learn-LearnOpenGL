#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;

void main() {

	vec3 result = lightColor;

	FragColor = vec4(result, 1.0);
}