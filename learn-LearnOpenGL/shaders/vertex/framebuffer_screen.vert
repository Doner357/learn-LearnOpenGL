#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 screen_position;

void main() {
	vec4 position = vec4(aPos, 0.0, 1.0);
	gl_Position = screen_position * position;
	TexCoords = aTexCoords;
}