#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D frontTexture;
uniform sampler2D backTexture;


void main()	{
	if(gl_FrontFacing)
		FragColor = texture(frontTexture, TexCoord);
	else
		FragColor = texture(backTexture, TexCoord);
}