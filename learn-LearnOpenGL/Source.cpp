#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "learnopengl/shader_m.h"
#include "learnopengl/camera_plus.h"
#include "learnopengl/model.h"

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const *path);
unsigned int loadCubemap(std::vector<std::string> faces);

// Screen Width and Height setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0f;
float lastY = (float)SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;   // Time between current frame and last frame
float lastFrame = 0.0f;   // Time of last frame



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



	/*
	* OpenGL: Congifure OpenGL global state
	* --------------------------------------------------------------------------------------------------------------------
	*/

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	// Disable writing to the depth buffer
	/*
	glDepthMask(GL_FALSE);
	*/
	// Configure depth test function
	glDepthFunc(GL_LESS);


	// Enable blending
	glEnable(GL_BLEND);
	// Set up blending factors
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Set up blending constant color
	/*
	glBlendColor()
	*/
	// Set up RGBA blending factors separately
	/*
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	*/
	// Change the operator between the source and destination part of the blending equation.
	/*
	glBlendEquation(GLenum mode);
	*/


	// Enable face culling
	glEnable(GL_CULL_FACE);
	// Change the type of face we want to cull
	/*
	glCullFace(GL_FRONT);
	*/
	// Change which winding order is the front face
	/*
	glFrontFace(GL_CW);  // The clockwise is the front face
	*/


	// Enable rendering point size changes via the vertex shader
	/*
	glEnable(GL_PROGRAM_POINT_SIZE);
	*/
	



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
	Shader skyboxShader("shaders/vertex/skybox.vert", "shaders/fragment/skybox.frag");
	Shader screenShader("shaders/vertex/framebuffer_screen.vert", "shaders/fragment/framebuffer_screen.frag");
	Shader fragCoordShader("shaders/vertex/gl_FragCoord.vert", "shaders/fragment/gl_FragCoord.frag");



	/*
	* Set up vertex data (and buffer(s)) and configure vertex attributes
	* --------------------------------------------------------------------------------------------------------------------
	*/

	// Winding order vertex data
	float cubeVertices[] = {
		// positions          // normal vector
		// Back face
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f, // Bottom-left
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f, // top-right
		 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f, // bottom-right         
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f, // top-right
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f, // top-left
		// Front face
		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f, // top-right
		 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f, // top-right
		-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f, // top-left
		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f, // bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, // top-right
		-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, // top-left
		-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, // top-right
		// Right face
		 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f, // top-left
		 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f, // top-right         
		 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f, // top-left
		 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f, // top-right
		 0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f, // top-left
		 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f, // top-right
		// Top face
		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f, // top-left
		 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f, // top-right     
		 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f, // top-left
		-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f  // bottom-left
	};
	float skyboxVertices[] = {
		// positions         
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f
	};
	// Quad vertex data
	float quadVertices[] = {
		// postions	   // texture Coords
		 1.0f,  1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  0.0f,  1.0f,
		-1.0f, -1.0f,  0.0f,  0.0f,

		 1.0f, -1.0f,  1.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  0.0f,  0.0f
	};

	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glBindVertexArray(0);

	// cubemap VAO
	unsigned int cubemapVAO, cubemapVBO;
	glGenVertexArrays(1, &cubemapVAO);
	glGenBuffers(1, &cubemapVBO);
	glBindVertexArray(cubemapVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glBindVertexArray(0);	

	// Quad VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
	glBindVertexArray(0);


	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &cubemapVBO);
	glDeleteBuffers(1, &quadVBO);
	/*
	glDeleteBuffers(1, &planeVBO);	
	*/



	/*
	 * Texture loading
	 * --------------------------------------------------------------------------------------------------------------------
	 */



	/*
	 * Cubemap loading
	 * --------------------------------------------------------------------------------------------------------------------
	 */
	
	const std::string folder_path = "cubemaps/skybox_1/";
	std::vector<std::string> faces = {
		folder_path + "right.jpg",
		folder_path + "left.jpg",
		folder_path + "top.jpg",
		folder_path + "bottom.jpg",
		folder_path + "front.jpg",
		folder_path + "back.jpg"
	};
	unsigned int skyboxTexture = loadCubemap(faces);



	/*
	 * Model loading
	 * --------------------------------------------------------------------------------------------------------------------
	 */



	/*
	 * Uniform value setting
	 * --------------------------------------------------------------------------------------------------------------------
	 */

	skyboxShader.use();
	skyboxShader.setInt("cubemap", 0);

	screenShader.use();
	screenShader.setInt("screenTexture", 0);


	glUseProgram(0);



	/*
	 * Render type setting
	 * --------------------------------------------------------------------------------------------------------------------
	 */

	 /*Enable this if you wnat to draw triangle in wireframe mode*/
	 //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



	/*
	* Frambuffers creating
	* --------------------------------------------------------------------------------------------------------------------
	*/
	unsigned int framebuffer;
	// Generate a framebuffer and get its ID
	glGenFramebuffers(1, &framebuffer);
	// Bind framebuffers
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);


	// Attach texture to framebuffer
	// Create a texture to store the scene's image
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Attach the texture to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);


	// Attach Render Buffer (RBO) to framebuffer
	unsigned int RBO;
	// Generate renderbuffer and get its ID
	glGenRenderbuffers(1, &RBO);
	// Bind render buffer
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	// Creating a depth and stencil renderbuffer object
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	// Unbind renderbuffer to default
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// Actually attach the renderbuffer to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


	// Check whether the framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	// Bind framebuffer to default
	glBindFramebuffer(GL_FRAMEBUFFER, 0);




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


		// **FIRST PASS**
		//----------------------------------------------------------------------

		// Bind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		
		// Render command
		//--------------------------------------------------
		// Clear Buffer
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);


		// Create transformations
		//-------------------------
		// --model matrix--
		// Since each cube has its own position, we declare the matrix variable here
		glm::mat4 model;
		// --view matrix--
		glm::mat4 view = camera.GetViewMatrix();
		// --projection matrix--
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);



		// Render scene
		//-------------------------

		// Render cubes
		//---------------
		fragCoordShader.use();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		fragCoordShader.setMat4("model", model);
		fragCoordShader.setMat4("view", view);
		fragCoordShader.setMat4("projection", projection);
		
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);



		// skybox
		//---------------
		// Since the default value in depth buffer is 1.0, so the fragment should pass the depth test when the depth of fragment is less or equal to
		// the value store in the depth buffer. This can avoid the depth fighting.
		glDepthFunc(GL_LEQUAL);
		// Cull the front face
		glCullFace(GL_FRONT);

		// Activate the shader
		skyboxShader.use();

		// Bind cubemap
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

		// Set up the view and projection matrix, remember to remove the translation part of the view matrix
		skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
		skyboxShader.setMat4("projection", projection);

		glBindVertexArray(cubemapVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Set the depth function and culling face to default
		glDepthFunc(GL_LESS);
		glCullFace(GL_BACK);


		// **SECOND PASS**
		//----------------------------------------------------------------------
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		
		// Render scene
		//-------------------------
		screenShader.use();

		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

		glDrawArrays(GL_TRIANGLES, 0, 6);


		// Unbind VAO
		glBindVertexArray(0);


		// glfw: Swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		//-----------------------------------------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// --------------------------------------------------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteVertexArrays(1, &cubemapVAO);
	skyboxShader.clear();
	screenShader.clear();
	fragCoordShader.clear();
	glDeleteFramebuffers(1, &framebuffer);


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

		if (format == GL_RGBA) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

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

// utility function for loading a cubemap textures from file
// ------------------------------------------------------------
unsigned int loadCubemap(std::vector<std::string> faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}