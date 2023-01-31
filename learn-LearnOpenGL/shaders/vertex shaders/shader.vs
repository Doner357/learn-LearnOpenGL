#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform float xoffSet;   // Get the off set value from uniform

void main() {
	gl_Position = vec4(aPos.x + xoffSet, aPos.y, aPos.z, 1.0);   // Add the off set to the x value
	ourColor = aColor;
}