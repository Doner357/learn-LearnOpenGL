#version 330 core
in vec3 Position;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPos;
uniform samplerCube cubemap;

void main() {
	float ratio = 1.00 / 1.52;
	vec3 viewDir = normalize(Position - viewPos);
	vec3 reflectDir = refract(viewDir, normalize(Normal), ratio);
	FragColor = vec4(texture(cubemap, reflectDir).rgb, 1.0);
}