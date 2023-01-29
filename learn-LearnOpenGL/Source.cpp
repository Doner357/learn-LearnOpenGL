/********************************************************************************************************/
/* EXERCISES-2:                                                                                         */
/* --Now create the same 2 triangles using two different VAOs and VBOs for their data                   */
/********************************************************************************************************/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// Screen Width and Height setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Vertex shader and Fragment shader setting
// vertex shader
const char *vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// fragment shader
const char *fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"	FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
"}\0";



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



	/*
	* glad: load all OpenGL function pointers
	* --------------------------------------------------------------------------------------------------------------------
	*/

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}



	/*
	* Build and compile shader program
	* --------------------------------------------------------------------------------------------------------------------
	*/

	// Vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach the shader source and compile the shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for fhader compile errors
	int success;
	char infoLog[512];
	// Get compile status
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		// Get error massage
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAIFED\n" << infoLog << std::endl;
	}

	// Fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach the shader source and compile the shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for fhader compile errors
	// Get compile status
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		// Get error massage
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Link shaders
	// Get ID
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// Attach the shader to the program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Link
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// We can active the shaderProgram by calling glUseProgram(shaderProgram)

	// Don't forget to delete the shader object once we've linked them into the program object
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	/*
	* Set up vertex data (and buffer(s)) and configure vertex attributes
	* --------------------------------------------------------------------------------------------------------------------
	*/

	// first triangle
	float triangle1[] = {
		-0.9f,  -0.5f, 0.0f,   // bottom left
		 0.0f,  -0.5f, 0.0f,   // bottom right
		-0.45f,  0.5f, 0.0f,   // top
	};

	// second triangle
	float triangle2[] = {
		 0.0f,  -0.5f, 0.0f,   // bottom left
		 0.9f,  -0.5f, 0.0f,   // bottom right
		 0.45f,  0.5f, 0.0f,   // top
	};

	// --First Triangle--
	// Create vertex buffer object(VBO) and vertex attribute object(VAO)
	unsigned int VBO1, VAO1;
	glGenBuffers(1, &VBO1);
	glGenVertexArrays(1, &VAO1);

	// Bind VAO
	glBindVertexArray(VAO1);

	// Bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	// Copy the vertex data to into the buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
	// Set vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	// Enable the layout location0
	glEnableVertexAttribArray(0);

	// Bind the Buffer to nothing
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// --Second Triangle--
	// Create VBO and VAO
	unsigned int VBO2, VAO2;
	glGenBuffers(1, &VBO2);
	glGenVertexArrays(1, &VAO2);

	// Bind VAO
	glBindVertexArray(VAO2);
	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);

	// Copy the vertex data to into the buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
	// Set vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	// Enable the layout location0
	glEnableVertexAttribArray(0);

	// Unbind VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	/*
	 * Render loop
	 * --------------------------------------------------------------------------------------------------------------------
	 */

	while (!glfwWindowShouldClose(window)) {

		// Input
		//--------------------------------------------------
		processInput(window);

		// Render command
		//--------------------------------------------------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Active the shader program
		glUseProgram(shaderProgram);
		// Draw the first triangle
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// Draw the second triangle
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// glfw: Swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		//-----------------------------------------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// --------------------------------------------------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteProgram(shaderProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// --------------------------------------------------------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// --------------------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)                     // Press the ESC
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// --------------------------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}