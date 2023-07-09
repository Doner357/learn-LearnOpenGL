#version 330 core
in CubeInfo {
	vec3 Position;
	vec3 Normal;	
} fs_in;

out vec4 FragColor;

uniform vec3 viewPos;
uniform samplerCube cubemap;

void main() {
	vec3 viewDir = normalize(fs_in.Position - viewPos);
	vec3 reflectDir = reflect(viewDir, normalize(fs_in.Normal));
	FragColor = vec4(texture(cubemap, reflectDir).rgb, 1.0);
}