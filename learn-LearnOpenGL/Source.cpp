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



	/*
	 * Instance data calculation
	 * --------------------------------------------------------------------------------------------------------------------
	 */



	/*
	 * Texture loading
	 * --------------------------------------------------------------------------------------------------------------------
	 */
	unsigned int wood_diff = LoadTexture("textures/wood.jpg", true);
	unsigned int common_spec = LoadTexture("textures/common_spec.jpg", false);

	unsigned int container_diff = LoadTexture("textures/container2/container2.png", true);
	unsigned int container_spec = LoadTexture("textures/container2/container2_specular.png", false);



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
	Shader viewDepthShader_ortho("shaders/post-processing/vertex/regular_screen.vert", "shaders/post-processing/fragment/ortho_depth-map.frag");
	Shader viewDepthShader_pers("shaders/post-processing/vertex/regular_screen.vert", "shaders/post-processing/fragment/perspect_depth-map.frag");
	Shader skyboxShader("shaders/others/vertex/skybox.vert", "shaders/others/fragment/skybox.frag");
	Shader lightCubeShader("shaders/others/vertex/light_cube.vert", "shaders/others/fragment/light_cube.frag");
	Shader TexShader("shaders/shadow-lighting/vertex/lighting_point-shadow_texture.vert", "shaders/shadow-lighting/fragment/local-lighting_point-shadow_texture.frag");
	Shader repeatTexShader("shaders/shadow-lighting/vertex/lighting_point-shadow_repeated-texture.vert", "shaders/shadow-lighting/fragment/local-lighting_point-shadow_texture.frag");
	Shader modelShader("shaders/lighting/vertex/lighting_model.vert", "shaders/lighting/fragment/global-lighting_model.frag");
	Shader simpleDepthShader("shaders/bake/depth_map/vertex/dir-depth_map.vert", "shaders/bake/depth_map/fragment/dir-depth_map.frag");
	Shader cubeDepthShader("shaders/bake/depth_map/vertex/cube-depth_map.vert", "shaders/bake/depth_map/fragment/cube-depth_map.frag", "shaders/bake/depth_map/geometry/cube-depth_map.geom");
	Shader globalShadowTexShader("shaders/shadow-lighting/vertex/global-shadow-lighting_texture.vert", "shaders/shadow-lighting/fragment/global-shadow-lighting_texture.frag");



	/*
	 * Uniform value setting
	 * --------------------------------------------------------------------------------------------------------------------
	 */

	skyboxShader.use();
	skyboxShader.setInt("cubemap", 0);


	screenShader.use();
	screenShader.setInt("screenTexture", 0);


	viewDepthShader_ortho.use();
	viewDepthShader_ortho.setInt("screenTexture", 0);


	viewDepthShader_pers.use();
	viewDepthShader_pers.setInt("screenTexture", 0);


	repeatTexShader.use();
	repeatTexShader.setInt("material.diffuse", 0);
	repeatTexShader.setInt("material.specular", 1);
	repeatTexShader.setInt("pointLight_sh.shadowMap", 2);
	repeatTexShader.setFloat("material.shininess", 1024);


	TexShader.use();
	TexShader.setInt("material.diffuse", 0);
	TexShader.setInt("material.specular", 1);
	TexShader.setInt("pointLight_sh.shadowMap", 2);
	TexShader.setFloat("material.shininess", 2048.0f);


	globalShadowTexShader.use();
	globalShadowTexShader.setInt("material.diffuse", 0);
	globalShadowTexShader.setInt("material.specular", 1);
	globalShadowTexShader.setFloat("material.shininess", 2048.0f);


	glUseProgram(0);

	

	/*
	 * Uniform Block Object setting
	 * --------------------------------------------------------------------------------------------------------------------
	 */

	// Camera matrices uniform block
	CustomHelper::CameraMatricesManager cameraMatManager(CustomHelper::UBOPOINT_NAME_CAMERA_MATRICES, CustomHelper::UBOPOINT_CAMERA_MATRICES);
	cameraMatManager.registerShader(skyboxShader);
	cameraMatManager.registerShader(lightCubeShader);
	cameraMatManager.registerShader(TexShader);
	cameraMatManager.registerShader(repeatTexShader);
	cameraMatManager.registerShader(modelShader);
	cameraMatManager.registerShader(globalShadowTexShader);

	// Global light uniform block
	CustomHelper::GlobalBlinnPongLightManager globalLightManager(CustomHelper::UBOPOINT_NAME_BLINPHONG_LIGHTING, CustomHelper::UBOPOINT_BLINPHONG_LIGHTING, CustomHelper::MAX_NUM_DIRECTIONALLIGHT, CustomHelper::MAX_NUM_POINTLIGHT, CustomHelper::MAX_NUM_SPOTLIGHT);
	//globalLightManager.registerShader(TexShader);
	//globalLightManager.registerShader(repeatTexShader);
	globalLightManager.registerShader(modelShader);
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
	gammaManager.registerShader(TexShader);
	gammaManager.registerShader(repeatTexShader);
	gammaManager.registerShader(modelShader);
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
	 * Depth map creating
	 * --------------------------------------------------------------------------------------------------------------------
	 */



	/*
	 * Others data calculation
	 * --------------------------------------------------------------------------------------------------------------------
	 */
	std::vector<glm::vec3> containerPos = {
		glm::vec3(0.0f, -2.0f, -2.0f),
		glm::vec3(1.0f, 1.0f, 3.0f),
		glm::vec3(0.0f, 3.0f, -4.0f),
		glm::vec3(-4.0f, -1.5f, 0.0f),
		glm::vec3(3.0f, 4.0f, 0.0f),
		glm::vec3(-4.0f, 4.0f, 4.0f),
		glm::vec3( 2.0f, 0.0f, 0.0f)
	};



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
		globalLightManager.editDirLight(dirLight, num_of_dirLight++);
	}
	for (unsigned int i = 0; i < num_of_random_pointLight; i++) {
		pointLight = CustomHelper::GenerateRandomGlobalBlinnPhongLight_pointLight(glm::vec3(-appear_area, 1.0f, -appear_area), glm::vec3(appear_area, appear_hieght, appear_area));
		globalLightManager.editPointLight(pointLight, num_of_pointLight++);
	}
	for (unsigned int i = 0; i < num_of_random_spotLight; i++) {
		spotLight = CustomHelper::GenerateRandomGlobalBlinnPhongLight_spotLight(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(-appear_area, 5.0f, -appear_area), glm::vec3(appear_area, 5.0f, appear_area), 12.5f, 90.0f);
		globalLightManager.editSpotLight(spotLight, num_of_spotLight++);
	}


	CustomHelper::BlinnPhongLight_direct shadowDirLight = {
		glm::vec3(0.1f, -1.0f, 0.3f),
		glm::vec3(0.009f),
		glm::vec3(0.02f),
		glm::vec3(0.02f)
	};

	CustomHelper::BlinnPhongLight_point shadowPointLight1 = {
		glm::vec3(0.0f, 0.5f, 0.0f),
		0.0f,
		0.0f,
		1.0f,
		glm::vec3(0.2f) * 0.2f,
		glm::vec3(0.8f) * 0.5f,
		glm::vec3(1.0f) * 0.5f
	};
	CustomHelper::BlinnPhongLight_point shadowPointLight2 = {
		glm::vec3(0.0f, 0.5f, 0.0f),
		0.0f,
		0.0f,
		1.0f,
		glm::vec3(0.0f, 0.2f, 0.2f) * 0.2f,
		glm::vec3(0.0f, 1.0f, 1.0f) * 0.5f,
		glm::vec3(0.0f, 1.0f, 1.0f) * 0.5f
	};
	CustomHelper::BlinnPhongLight_point shadowPointLight3 = {
		glm::vec3(0.0f, 0.5f, 0.0f),
		0.0f,
		0.0f,
		1.0f,
		glm::vec3(0.2f, 0.0f, 0.2f) * 0.2f,
		glm::vec3(1.0f, 0.0f, 1.0f) * 0.5f,
		glm::vec3(1.0f, 0.0f, 1.0f) * 0.5f
	};

	CustomHelper::BlinnPhongLight_point shadowPointLight4 = {
		glm::vec3(0.0f, 0.5f, 0.0f),
		0.0f,
		0.0f,
		1.0f,
		glm::vec3(0.2f, 0.2f, 0.0f) * 0.2f,
		glm::vec3(1.0f, 1.0f, 0.0f) * 0.5f,
		glm::vec3(1.0f, 1.0f, 0.0f) * 0.5f
	};

	CustomHelper::BlinnPhongLight_spot shadowSpotLight1 = {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-4.0f, -0.3f, 0.0f),
		glm::cos(glm::radians(45.5f)),
		glm::cos(glm::radians(48.5f)),
		0.0f,
		0.0f,
		1.0f,
		glm::vec3(0.2f, 0.0f, 0.0f) * 0.2f,
		glm::vec3(1.0f, 0.0f, 0.0f) * 0.5f,
		glm::vec3(1.0f, 0.0f, 0.0f) * 0.5f
	};

	CustomHelper::BlinnPhongLight_spot shadowSpotLight2 = {
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-4.0f, -0.3f, 0.0f),
		glm::cos(glm::radians(45.5f)),
		glm::cos(glm::radians(48.5f)),
		0.0f,
		0.0f,
		1.0f,
		glm::vec3(0.2f, 0.1f, 0.2f) * 0.2f,
		glm::vec3(1.0f, 0.5f, 1.0f) * 0.5f,
		glm::vec3(1.0f, 0.5f, 1.0f) * 0.5f
	};


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
		shadowDrawFunction = [=](Shader &shader) {
				// Draw Boxes
				// Avoid peter panning by cull the front faces
				glCullFace(GL_FRONT);
				for (unsigned int i = 0; i < containerPos.size(); i++) {
					glm::mat4 model(1.0f);
					model = glm::translate(model, containerPos[i]);
					model = glm::rotate(model, static_cast<float>(i), glm::vec3(1.0f, 2.0f, 0.5f));
					model = glm::scale(model, glm::vec3(0.5f));
					shader.setMat4("model", model);

					glBindVertexArray(cubeVAO);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				// Set culled faces as back faces
				glCullFace(GL_BACK);

				// Draw room
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(5.0f));
				shader.setMat4("model", model);
				glBindVertexArray(roomVAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);

				glBindVertexArray(0);
			}
		};
		globalShadowLightManager.updateDirLight(shadowDirLight, 0, 10.0f, 20.0f, 7.0f, shadowDrawFunction);
		shadowPointLight1.position = glm::vec3(0.0f, 4.5f * glm::sin(static_cast<float>(glfwGetTime())), 0.0f);
		globalShadowLightManager.updatePointLight(shadowPointLight1, 0, 25.0f, shadowDrawFunction);
		shadowPointLight2.position = glm::vec3(0.0f, 0.01f, 4.5f * glm::sin(static_cast<float>(glfwGetTime())));
		globalShadowLightManager.updatePointLight(shadowPointLight2, 1, 25.0f, shadowDrawFunction);
		shadowPointLight4.position = glm::vec3(4.5f * glm::cos(static_cast<float>(glfwGetTime())), 0.01f, 4.5f * glm::sin(static_cast<float>(glfwGetTime())));
		globalShadowLightManager.updatePointLight(shadowPointLight4, 3, 25.0f, shadowDrawFunction);
		shadowPointLight3.position = glm::vec3(4.5f * glm::sin(static_cast<float>(glfwGetTime())), 0.01f, 0.0f);
		globalShadowLightManager.updatePointLight(shadowPointLight3, 2, 25.0f, shadowDrawFunction);
		shadowSpotLight1.position = glm::vec3(4.0f * glm::sin(glfwGetTime()), 0.01f, 0.0f);
		// The fov of spot light should double the angle of outerCutOff
		globalShadowLightManager.updateSpotLight(shadowSpotLight1, 0, 97.0f, 10.0f, shadowDrawFunction);
		shadowSpotLight2.position = glm::vec3(0.0f, 0.01f, 4.0f * glm::sin(glfwGetTime()));
		globalShadowLightManager.updateSpotLight(shadowSpotLight2, 1, 97.0f, 10.0f, shadowDrawFunction);
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


		// Draw boxes
		globalShadowTexShader.use();

		// Light space transform matrix
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, container_diff);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, container_spec);

		for (unsigned int i = 0; i < containerPos.size(); i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, containerPos[i]);
			model = glm::rotate(model, static_cast<float>(i), glm::vec3(1.0f, 2.0f, 0.5f));
			model = glm::scale(model, glm::vec3(0.5f));
			normalMat = CustomHelper::CalculateNormalMat(model);
			globalShadowTexShader.setMat4("model", model);
			globalShadowTexShader.setMat3("normalMat", normalMat);
			globalShadowTexShader.setVec3("viewPos", camera.Position);

			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Draw room
		globalShadowTexShader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wood_diff);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, common_spec);

		model = glm::mat4(1.0f);
		normalMat = glm::mat3(1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		normalMat = CustomHelper::CalculateNormalMat(model);
		globalShadowTexShader.setMat4("model", model);
		globalShadowTexShader.setMat3("normalMat", normalMat);
		globalShadowTexShader.setVec3("viewPos", camera.Position);

		glBindVertexArray(roomVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Draw light cube
		lightCubeShader.use();
		glBindVertexArray(cubeVAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, shadowSpotLight1.position);
		model = glm::scale(model, glm::vec3(0.05f));
		lightCubeShader.setMat4("model", model);
		lightCubeShader.setVec3("lightColor", shadowSpotLight1.specular * 1.2f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(cubeVAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, shadowSpotLight2.position);
		model = glm::scale(model, glm::vec3(0.05f));
		lightCubeShader.setMat4("model", model);
		lightCubeShader.setVec3("lightColor", shadowSpotLight2.specular * 1.2f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(cubeVAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, shadowPointLight1.position);
		model = glm::scale(model, glm::vec3(0.05f));
		lightCubeShader.setMat4("model", model);
		lightCubeShader.setVec3("lightColor", shadowPointLight1.specular * 1.2f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(cubeVAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, shadowPointLight2.position);
		model = glm::scale(model, glm::vec3(0.05f));
		lightCubeShader.setMat4("model", model);
		lightCubeShader.setVec3("lightColor", shadowPointLight2.specular * 1.2f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(cubeVAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, shadowPointLight3.position);
		model = glm::scale(model, glm::vec3(0.05f));
		lightCubeShader.setMat4("model", model);
		lightCubeShader.setVec3("lightColor", shadowPointLight3.specular * 1.2f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(cubeVAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, shadowPointLight4.position);
		model = glm::scale(model, glm::vec3(0.05f));
		lightCubeShader.setMat4("model", model);
		lightCubeShader.setVec3("lightColor", shadowPointLight4.specular * 1.2f);
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glBindVertexArray(0);
		glUseProgram(0);

		// Draw light cube
		/*
		CustomHelper::DrawGlobalPointLightCube(globalLightManager, cubeVAO, lightCubeShader, 0.05f);
		*/
		// Draw floor
		/*
		CustomHelper::DrawTexFloor(quadVAO, repeatTexShader, camera.Position, wood_diff, common_spec, 1024.0f, glm::vec3(0.0f, -1.0f, 0.0f), 30.0f, 10);
		*/

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
		/*
		viewDepthShader_pers.use();
		viewDepthShader_pers.setFloat("near_plane", 0.1f);
		viewDepthShader_pers.setFloat("far_plane", 10.0f);
		*/
			


		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screentexuture);
		//glBindTexture(GL_TEXTURE_2D, globalShadowLightManager.getSpotDepthMap(0));

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
	TexShader.clear();
	repeatTexShader.clear();
	modelShader.clear();
	simpleDepthShader.clear();
	viewDepthShader_ortho.clear();
	viewDepthShader_pers.clear();
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