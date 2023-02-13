#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "learnopengl/shader_m.h"
#include "learnopengl/camera_plus.h"

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const *path);

// Screen Width and Height setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;   // Time between current frame and last frame
float lastFrame = 0.0f;   // Time of last frame

// The positions of point light
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

// Point light's color
glm::vec3 pointLightColors[] = {
	glm::vec3(0.8f, 0.2f, 0.1f),
	glm::vec3(0.8f, 0.2f, 0.1f),
	glm::vec3(0.8f, 0.2f, 0.1f),
	glm::vec3(0.8f, 0.2f, 0.1f)
};



int main(void) {

	/*
	* glfw: Initialize and configure
	* --------------------------------------------------------------------------------------------------------------------
	*/

	// Initialize the glfw
	glfwInit();

	// Tell the glfw what version of opengl do we want to use, this time we use version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell glfw what kind of profile we want to use, this time is core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* If you are a Mac OS X user, you have to add this code: */
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif



	/*
	* glfw: Window creation
	* --------------------------------------------------------------------------------------------------------------------
	*/

	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

	// Check if the window open successfully
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Register the call back function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



	/*
	* glad: load all OpenGL function pointers
	* --------------------------------------------------------------------------------------------------------------------
	*/

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// Configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);



	/*
	* Extra function
	* --------------------------------------------------------------------------------------------------------------------
	*/

	// You can enable this function to querying how many vertex attribute that your hardware allow
	/*
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	*/



	/*
	* Build and compile shader program
	* --------------------------------------------------------------------------------------------------------------------
	*/

	Shader lightingShader("shaders/vertex/colors.vert", "shaders/fragment/colors.frag");
	Shader lightCubeShader("shaders/vertex/light_cube.vert", "shaders/fragment/light_cube.frag");



	/*
	* Set up vertex data (and buffer(s)) and configure vertex attributes
	* --------------------------------------------------------------------------------------------------------------------
	*/

	// Vertex attributes: positions, normal
	float vertices[] = {
		// Positions            // Normals             // Texture coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,     0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,     1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,     1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,     1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,     0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,     0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f
	};

	// Ten different container's positions
	glm::vec3 cubePositions[] = {
	glm::vec3( 0.0f,  0.0f,  0.0f),
	glm::vec3( 2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3( 2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3( 1.3f, -2.0f, -2.5f),
	glm::vec3( 1.5f,  2.0f, -2.5f),
	glm::vec3( 1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Create vertex objects
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	// Bind VAO and VBO
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy data to memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Set up attributes pointer
	// --positions--
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// --normal--
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// --texture coordinats
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// Unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind cubeVAO
	glBindVertexArray(0);


	// Create lighting cube VAO
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	// Bind lightVAO
	glBindVertexArray(lightCubeVAO);
	// We only need to bind to the VBO, the container's VBO's data already contains the data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set up attributes pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	//Unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Unbind VAO
	glBindVertexArray(0);



	/*
	 * Texture loading
	 * --------------------------------------------------------------------------------------------------------------------
	 */

	 /* Tell stb_image.h to flip loaded texture's on the y-axis. */
	stbi_set_flip_vertically_on_load(true);

	unsigned int diffuseMap = loadTexture("textures/container2.png");
	unsigned int specularMap = loadTexture("textures/container2_specular.png");



	/*
	 * Uniform value setting
	 * --------------------------------------------------------------------------------------------------------------------
	 */
	
	// Texture setting
	//-------------------------
	// Activate shader
	lightingShader.use();
	// Set up texture unit
	lightingShader.setInt("material.diffuse", 0);    // diffuse map
	lightingShader.setInt("material.specular", 1);   // specular map

	
	glUseProgram(0);


	/*
	 * Render type setting
	 * --------------------------------------------------------------------------------------------------------------------
	 */

	 /*Enable this if you wnat to draw triangle in wireframe mode*/
	 //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



	 /*
	  * Render loop
	  * --------------------------------------------------------------------------------------------------------------------
	  */

	while (!glfwWindowShouldClose(window)) {

		// Calculate delta time
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		//--------------------------------------------------
		processInput(window);

		// Render command
		//--------------------------------------------------
		// Clear Buffer
		glClearColor(0.01f, 0.005f, 0.005f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Create transformations
		//-------------------------
		// --model matrix--
		// Since each cube has its own position, we declare the matrix variable here
		glm::mat4 model;
		// --view matrix--
		glm::mat4 view = camera.GetViewMatrix();
		// --projection matrix--
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


		// Render color cube
		//-------------------------
		
		// Be sure activate the shader when setting uniforms/drawing objects
		lightingShader.use();


		// Set cube material attributes
		// --ambient & diffuse--
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);                        // Take diffuse map's color as ambient and diffuse color
		// --specular--
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);                       // Take specular map's color as specular color/intensity
		// --shininess--
		lightingShader.setFloat("material.shininess", 64.0f);            // shininess value

		
		// Set light properties
		// --Directional light--
		// directional light
		glm::vec3 dirLightColor = glm::vec3(0.05f, 0.01f, 0.01f);
		lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShader.setVec3("dirLight.ambient", dirLightColor * glm::vec3(0.01f));
		lightingShader.setVec3("dirLight.diffuse", dirLightColor * glm::vec3(0.1f));
		lightingShader.setVec3("dirLight.specular", dirLightColor * glm::vec3(1.0f));
		// point light 1
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader.setVec3("pointLights[0].ambient", pointLightColors[0] * glm::vec3(0.05f));
		lightingShader.setVec3("pointLights[0].diffuse", pointLightColors[0] * glm::vec3(0.1f));
		lightingShader.setVec3("pointLights[0].specular", pointLightColors[0] * glm::vec3(1.0f));
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09f);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShader.setVec3("pointLights[1].ambient", pointLightColors[1] * glm::vec3(0.05f));
		lightingShader.setVec3("pointLights[1].diffuse", pointLightColors[1] * glm::vec3(0.1f));
		lightingShader.setVec3("pointLights[1].specular", pointLightColors[1] * glm::vec3(1.0f));
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.09f);
		lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2] * glm::vec3(0.05f));
		lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2] * glm::vec3(0.1f));
		lightingShader.setVec3("pointLights[2].specular", pointLightColors[2] * glm::vec3(1.0f));
		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.09f);
		lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		lightingShader.setVec3("pointLights[3].ambient", pointLightColors[3] * glm::vec3(0.05f));
		lightingShader.setVec3("pointLights[3].diffuse", pointLightColors[3] * glm::vec3(0.1f));
		lightingShader.setVec3("pointLights[3].specular", pointLightColors[3] * glm::vec3(1.0f));
		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.09f);
		lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
		// spotLight
		lightingShader.setVec3("spotLight.position", camera.Position);
		lightingShader.setVec3("spotLight.direction", camera.SightFront);
		lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("spotLight.constant", 1.0f);
		lightingShader.setFloat("spotLight.linear", 0.22f);
		lightingShader.setFloat("spotLight.quadratic", 0.20f);
		lightingShader.setFloat("spotLight.innerCutOff", glm::cos(glm::radians(12.5f)));
		lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


		glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(model)));   // Calculate normal matrix in CPU

		// Send translation matrices to uniforms
		//lightingShader.setMat4("model", model);             // Since we have mutiple cubes have to draw, we will set model matrix in following loop
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat3("normalMat", normalMat);
		lightingShader.setVec3("viewPos", camera.Position);   // Viewer position

		// Bind VAO
		glBindVertexArray(cubeVAO);
		// Set model matrix and render the containers
		for (unsigned int i = 0; i < 10; i++) {


			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(cubePositions[i]));
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			// Send model matrix to uniform
			lightingShader.setMat4("model", model);

			// Calculate normal matrix and send it to uniform
			glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(model)));
			lightingShader.setMat3("normalMat", normalMat);
			
			// Render the cube
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}


		// Render lamp cube      // Not for this time
		//-------------------------
		// Activate the shader
		lightCubeShader.use();


		// Send translation matrices to uniforms
		lightCubeShader.setMat4("view", view);
		lightCubeShader.setMat4("projection", projection);

		// Render the cube
		glBindVertexArray(lightCubeVAO);
		for (int i = 0; i < sizeof(pointLightPositions) / sizeof(pointLightPositions[0]); i++) {
			// Set light color
			lightCubeShader.setVec3("lightColor", pointLightColors[i]);


			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));            // A smaller cube

			// Send to the uniform
			lightCubeShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// glfw: Swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		//-----------------------------------------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// --------------------------------------------------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &diffuseMap);
	glDeleteTextures(1, &specularMap);
	lightingShader.clear();
	lightCubeShader.clear();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// --------------------------------------------------------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// --------------------------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// --------------------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)                     // Press the ESC
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(CAMERA_FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(CAMERA_BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(CAMERA_LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(CAMERA_RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(CAMERA_UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(CAMERA_DOWN, deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
// --------------------------------------------------------------------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	// Check if the window is first time be clicked
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	// Calculate the x-axis and y-axis offset
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;  // reversed since y-coordinates go from top to bottom
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// --------------------------------------------------------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path" << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}