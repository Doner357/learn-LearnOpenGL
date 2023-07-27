#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;   // Texture coordinate
out vec3 FragPos;     // Fragment position
out vec3 Normal;      // Normal

uniform mat4 model;
layout (std140) uniform Matrices {
	mat4 view;
	mat4 projection;
};
uniform mat3 normalMat;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	
	TexCoords = aTexCoords;
	FragPos = vec3(model * vec4(aPos, 1.0));

	/* The matrix calculation below is normal matrix, which can convert normal vector to model space.
	Inversing matrices is a costly operation for shaders, You'd better calculate this in
	CPU and send it to the shaders via a uniform before drawing. */
	//Normal = mat3(transpose(inverse(model))) * aNormal;

	Normal =  normalMat * aNormal;   // Get normal matrix from CPU
}