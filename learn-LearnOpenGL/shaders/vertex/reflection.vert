#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out CubeInfo {
	vec3 Position;
	vec3 Normal;	
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat;

void main() {
	vs_out.Normal = normalMat * aNormal;
	vs_out.Position = (model * vec4(aPos, 1.0)).xyz;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}