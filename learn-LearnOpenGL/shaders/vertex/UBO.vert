#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices {
						// Base alignment		// aligned offset
	mat4 projection;	//	16					//	0
						//	16					//	16
						//	16					//	32
						//	16					//	48
	mat4 view;			//	16					//	64
						//	16					//	80
						//	16					//	96
						//	16					//	112
};	// Totoal 128 bytes
uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}