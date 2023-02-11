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

// The podition of light
glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);



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
		// Positions          // Normal
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
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
	//stbi_set_flip_vertically_on_load(true);



	/*
	 * Uniform value setting
	 * --------------------------------------------------------------------------------------------------------------------
	 */
	
	/* This time we send all uniform value by using shader class */



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
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Create transformations
		//-------------------------
		// --model matrix--
		// Since each cube has its own position, we initialize the matrix here
		glm::mat4 model = glm::mat4(1.0f);
		// --view matrix--
		glm::mat4 view = glm::mat4(1.0f);
		// Get look at matrix from camera
		view = camera.GetViewMatrix();
		// --projection matrix--
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


		// Render color cube
		//-------------------------
		
		// Be sure activate the shader when setting uniforms/drawing objects
		lightingShader.use();

		
		// Set cube material attributes
		lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);   // ambient  color
		lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);   // diffuse  color
		lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);   // specular color
		lightingShader.setFloat("material.shininess", 32.0f);            // shininess value

		// Set the lighting properties
		glm::vec3 lightColor;
		// Change light's color over time
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);
		
		// Multiply light color with intensity
		glm::vec3 ambientColor  = lightColor * glm::vec3(0.2f);
		glm::vec3 diffuseColor  = lightColor * glm::vec3(0.5f);

		// Send the properties to uniforms
		lightingShader.setVec3("light.position", lightPos);
		lightingShader.setVec3("light.ambient", ambientColor);       // light's ambient  intensity
		lightingShader.setVec3("light.diffuse", diffuseColor);       // light's diffuse  intensity
		lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);     // light's specular intensity
		

		// Set the cube position to the model matrix
		model = glm::mat4(1.0f);

		/* The matrix below is normal matrix, which can convert normal vector to model space.
		Inversing matrices is a costly operation for shaders, You'd better calculate this in
		CPU and send it to the shaders via a uniform before drawing. */
		glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(model)));   // Calculate normal matrix in CPU

		// Send translation matrices to uniforms
		lightingShader.setMat4("model", model);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat3("normalMat", normalMat);
		lightingShader.setVec3("viewPos", camera.Position);   // Viewer position

		// Render the cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Render lamp cube
		//-------------------------
		
		// Activate the shader
		lightCubeShader.use();


		// Set the cube position to the model matrix
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));   // A smaller cube

		// Send translation matrices to uniforms
		lightCubeShader.setMat4("model", model);
		lightCubeShader.setMat4("view", view);
		lightCubeShader.setMat4("projection", projection);

		// Render the cube
		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


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