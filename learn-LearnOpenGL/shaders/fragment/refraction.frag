#version 330 core
in CubeInfo {
	vec3 Position;
	vec3 Normal;
} fs_in;

out vec4 FragColor;

uniform vec3 viewPos;
uniform samplerCube cubemap;

void main() {
	float ratio = 1.00 / 1.52;
	vec3 viewDir = normalize(fs_in.Position - viewPos);
	vec3 reflectDir = refract(viewDir, normalize(fs_in.Normal), ratio);
	FragColor = vec4(texture(cubemap, reflectDir).rgb, 1.0);
}