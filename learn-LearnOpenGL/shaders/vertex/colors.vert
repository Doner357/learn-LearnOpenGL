#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(view * model * vec4(aPos, 1.0));

	/* The matrix calculation below is normal matrix, which can convert normal vector to model space.
	Inversing matrices is a costly operation for shaders, You'd better calculate this in
	CPU and send it to the shaders via a uniform before drawing. */
	//Normal = mat3(transpose(inverse(model))) * aNormal;

	Normal =  normalMat * aNormal;   // Get normal matrix from CPU
}