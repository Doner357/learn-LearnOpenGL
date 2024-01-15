#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "learnopengl/shader_m.h"
#include "learnopengl/camera_plus.h"
#include "learnopengl/model.h"
#include "learnopengl/custom_helper.h"

#include <iostream>
#include <string>
#include <cmath>
#include <functional>

// glfw windows call back function
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// Custom function
unsigned int LoadTexture(char const* path, bool gammaCorrection, bool flip_vertically = true);
unsigned int LoadCubemap(std::vector<std::string> faces, bool gammaCorrection, bool flip_vertically = false);
unsigned int CreateColorFramebuffer(unsigned int &frameColortexture, const unsigned int width, const unsigned int height, const bool multisample = false, const unsigned int samples = 1);

// Screen Width and Height setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Gamma value
float gamma = 2.2f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0f;
float lastY = (float)SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;   // Time between current frame and last frame
float lastFrame = 0.0f;   // Time of last frame


bool tangent_to_world_shader = true;


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
	// Tell glfw to create multisample buffer with 4 subsamples
	/*
	const unsigned int multiSamples = 4;
	glfwWindowHint(GLFW_SAMPLES, multiSamples);	
	*/


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

	// Enable multisampling
	/*
	glEnable(GL_MULTISAMPLE);
	*/



	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	// Disable writing to the depth buffer
	/*
	glDepthMask(GL_FALSE);
	*/
	// Configure depth test function
	glDepthFunc(GL_LESS);


	// Enable stencil testing
	/*
	glEnable(GL_STENCIL_TEST);	
	*/
	// Set up stencil writting mask
	/*
	glStencilMask(0xFF); // each bit is written to the stencil buffer as is
	glStencilMask(0x00); // each bit ends up as 0 in the stencil buffer (disabling writes)
	*/
	// Determines whether a fragment passes or is discarded.
	/*
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	*/
	// Set how we can actually update the buffer.
	/*
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	*/


	// Enable blending
	glEnable(GL_BLEND);
	// Set up blending factors
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Set up blending constant color
	/*
	glBlendColor();
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
	

	// Enable OpenGL's built-in sRGB framebuffer support.
	/*
	glEnable(GL_FRAMEBUFFER_SRGB);	
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
	 * Model loading
	 * --------------------------------------------------------------------------------------------------------------------
	 */



	/*
	* Set up vertex data (and buffer(s)) and configure vertex attributes
	* --------------------------------------------------------------------------------------------------------------------
	*/
	CustomHelper::VAOManager vaoManager;
	unsigned int cubemapVAO = vaoManager.getVAO(CustomHelper::VAO_SKYBOX);
	unsigned int quadVAO = vaoManager.getVAO(CustomHelper::VAO_QUAD);
	unsigned int cubeVAO = vaoManager.getVAO(CustomHelper::VAO_CUBE);
	unsigned int roomVAO = vaoManager.getVAO(CustomHelper::VAO_ROOM);


	// positions
	glm::vec3 pos1(-1.0f,  1.0f, 0.0f);
	glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
	glm::vec3 pos3( 1.0f, -1.0f, 0.0f);
	glm::vec3 pos4( 1.0f,  1.0f, 0.0f);
	// texture coordinates
	glm::vec2 uv1(0.0f, 1.0f);
	glm::vec2 uv2(0.0f, 0.0f);
	glm::vec2 uv3(1.0f, 0.0f);
	glm::vec2 uv4(1.0f, 1.0f);
	// normal vector
	glm::vec3 nm(0.0f, 0.0f, 1.0f);


	// First triangel's tangent and bitangent
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	glm::vec3 tangent1(
		f *(deltaUV2.y *edge1.x - deltaUV1.y * edge2.x),
		f *(deltaUV2.y *edge1.y - deltaUV1.y * edge2.y),
		f *(deltaUV2.y *edge1.z - deltaUV1.y * edge2.z)
	);

	glm::vec3 bitangent1(
		f *(-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x),
		f *(-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y),
		f *(-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z)
	);


	// Secod triangel's tangent and bitangent
	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	glm::vec3 tangent2(
		f *(deltaUV2.y *edge1.x - deltaUV1.y * edge2.x),
		f *(deltaUV2.y *edge1.y - deltaUV1.y * edge2.y),
		f *(deltaUV2.y *edge1.z - deltaUV1.y * edge2.z)
	);

	glm::vec3 bitangent2(
		f *(-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x),
		f *(-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y),
		f *(-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z)
	);

	float normQuadVertices[] = {
		// positions               // normal            // texCoords     // tangent                             // bitangent
		pos1.x, pos1.y, pos1.z,    nm.x, nm.y, nm.z,    uv1.x, uv1.y,    tangent1.x, tangent1.y, tangent1.z,    bitangent1.x, bitangent1.y, bitangent1.z,
		pos2.x, pos2.y, pos2.z,    nm.x, nm.y, nm.z,    uv2.x, uv2.y,    tangent1.x, tangent1.y, tangent1.z,    bitangent1.x, bitangent1.y, bitangent1.z,
		pos3.x, pos3.y, pos3.z,    nm.x, nm.y, nm.z,    uv3.x, uv3.y,    tangent1.x, tangent1.y, tangent1.z,    bitangent1.x, bitangent1.y, bitangent1.z,

		pos1.x, pos1.y, pos1.z,    nm.x, nm.y, nm.z,    uv1.x, uv1.y,    tangent2.x, tangent2.y, tangent2.z,    bitangent2.x, bitangent2.y, bitangent2.z,
		pos3.x, pos3.y, pos3.z,    nm.x, nm.y, nm.z,    uv3.x, uv3.y,    tangent2.x, tangent2.y, tangent2.z,    bitangent2.x, bitangent2.y, bitangent2.z,
		pos4.x, pos4.y, pos4.z,    nm.x, nm.y, nm.z,    uv4.x, uv4.y,    tangent2.x, tangent2.y, tangent2.z,    bitangent2.x, bitangent2.y, bitangent2.z,
	};

	unsigned int normQuadVAO, normQuadVBO;
	glGenVertexArrays(1, &normQuadVAO);
	glGenBuffers(1, &normQuadVBO);
	glBindVertexArray(normQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, normQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normQuadVertices), &normQuadVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(11 * sizeof(float)));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &normQuadVBO);
	



	/*
	 * Instance data calculation
	 * --------------------------------------------------------------------------------------------------------------------
	 */



	/*
	 * Texture loading
	 * --------------------------------------------------------------------------------------------------------------------
	 */
	unsigned int brickwall_diff = LoadTexture("textures/brickwall/brickwall.jpg", true);
	unsigned int brickwall_spec = LoadTexture("textures/brickwall/brickwall_spec.jpg", false);
	unsigned int brickwall_norm = LoadTexture("textures/brickwall/brickwall_normal.jpg", false);



	/*
	 * Cubemap loading
	 * --------------------------------------------------------------------------------------------------------------------
	 */

	const std::string folder_path = "cubemaps/skybox/";
	std::vector<std::string> faces = {
		folder_path + "right.png",
		folder_path + "left.png",
		folder_path + "top.png",
		folder_path + "bottom.png",
		folder_path + "front.png",
		folder_path + "back.png"
	};

	unsigned int skyboxTexture = LoadCubemap(faces, false);



	/*
	* Build and compile shader program
	* --------------------------------------------------------------------------------------------------------------------
	*/

	Shader screenShader("shaders/post-processing/vertex/regular_screen.vert", "shaders/post-processing/fragment/regular_screen.frag");
	Shader skyboxShader("shaders/others/vertex/skybox.vert", "shaders/others/fragment/skybox.frag");
	Shader lightCubeShader("shaders/others/vertex/light_cube.vert", "shaders/others/fragment/light_cube.frag");
	Shader dirDepthShader("shaders/bake/depth_map/vertex/dir-depth_map.vert", "shaders/bake/depth_map/fragment/dir-depth_map.frag");
	Shader cubeDepthShader("shaders/bake/depth_map/vertex/cube-depth_map.vert", "shaders/bake/depth_map/fragment/cube-depth_map.frag", "shaders/bake/depth_map/geometry/cube-depth_map.geom");
	// The shader transfer tangent space normal to world space normal
	Shader globalShadowTexShader("shaders/shadow-lighting/vertex/global-shadow-lighting_normal-texture.vert", "shaders/shadow-lighting/fragment/global-shadow-lighting_normal-texture.frag");


	/*
	 * Uniform value setting
	 * --------------------------------------------------------------------------------------------------------------------
	 */

	skyboxShader.use();
	skyboxShader.setInt("cubemap", 0);


	screenShader.use();
	screenShader.setInt("screenTexture", 0);

	globalShadowTexShader.use();
	globalShadowTexShader.setInt("material.diffuse", CustomHelper::SAMPLER_DIFFUSE);
	globalShadowTexShader.setInt("material.specular", CustomHelper::SAMPLER_SPECULAR);
	globalShadowTexShader.setInt("material.normal", CustomHelper::SAMPLER_NORMAL);
	globalShadowTexShader.setFloat("material.shininess", 256.0f);



	glUseProgram(0);

	

	/*
	 * Uniform Block Object setting
	 * --------------------------------------------------------------------------------------------------------------------
	 */

	// Camera matrices uniform block
	CustomHelper::CameraMatricesManager cameraMatManager(CustomHelper::UBOPOINT_NAME_CAMERA_MATRICES, CustomHelper::UBOPOINT_CAMERA_MATRICES);
	cameraMatManager.registerShader(skyboxShader);
	cameraMatManager.registerShader(lightCubeShader);
	cameraMatManager.registerShader(globalShadowTexShader);

	// Global light uniform block
	CustomHelper::GlobalBlinnPongLightManager globalLightManager(CustomHelper::UBOPOINT_NAME_BLINPHONG_LIGHTING, CustomHelper::UBOPOINT_BLINPHONG_LIGHTING, CustomHelper::MAX_NUM_DIRECTIONALLIGHT, CustomHelper::MAX_NUM_POINTLIGHT, CustomHelper::MAX_NUM_SPOTLIGHT);
	//globalLightManager.registerShader(TexShader);
	//globalLightManager.registerShader(repeatTexShader);
	globalLightManager.registerShader(globalShadowTexShader);

	// Global light with shadow uniform block
	CustomHelper::GlobalBlinnPongShadowLightManager globalShadowLightManager(
		CustomHelper::UBOPOINT_NAME_BLINPHONG_SHADOWLIGHTING,
		CustomHelper::UBOPOINT_NAME_BLINPHONG_SHADOWMATRICES,
		CustomHelper::UBOPOINT_NAME_BLINPHONG_SHADOWFARPLANE,
		CustomHelper::UBOPOINT_BLINPHONG_SHADOWLIGHTING,
		CustomHelper::UBOPOINT_BLINPHONG_SHADOWMATRICES,
		CustomHelper::UBOPOINT_BLINPHONG_SHADOWFARPLANE,
		CustomHelper::MAX_NUM_SHADOWDIRECTIONALLIGHT,
		CustomHelper::MAX_NUM_SHADOWPOINTLIGHT,
		CustomHelper::MAX_NUM_SHADOWSPOTLIGHT,
		2048,
		1024,
		1024
	);
	globalShadowLightManager.registerShader(globalShadowTexShader);

	// Gamma Correction uniform block
	CustomHelper::GammaManager gammaManager(CustomHelper::UBOPOINT_NAME_GAMMA_CORRECTION, CustomHelper::UBOPOINT_GAMMA_CORRECTION);
	gammaManager.registerShader(lightCubeShader);
	gammaManager.registerShader(globalShadowTexShader);



	/*
	* Frambuffers creating
	* --------------------------------------------------------------------------------------------------------------------
	*/

	unsigned int ms_Frametexture;
	unsigned int screentexuture;
	unsigned int ms_Framebuffer = CreateColorFramebuffer(ms_Frametexture, SCR_WIDTH, SCR_HEIGHT, true, 4);
	unsigned int screenFramebuffer = CreateColorFramebuffer(screentexuture, SCR_WIDTH, SCR_HEIGHT);



	/*
	 * Others data calculation
	 * --------------------------------------------------------------------------------------------------------------------
	 */



	/*
	 * Light setting
	 * --------------------------------------------------------------------------------------------------------------------
	 */

	// Setting global light data
	CustomHelper::BlinnPhongLight_direct dirLight;
	CustomHelper::BlinnPhongLight_point pointLight;
	CustomHelper::BlinnPhongLight_spot spotLight;
	// Number of each type light
	unsigned int num_of_dirLight = 0;
	unsigned int num_of_pointLight = 0;
	unsigned int num_of_spotLight = 0;

	// Manual setting part



	// Random generate part
	unsigned int num_of_random_dirLight = 0;
	unsigned int num_of_random_pointLight = 0;
	unsigned int num_of_random_spotLight = 0;
	// Appear area(x, z) = (-area, -area) ~ (area, area)
	float appear_area = 30.0f;
	// Appear height(y) = (0) ~ (height)
	float appear_hieght = 4.0f;
	for (unsigned int i = 0; i < num_of_random_dirLight; i++) {
		dirLight = CustomHelper::GenerateRandomGlobalBlinnPhongLight_dirLight(glm::vec3(-1.0f, -0.2, 0.1f), glm::vec3(-1.0f, -0.2, 0.1f), glm::vec3(0.5f), glm::vec3(0.5f));
		globalLightManager.updateDirLight(dirLight, num_of_dirLight++);
	}
	for (unsigned int i = 0; i < num_of_random_pointLight; i++) {
		pointLight = CustomHelper::GenerateRandomGlobalBlinnPhongLight_pointLight(glm::vec3(-appear_area, 1.0f, -appear_area), glm::vec3(appear_area, appear_hieght, appear_area));
		globalLightManager.updatePointLight(pointLight, num_of_pointLight++);
	}
	for (unsigned int i = 0; i < num_of_random_spotLight; i++) {
		spotLight = CustomHelper::GenerateRandomGlobalBlinnPhongLight_spotLight(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(-appear_area, 5.0f, -appear_area), glm::vec3(appear_area, 5.0f, appear_area), 12.5f, 90.0f);
		globalLightManager.updateSpotLight(spotLight, num_of_spotLight++);
	}



	/*
	 * Local Light setting
	 * --------------------------------------------------------------------------------------------------------------------
	 */

	glUseProgram(0);



	/*
	 * Gamma setting
	 * --------------------------------------------------------------------------------------------------------------------
	 */

	gammaManager.updateGamma(gamma);



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

	// FPS time record
	double fps_previous_time = glfwGetTime(), fps_current_time, fps_delta_time = 0;
	// Record how many frames has passed
	unsigned int fps_passframe_count = 0;

	while (!glfwWindowShouldClose(window)) {

		// FPS presentation
		//--------------------------------------------------
		// Refresh fps recorder
		fps_current_time = glfwGetTime();
		fps_delta_time = fps_current_time - fps_previous_time;
		fps_passframe_count++;
		// Update the fps presentation
		if (fps_delta_time >= 1.0) {
			glfwSetWindowTitle(window, ("LearnOpenGL FPS:" + std::to_string(fps_passframe_count)).c_str());
			fps_previous_time = fps_current_time;
			fps_passframe_count = 0;
		}
		
		// Calculate delta time
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// Input
		//--------------------------------------------------
		processInput(window);


		// **FIRST PASS**
		//----------------------------------------------------------------------

		// Create transformations
		//-------------------------
		// --model matrix--
		// Since each cube has its own position, we declare the matrix variable here
		glm::mat4 model;
		// --view matrix--
		glm::mat4 view = camera.GetViewMatrix();
		// --projection matrix--
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		// --normal matrix--
		glm::mat3 normalMat(1.0f);


		// Fill the uniform buffer
		//-------------------------
		// Camera view
		cameraMatManager.updateView(view);

		// Camera projection
		cameraMatManager.updateProjection(projection);


		// Render scene
		//---------------------------------------------

		glEnable(GL_DEPTH_TEST);

		// Render Shadow
		//--------------------------

		std::function<void(Shader &)> shadowDrawFunction;
		shadowDrawFunction = [=](Shader &shader) {};
		globalShadowLightManager.bindShadowMaps();


		// Render Objects
		//--------------------------
		
		// Rescale the view port to the size of the screen
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		// Bind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, ms_Framebuffer);

		// Render command
		//---------------
		// Clear Buffer
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glBindVertexArray(0);
		glUseProgram(0);


		// skybox
		//---------------
		// Since the default value in depth buffer is 1.0, so the fragment should pass the depth test when the depth of fragment is less or equal to
		// the value store in the depth buffer. This can avoid the depth fighting.
		glDepthFunc(GL_LEQUAL);

		// Activate the shader
		skyboxShader.use();

		// Bind cubemap
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

		glBindVertexArray(cubemapVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Set the depth function and culling face to default
		glDepthFunc(GL_LESS);
		glCullFace(GL_BACK);		
		


		// **SECOND PASS**
		//----------------------------------------------------------------------
		
		glBindFramebuffer(GL_READ_FRAMEBUFFER, ms_Framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, screenFramebuffer);
		glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);		


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		
		// Render scene
		//-------------------------
		
		screenShader.use();

		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screentexuture);

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
	vaoManager.clean();
	screenShader.clear();
	skyboxShader.clear();
	dirDepthShader.clear();
	cubeDepthShader.clear();
	glDeleteFramebuffers(1, &ms_Framebuffer);
	glDeleteFramebuffers(1, &screenFramebuffer);


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
unsigned int LoadTexture(char const *path, bool gammaCorrection, bool flip_vertically) {
	// Set whether filp vertical axis
	stbi_set_flip_vertically_on_load(flip_vertically);

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data) {
		GLenum internalFormat;
		GLenum dataFormat;
		if (nrComponents == 1) {
			internalFormat = dataFormat = GL_RED;
		}
		else if (nrComponents == 3) {
			internalFormat = (gammaCorrection ? GL_SRGB : GL_RGB);
			dataFormat = GL_RGB;
		}
		else if (nrComponents == 4) {
			internalFormat = (gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA);
			dataFormat = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		if (dataFormat == GL_RGBA) {
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
	// Set whether flip vertically to false
	stbi_set_flip_vertically_on_load(false);

	return textureID;
}

// utility function for loading a cubemap textures from file
// ------------------------------------------------------------
unsigned int LoadCubemap(std::vector<std::string> faces, bool gammaCorrection, bool flip_vertically) {
	// Set whether filp vertical axis
	stbi_set_flip_vertically_on_load(flip_vertically);

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			GLenum internalFormat;
			GLenum dataFormat;
			if (nrChannels == 1) {
				internalFormat = dataFormat = GL_RED;
			}
			else if (nrChannels == 3) {
				internalFormat = (gammaCorrection ? GL_SRGB : GL_RGB);
				dataFormat = GL_RGB;
			}
			else if (nrChannels == 4) {
				internalFormat = (gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA);
				dataFormat = GL_RGBA;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
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

	// Set whether flip vertically to false
	stbi_set_flip_vertically_on_load(false);

	return textureID;
}

// Generate a framebuffer attach the given texture as the color attachment.
unsigned int CreateColorFramebuffer(unsigned int &frameColortexture, const unsigned int width, const unsigned int height, const bool multisample, const unsigned int samples) {

	unsigned int framebuffer;
	// Generate a framebuffer and get its ID
	glGenFramebuffers(1, &framebuffer);
	// Bind framebuffers
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);


	// Attach texture to framebuffer
	// Create a texture to store the scene's image
	glGenTextures(1, &frameColortexture);

	// Determine whether use the multisampling texture
	GLenum texformat = GL_TEXTURE_2D;
	if (multisample)
		texformat = GL_TEXTURE_2D_MULTISAMPLE;
	glBindTexture(texformat, frameColortexture);

	if (multisample)
		glTexImage2DMultisample(texformat, samples, GL_RGB, width, height, GL_TRUE);
	else {
		glTexImage2D(texformat, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(texformat, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(texformat, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glBindTexture(texformat, 0);

	// Attach the texture to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texformat, frameColortexture, 0);


	// Attach Render Buffer (RBO) to framebuffer
	unsigned int RBO;
	// Generate renderbuffer and get its ID
	glGenRenderbuffers(1, &RBO);
	// Bind render buffer
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	// Creating a depth and stencil renderbuffer object
	// Determine whether use the multisampling render buffer
	if (multisample)
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
	else
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


	return framebuffer;
}