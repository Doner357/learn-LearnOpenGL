#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 VertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat;

uniform vec3 objectColor;   // Cube color
uniform vec3 lightColor;    // Light color
uniform vec3 lightPos;      // Light position
uniform vec3 viewPos;       // Viewer position

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vec3 vertexPos = vec3(model * vec4(aPos, 1.0));

	/* The matrix calculation below is normal matrix, which can convert normal vector to model space.
	Inversing matrices is a costly operation for shaders, You'd better calculate this in
	CPU and send it to the shaders via a uniform before drawing. */
	//Normal = mat3(transpose(inverse(model))) * aNormal;

	vec3 Normal =  normalMat * aNormal;   // Get normal matrix from CPU

		// Basic data calculation
	//--------------------------
	// Fragment normal
	vec3 norm = normalize(Normal);
	// Light direction
	vec3 lightDir = normalize(lightPos - vertexPos);
	// View direction
	vec3 viewDir = normalize(viewPos - vertexPos);
	// Reflect direction
	vec3 reflectDir = reflect(-lightDir, norm);

	// Lighting Calculation
	//--------------------------

	// --Ambient--
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// --Diffuse--
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// --Specular--
	float specularStrength = 0.5;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = spec * specularStrength * lightColor;

	// --Result--
	VertexColor = (ambient + diffuse + specular) * objectColor;
}