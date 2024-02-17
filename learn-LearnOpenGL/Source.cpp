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
#include <vector>
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
unsigned int CreateColorFramebuffer(const size_t numOfColorAttachment, unsigned int *frameColortextures, const unsigned int width, const unsigned int height, const bool multisample, const unsigned int samples, const bool hdr);

// Screen Width and Height setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Gamma value
// This time we do gamma correction in screen post-processing shader
float gamma = 2.2f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0f;
float lastY = (float)SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;    // Time between current frame and last frame
float lastFrame = 0.0f;    // Time of last frame

// Control vertical sync
bool needVerticalSync = true;
bool vSyncKeyPressed  = false;

// Tone mapping options
bool  isEyeAdaptionEnable = true;   // Determine whether do the eye adaption for tone mapping
bool  eyeAdaptionKeyPressed = false;
const float kMaxLuminance = 0.7f;
const float kMinLuminance = 0.3f;
const float kExposureAdjustSpeed = 50.0f;
const float kAverageLuminance = 0.5f;
float current_luminance = 0.3f;
float last_luminance = 0.3f;
float exposure = 1.0f;              // Control the exposure value


// Determind whether activate bloom effect
bool applyBloom = true;
// Used to control bloom
bool bloomKeyPressed = false;


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
	Model backpack("models/backpack/backpack.obj", false, false);



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

	unsigned int tangentQuadVAO, tangentQuadVBO;
	glGenVertexArrays(1, &tangentQuadVAO);
	glGenBuffers(1, &tangentQuadVBO);
	glBindVertexArray(tangentQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, tangentQuadVBO);
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
	glDeleteBuffers(1, &tangentQuadVBO);
	



	/*
	 * Instance data calculation
	 * --------------------------------------------------------------------------------------------------------------------
	 */



	/*
	 * Texture loading
	 * --------------------------------------------------------------------------------------------------------------------
	 */



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

	unsigned int skyboxTexture = LoadCubemap(faces, true);



	/*
	* Build and compile shader program
	* --------------------------------------------------------------------------------------------------------------------
	*/

	Shader screenShader("shaders/post-processing/vertex/regular_screen.vert", "shaders/post-processing/fragment/regular_screen.frag");
	Shader skyboxShader("shaders/others/vertex/skybox.vert", "shaders/others/fragment/skybox.frag");
	Shader lightCubeShader("shaders/others/vertex/light_cube.vert", "shaders/others/fragment/light_cube.frag");
	Shader dirDepthShader("shaders/bake/depth_map/vertex/dir-depth_map.vert", "shaders/bake/depth_map/fragment/dir-depth_map.frag");
	Shader cubeDepthShader("shaders/bake/depth_map/vertex/cube-depth_map.vert", "shaders/bake/depth_map/fragment/cube-depth_map.frag", "shaders/bake/depth_map/geometry/cube-depth_map.geom");

	// HDR screen shader using Reinhard tone mapping
	Shader hdr_reinhard_screenShader("shaders/post-processing/vertex/regular_screen.vert", "shaders/post-processing/fragment/hdr_reinhard_screen.frag");
	// HDR screen shader using Exposure tone mapping
	Shader hdr_exposure_screenShader("shaders/post-processing/vertex/regular_screen.vert", "shaders/post-processing/fragment/hdr_exposure_screen.frag");
	// Shader just for render texture on simple shape
	Shader textureShader("shaders/lighting/vertex/lighting_texture.vert", "shaders/lighting/fragment/global-lighting_texture.frag");

	// Shaders for deferred shading
	Shader geometryShader("shaders/deferred_shading/g-buffer/vertex/model.vert", "shaders/deferred_shading/g-buffer/fragment/normal_model_geometry.frag");


	// Shader for bloom
	Shader hdr_bloom_screenShader("shaders/post-processing/vertex/regular_screen.vert", "shaders/post-processing/fragment/hdr_bloom_extract.frag");
	Shader bloom_gaussian_blur_screenShader("shaders/post-processing/vertex/regular_screen.vert", "shaders/post-processing/fragment/bloom_gaussian_blur.frag");
	Shader hdr_bloom_blending_screenShader("shaders/post-processing/vertex/regular_screen.vert", "shaders/post-processing/fragment/hdr_bloom_blending.frag");

	/*
	 * Uniform value setting
	 * --------------------------------------------------------------------------------------------------------------------
	 */

	skyboxShader.use();
	skyboxShader.setInt("cubemap", 0);


	screenShader.use();
	screenShader.setInt("screenTexture", 0);


	hdr_reinhard_screenShader.use();
	hdr_reinhard_screenShader.setInt("screenTexture", 0);


	hdr_exposure_screenShader.use();
	hdr_exposure_screenShader.setInt("screenTexture", 0);


	hdr_bloom_screenShader.use();
	hdr_bloom_screenShader.setInt("screenTexture", 0);

	// Bloom
	bloom_gaussian_blur_screenShader.use();
	bloom_gaussian_blur_screenShader.setInt("image", 0);
	
	hdr_bloom_blending_screenShader.use();
	hdr_bloom_blending_screenShader.setInt("scene", 0);
	hdr_bloom_blending_screenShader.setInt("bloom", 1);
	


	textureShader.use();
	textureShader.setInt("material.diffuse", CustomHelper::SAMPLER_DIFFUSE);
	textureShader.setInt("material.specular", CustomHelper::SAMPLER_SPECULAR);
	textureShader.setFloat("material.shininess", 64.0f);


	glUseProgram(0);

	

	/*
	 * Uniform Block Object setting
	 * --------------------------------------------------------------------------------------------------------------------
	 */

	// Camera matrices uniform block
	CustomHelper::CameraMatricesManager cameraMatManager(CustomHelper::UBOPOINT_NAME_CAMERA_MATRICES, CustomHelper::UBOPOINT_CAMERA_MATRICES);
	cameraMatManager.registerShader(skyboxShader);
	cameraMatManager.registerShader(lightCubeShader);
	cameraMatManager.registerShader(textureShader);
	cameraMatManager.registerShader(geometryShader);

	// Global light uniform block
	CustomHelper::GlobalBlinnPongLightManager globalLightManager(CustomHelper::UBOPOINT_NAME_BLINPHONG_LIGHTING, CustomHelper::UBOPOINT_BLINPHONG_LIGHTING, CustomHelper::MAX_NUM_DIRECTIONALLIGHT, CustomHelper::MAX_NUM_POINTLIGHT, CustomHelper::MAX_NUM_SPOTLIGHT);
	globalLightManager.registerShader(textureShader);

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

	// Gamma Correction uniform block
	CustomHelper::GammaManager gammaManager(CustomHelper::UBOPOINT_NAME_GAMMA_CORRECTION, CustomHelper::UBOPOINT_GAMMA_CORRECTION);
	// Move gamma correction to post-processing part
	gammaManager.registerShader(hdr_reinhard_screenShader);
	gammaManager.registerShader(hdr_exposure_screenShader);



	/*
	* Frambuffers creating
	* --------------------------------------------------------------------------------------------------------------------
	*/
	// Multisample framebuffer for render first scene
	// Disable MSAA since we're using deferred shading
	/*
	unsigned int hdr_ms_render_screen_texture;
	unsigned int hdr_ms_render_screen_framebuffer = CreateColorFramebuffer(1, &hdr_ms_render_screen_texture, SCR_WIDTH, SCR_HEIGHT, true, 4, true);
	*/

	// Initial framebuffer for post-processing
	unsigned int hdr_initial_screen_texture;
	unsigned int hdr_initial_screen_framebuffer   = CreateColorFramebuffer(1, &hdr_initial_screen_texture, SCR_WIDTH, SCR_HEIGHT, false, 0, true);

	// Framebuffer to store result image
	unsigned int ldr_final_screen_texture;
	unsigned int ldr_final_screen_framebuffer     = CreateColorFramebuffer(1, &ldr_final_screen_texture, SCR_WIDTH, SCR_HEIGHT, false, 0, true);

	// Framebuffer for tone mapping
	unsigned int hdr_tone_mapping_screen_texture;
	unsigned int hdr_tone_mapping_screen_framebuffer = CreateColorFramebuffer(1, &hdr_tone_mapping_screen_texture, SCR_WIDTH, SCR_HEIGHT, false, 0, true);

	// Framebuffer for storing image after post-processing 
	unsigned int hdr_process_screen_texture;
	unsigned int hdr_process_screen_framebuffer = CreateColorFramebuffer(1, &hdr_process_screen_texture, SCR_WIDTH, SCR_HEIGHT, false, 0, true);

	// Framebuffer for bloom
	unsigned int hdr_bloom_screen_textures[2];
	unsigned int hdr_bloom_screen_framebuffer     = CreateColorFramebuffer(2, hdr_bloom_screen_textures, SCR_WIDTH, SCR_HEIGHT, false, 0, true);

	// Framebuffer for gaussian blur
	unsigned int pingpong_textures[2];
	unsigned int pingpong_framebuffer[2];
	for (int i = 0; i < 2; i++) {
		pingpong_framebuffer[i] = CreateColorFramebuffer(1, &pingpong_textures[i], SCR_WIDTH, SCR_HEIGHT, false, 0, true);
	}


	// --Deferred shading
	// G-buffer creation
	unsigned int g_buffer;
	glGenFramebuffers(1, &g_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
	unsigned int g_position, g_normal, g_color_spec;

	// - position color buffer
	glGenTextures(1, &g_position);
	glBindTexture(GL_TEXTURE_2D, g_position);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_position, 0);

	// - normal color buffer
	glGenTextures(1, &g_normal);
	glBindTexture(GL_TEXTURE_2D, g_normal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_normal, 0);

	// - color + specular color buffer
	glGenTextures(1, &g_color_spec);
	glBindTexture(GL_TEXTURE_2D, g_color_spec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_color_spec, 0);

	// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
	unsigned int g_attachments[3] = {GL_COLOR_ATTACHMENT0 ,GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, g_attachments);

	// Create and attach render buffer (renderbuffer)
	unsigned g_renderbuffer;
	glGenRenderbuffers(1, &g_renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, g_renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, g_renderbuffer);

	// Check whether the framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	/*
	 * Others data calculation
	 * --------------------------------------------------------------------------------------------------------------------
	 */
	// Positions of container
	std::vector<glm::vec3> objectPositions;
	objectPositions.push_back(glm::vec3(-3.0, -0.5, -3.0));
	objectPositions.push_back(glm::vec3(0.0, -0.5, -3.0));
	objectPositions.push_back(glm::vec3(3.0, -0.5, -3.0));
	objectPositions.push_back(glm::vec3(-3.0, -0.5, 0.0));
	objectPositions.push_back(glm::vec3(0.0, -0.5, 0.0));
	objectPositions.push_back(glm::vec3(3.0, -0.5, 0.0));
	objectPositions.push_back(glm::vec3(-3.0, -0.5, 3.0));
	objectPositions.push_back(glm::vec3(0.0, -0.5, 3.0));
	objectPositions.push_back(glm::vec3(3.0, -0.5, 3.0));



	/*
	 * Light setting
	 * --------------------------------------------------------------------------------------------------------------------
	 */

	// Setting global light data
	CustomHelper::BlinnPhongLight_direct dirLight = {};
	CustomHelper::BlinnPhongLight_point pointLight = {};
	CustomHelper::BlinnPhongLight_spot spotLight = {};
	// Number of each type light
	unsigned int num_of_dirLight = 0;
	unsigned int num_of_pointLight = 0;
	unsigned int num_of_spotLight = 0;


	// Random generate part
	unsigned int num_of_random_dirLight = 0;
	unsigned int num_of_random_pointLight = 0;
	unsigned int num_of_random_spotLight = 0;
	// Appear area(x, z) = (-area, -area) ~ (area, area)
	float appear_area = 10.0f;
	// Appear height(y) = (0) ~ (height)
	float appear_hieght = 4.0f;
	// Minimum and maximum value of direction lights
	glm::vec3 min_dirlights_color(1.0f);
	glm::vec3 max_dirlights_color(100.0f);
	// Minimum and maximum value of point lights
	glm::vec3 min_pointlights_color(0.5f);
	glm::vec3 max_pointlights_color(15.0f);
	// Minimum and maximum value of spot lights
	glm::vec3 min_spotlights_color(0.5f);
	glm::vec3 max_spotlights_color(100.0f);
	for (unsigned int i = 0; i < num_of_random_dirLight; i++) {
		dirLight = CustomHelper::GenerateRandomGlobalBlinnPhongLight_dirLight(glm::vec3(-1.0f, -0.2, 0.1f), glm::vec3(-1.0f, -0.2, 0.1f), min_dirlights_color, max_dirlights_color);
		globalLightManager.updateDirLight(dirLight, num_of_dirLight++);
	}
	for (unsigned int i = 0; i < num_of_random_pointLight; i++) {
		pointLight = CustomHelper::GenerateRandomGlobalBlinnPhongLight_pointLight(glm::vec3(-appear_area, 1.0f, -appear_area), glm::vec3(appear_area, appear_hieght, appear_area), min_pointlights_color, max_pointlights_color);
		globalLightManager.updatePointLight(pointLight, num_of_pointLight++);
	}
	for (unsigned int i = 0; i < num_of_random_spotLight; i++) {
		spotLight = CustomHelper::GenerateRandomGlobalBlinnPhongLight_spotLight(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(-appear_area, 5.0f, -appear_area), glm::vec3(appear_area, 5.0f, appear_area), 12.5f, 90.0f, min_spotlights_color, max_spotlights_color);
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

	/* Disable v-sync */
	//glfwSwapInterval(0);

	// FPS time record
	double fps_previous_time = glfwGetTime(), fps_current_time, fps_delta_time = 0;
	// Record how many frames has passed
	unsigned int fps_passframe_count = 0;
	unsigned int fps = 0;

	while (!glfwWindowShouldClose(window)) {

		// FPS presentation
		//--------------------------------------------------
		// Refresh fps recorder
		fps_current_time = glfwGetTime();
		fps_delta_time = fps_current_time - fps_previous_time;
		fps_passframe_count++;
		// Update the fps presentation
		if (fps_delta_time >= 1.0) {
			fps = fps_passframe_count;
			glfwSetWindowTitle(window, ("LearnOpenGL FPS:" + std::to_string(fps)).c_str());
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
		shadowDrawFunction = [&](Shader &shader) {};
		//globalShadowLightManager.bindShadowMaps();


		// Update global lighting
		//--------------------------



		// Render Objects
		//--------------------------
		

		// **Geometry pass**
		//----------------------------------------------------------------------
		
		// Disable blending so alpha won't affect the result
		glDisable(GL_BLEND);

		// Rescale the view port to the size of the screen
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		// Bind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);

		// Render command
		//---------------
		// Clear Buffer
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// keep it black so it doesn't leak into g-buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		geometryShader.use();
		
		for (glm::vec3 position : objectPositions) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, position);
			model = glm::scale(model, glm::vec3(0.5f));
			normalMat = CustomHelper::CalculateNormalMat(model);

			geometryShader.setMat4("model", model);
			geometryShader.setMat3("normalMat", normalMat);
			
			backpack.Draw(geometryShader);
		}

		// Enable the blending
		glEnable(GL_BLEND);

		/*
		// Draw light cube
		CustomHelper::DrawGlobalPointLightCube(globalLightManager, cubeVAO, lightCubeShader, 0.025f);
		

		// **Lighting pass**
		//----------------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, hdr_initial_screen_framebuffer);

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



		// **Post-processing pass**
		//----------------------------------------------------------------------
		
		// Transfer the color from multisamples framebuffer to normal framebuffer
		
		glBindFramebuffer(GL_READ_FRAMEBUFFER, hdr_ms_render_screen_framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, hdr_initial_screen_framebuffer);
		glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		
		// Also copy the image to the framebuffer used to store post-processing effects
		glBindFramebuffer(GL_READ_FRAMEBUFFER, hdr_initial_screen_framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, hdr_process_screen_framebuffer);
		glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);



		// Post-processing scene
		//-------------------------
		glDisable(GL_DEPTH_TEST);


		// Bloom
		//--------------------
		if (applyBloom) {
			glBindFramebuffer(GL_FRAMEBUFFER, hdr_bloom_screen_framebuffer);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);


			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, hdr_process_screen_texture);

			hdr_bloom_screenShader.use();

			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			// Do gaussian blur
			bool horizontal = true, first_iteration = true;
			// How many times Gaussian blur to do
			unsigned int blur_times = 5;
			bloom_gaussian_blur_screenShader.use();
			// Do two-pass Gaussian blur
			for (unsigned int i = 0; i < blur_times * 2; i++) {
				glBindFramebuffer(GL_FRAMEBUFFER, pingpong_framebuffer[horizontal]);
				bloom_gaussian_blur_screenShader.setBool("horizontal", horizontal);
				// Use previous extract image for first time blur, then swap ping pong texture each iteration
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, (first_iteration) ? hdr_bloom_screen_textures[1] : pingpong_textures[!horizontal]);
				// Blur
				glBindVertexArray(quadVAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				// Swap horizontal state each iteration
				horizontal = !horizontal;

				if (first_iteration) {
					first_iteration = !first_iteration;
				}
			}

			// Combine bloom to scene
			glBindFramebuffer(GL_FRAMEBUFFER, hdr_process_screen_framebuffer);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			hdr_bloom_blending_screenShader.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, hdr_bloom_screen_textures[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, pingpong_textures[1]);
			// Combine
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}



		// Tone mapping
		//--------------------

		glBindFramebuffer(GL_FRAMEBUFFER, hdr_tone_mapping_screen_framebuffer);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hdr_process_screen_texture);

		// Determine whether to do automatic exposure adjustment. Using Reinhard tone mapping if not
		if (isEyeAdaptionEnable) {

			hdr_exposure_screenShader.use();

			// Generate mipmap
			glGenerateMipmap(GL_TEXTURE_2D);
			// Get maximum texture mipmap level
			size_t max_mipmap_level = static_cast<size_t>(std::log2(std::max(SCR_WIDTH, SCR_HEIGHT)));

			//
			// The automatic exposure algorithm is from https://blog.csdn.net/coldkaweh/article/details/62893076
			// And the average luminace is based on lowest mipmap level generate by OpenGL itself, so it is very inefficient
			//
			// Get average color by get lowest mipmap level pixel color
			glm::vec3 average_color;
			glGetTexImage(GL_TEXTURE_2D, max_mipmap_level, GL_RGB, GL_FLOAT, &average_color);
			// Calculate the real luminance in the scene
			const float real_luminance = 0.2126f * average_color.r + 0.7152 * average_color.g + 0.0722 * average_color.b;

			// Calculate adapted luminance
			float adapted_luminance = last_luminance + (real_luminance - last_luminance) * (1.0 - std::pow(0.98f, kExposureAdjustSpeed * (1.0f / fps)));
			// Clamp the luminance
			adapted_luminance = (adapted_luminance >= kMaxLuminance) ? kMaxLuminance : ((adapted_luminance <= kMinLuminance) ? kMinLuminance : adapted_luminance);

			// Record the adpated luminance to last luminace
			last_luminance = adapted_luminance;

			// Calculate the exposure
			exposure = kAverageLuminance / adapted_luminance;
			// Transfer exposure value to shader
			hdr_exposure_screenShader.setFloat("exposure", exposure);
		}
		else {
			// Transfer exposure value to shader
			hdr_reinhard_screenShader.use();
		}

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		// Transfer result to process image
		glBindFramebuffer(GL_READ_FRAMEBUFFER, hdr_tone_mapping_screen_framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, hdr_process_screen_framebuffer);
		glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		*/



		// Render final scene
		//-------------------------

		// Transfer final image to LDR framebuffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, hdr_process_screen_framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ldr_final_screen_framebuffer);
		glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);


		glDisable(GL_DEPTH_TEST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render final result to screen framebuffer

		screenShader.use();

		// Albedo
		glViewport(0, 0, SCR_WIDTH / 2, SCR_HEIGHT / 2);

		screenShader.setBool("specular", false);	// Bottom-left
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_color_spec);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		// Specular
		glViewport(SCR_WIDTH / 2, 0, SCR_WIDTH / 2, SCR_HEIGHT / 2);	// Bottom-right

		screenShader.setBool("specular", true);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_color_spec);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		// Position
		glViewport(0, SCR_HEIGHT / 2, SCR_WIDTH / 2, SCR_HEIGHT / 2);	// Top-left
		screenShader.use();

		screenShader.setBool("specular", false);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_position);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		// Normal
		glViewport(SCR_WIDTH / 2, SCR_HEIGHT / 2, SCR_WIDTH / 2, SCR_HEIGHT / 2);	// Top-right

		screenShader.setBool("specular", false);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_normal);

		glBindVertexArray(quadVAO);
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

	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && !vSyncKeyPressed) {
		needVerticalSync = !needVerticalSync;
		if (needVerticalSync) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		vSyncKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_RELEASE) {
		vSyncKeyPressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !eyeAdaptionKeyPressed) {
		isEyeAdaptionEnable = !isEyeAdaptionEnable;
		eyeAdaptionKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
		eyeAdaptionKeyPressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !bloomKeyPressed) {
		applyBloom = !applyBloom;
		bloomKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) {
		bloomKeyPressed = false;
	}
	
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
		std::cout << "Texture failed to load at path: " << path << std::endl;
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
unsigned int CreateColorFramebuffer(const size_t numOfColorAttachment, unsigned int *frameColortextures, const unsigned int width, const unsigned int height, const bool multisample, const unsigned int samples, const bool hdr) {

	unsigned int framebuffer;
	// Generate a framebuffer and get its ID
	glGenFramebuffers(1, &framebuffer);
	// Bind framebuffers
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);


	// Attach texture to framebuffer
	// Create a texture to store the scene's image
	glGenTextures(numOfColorAttachment, frameColortextures);

	// Determine whether use the multisampling texture
	GLenum texformat = (multisample) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

	// Determine whether use the float color attachment
	GLenum texture_color_format = (hdr) ? GL_RGB16F : GL_RGB;
	GLenum texture_data_type    = (hdr) ? GL_FLOAT : GL_UNSIGNED_BYTE;

	for (unsigned int i = 0; i < numOfColorAttachment; i++) {
		glBindTexture(texformat, frameColortextures[i]);
		if (multisample)
			glTexImage2DMultisample(texformat, samples, texture_color_format, width, height, GL_TRUE);
		else {
			glTexImage2D(texformat, 0, texture_color_format, width, height, 0, GL_RGBA, texture_data_type, NULL);
			glTexParameteri(texformat, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(texformat, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(texformat, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(texformat, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		glBindTexture(texformat, 0);

		// Attach the texture to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, texformat, frameColortextures[i], 0);
	}


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

	// Activate the number of render targets
	std::vector<GLenum> attachments;
	for (size_t i = 0; i < numOfColorAttachment; i++) {
		attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
	}
	glDrawBuffers(numOfColorAttachment, attachments.data());


	// Check whether the framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	// Bind framebuffer to default
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	return framebuffer;
}