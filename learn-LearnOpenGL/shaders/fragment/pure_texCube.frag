#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture1;
uniform bool linear;

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth) {
	float z = depth * 2.0 - 1.0;   // Back to NDC
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
	//FragColor = texture(texture1, TexCoords);
	if(linear){
		float depth = LinearizeDepth(gl_FragCoord.z) / far;  //divide by far for demonstration
		FragColor = vec4(vec3(depth), 1.0);
	}
	else {
		FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
	}

}