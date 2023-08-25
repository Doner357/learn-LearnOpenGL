#ifndef COMMON_VAO_CREATOR_H
#define COMMON_VAO_CREATOR_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera_plus.h"

#include <random>
#include <ctime>
#include <cstdint>
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace CustomHelper {

	enum VAOType {
		VAO_CUBE,
		VAO_ROOM,
		VAO_QUAD,
		VAO_SKYBOX,
	};

	enum UBOBindingPoints {
		UBOPOINT_CAMERA_MATRICES = 0,
		UBOPOINT_BLINPHONG_LIGHTING = 1,
		UBOPOINT_GAMMA_CORRECTION = 2,
		UBOPOINT_BLINPHONG_SHADOWLIGHTING = 3,
		UBOPOINT_BLINPHONG_SHADOWMATRICES = 4,
		UBOPOINT_BLINPHONG_SHADOWFARPLANE = 5
	};
	std::string UBOPOINT_NAME_CAMERA_MATRICES = "CameraMatrices";
	std::string UBOPOINT_NAME_BLINPHONG_LIGHTING = "GlobalLights";
	std::string UBOPOINT_NAME_GAMMA_CORRECTION = "GammaCorrection";
	std::string UBOPOINT_NAME_BLINPHONG_SHADOWLIGHTING = "GlobalShadowLights";
	std::string UBOPOINT_NAME_BLINPHONG_SHADOWMATRICES = "ShadowMatrices";
	std::string UBOPOINT_NAME_BLINPHONG_SHADOWFARPLANE = "ShadowFarPlanes";
	
	enum LightType {
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT
	};

	enum LightProperties {
		MAX_NUM_DIRECTIONALLIGHT = 4,
		MAX_NUM_POINTLIGHT = 32,
		MAX_NUM_SPOTLIGHT = 8,
		MAX_NUM_SHADOWDIRECTIONALLIGHT = 1,
		MAX_NUM_SHADOWPOINTLIGHT = 4,
		MAX_NUM_SHADOWSPOTLIGHT = 2
	};

	enum ShaderSamplerNum {
		SAMPLER_DIFFUSE = 0,
		SAMPLER_SPECULAR = 1,
		SAMPLER_NORMAL = 2,
		SAMPLER_DIRSHADOW0 = 5,
		SAMPLER_POINTSHADOW0 = 6,
		SAMPLER_POINTSHADOW1 = 7,
		SAMPLER_POINTSHADOW2 = 8,
		SAMPLER_POINTSHADOW3 = 9,
		SAMPLER_SPOTSHADOW0 = 10,
		SAMPLER_SPOTSHADOW1 = 11
	};

	const float cubeVertices[] = {
		// positions			// normal				// texture Coords
		// Back face
		-1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f, // Bottom-left
		 1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.0f, 1.0f, // top-left
		// Front face
		-1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f, // top-right
		 1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f, // bottom-left
		// Left face
		-1.0f,  1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f, // top-right
		// Right face
		 1.0f,  1.0f,  1.0f,	 1.0f,  0.0f,  0.0f,	1.0f, 0.0f, // top-left
		 1.0f, -1.0f, -1.0f,	 1.0f,  0.0f,  0.0f,	0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,	 1.0f,  0.0f,  0.0f,	1.0f, 1.0f, // top-right         
		 1.0f, -1.0f, -1.0f,	 1.0f,  0.0f,  0.0f,	0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,	 1.0f,  0.0f,  0.0f,	1.0f, 0.0f, // top-left
		 1.0f, -1.0f,  1.0f,	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f, // bottom-left     
		 // Bottom face
		-1.0f, -1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	0.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	1.0f, 1.0f, // top-left
		 1.0f, -1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	1.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	0.0f, 1.0f, // top-right
		 // Top face
		-1.0f,  1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f, // top-left
		 1.0f,  1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 1.0f, // top-right     
		 1.0f,  1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f  // bottom-left
	};

	const float roomVertices[] = {
		// positions			// normal				// texture Coords
		// Back face
		 1.0f,  1.0f, -1.0f,	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f, // Bottom-left
		 1.0f, -1.0f, -1.0f,	 0.0f,  0.0f,  1.0f,	1.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f, // bottom-left
		 1.0f,  1.0f, -1.0f,	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f, // top-right
		-1.0f,  1.0f, -1.0f,	 0.0f,  0.0f,  1.0f,	0.0f, 1.0f, // top-left
		// Front face
		 1.0f, -1.0f,  1.0f,	 0.0f,  0.0f, -1.0f,	1.0f, 0.0f, // bottom-right
		-1.0f, -1.0f,  1.0f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f, // bottom-left
		 1.0f,  1.0f,  1.0f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,	 0.0f,  0.0f, -1.0f,	0.0f, 1.0f, // top-left
		 1.0f,  1.0f,  1.0f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f, // top-right
		-1.0f, -1.0f,  1.0f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f, // bottom-left
		// Left face
		-1.0f,  1.0f, -1.0f,	 1.0f,  0.0f,  0.0f,	1.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,	 1.0f,  0.0f,  0.0f,	1.0f, 0.0f, // top-right
		-1.0f, -1.0f, -1.0f,	 1.0f,  0.0f,  0.0f,	0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,	 1.0f,  0.0f,  0.0f,	0.0f, 1.0f, // bottom-left
		-1.0f,  1.0f,  1.0f,	 1.0f,  0.0f,  0.0f,	1.0f, 0.0f, // top-right
		// Right face
		 1.0f, -1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f, // top-left
		 1.0f,  1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f, // top-right
		 1.0f,  1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f, // top-left
		 1.0f, -1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f, // bottom-right
		 1.0f, -1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f, // bottom-left     
		 // Bottom face
		 1.0f, -1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f, // top-right
		 1.0f, -1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f, // bottom-right
		 1.0f, -1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f, // top-right
		 // Top face
		 1.0f,  1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	0.0f, 1.0f, // top-left
		 1.0f,  1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	1.0f, 1.0f, // top-right
		-1.0f,  1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	0.0f, 1.0f, // top-left
		 1.0f,  1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f  // bottom-left
	};

	const float quadVertices[] = {
		// postions		 // normal				// texture Coords
		 1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	1.0f,  1.0f,
		-1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.0f,  1.0f,
		-1.0f, -1.0f,	 0.0f,  0.0f,  1.0f,	0.0f,  0.0f,

		 1.0f, -1.0f,	 0.0f,  0.0f,  1.0f,	1.0f,  0.0f,
		 1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	1.0f,  1.0f,
		-1.0f, -1.0f,	 0.0f,  0.0f,  1.0f,	0.0f,  0.0f
	};

	const float skyboxVertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	class VAOManager {
		public:
			VAOManager() {
				InitialCubeVAO();
				InitialRoomVAO();
				InitialQuadVAO();
				InitialSkyboxVAO();
			};
			~VAOManager() {
				this->clean();
			}
			unsigned int getVAO(VAOType type) {
				std::string VAOname;
				switch (type) {
				case VAO_CUBE:
					VAOname = "Common_VAO_Cube";
					break;
				case VAO_ROOM:
					VAOname = "Common_VAO_Room";
					break;
				case VAO_QUAD:
					VAOname = "Common_VAO_Quad";
					break;
				case VAO_SKYBOX:
					VAOname = "Common_VAO_Skybox";
					break;
				default:
					break;
				}

				return findVAO(VAOname);
			}
			unsigned int getVAO(const std::string name) {
				return findVAO(name);
			}
			void registerVAO(const std::string name, const unsigned int VAO) {
				this->storedVAO[name] = VAO;
			}
			void clean() {
				unsigned int deleteVAO = 0;
				for (auto it = this->storedVAO.begin(); it != this->storedVAO.end(); it++)
					glDeleteVertexArrays(1, &(it->second));
			}
		private:
			std::map<std::string, unsigned int> storedVAO;
			
			void InitialCubeVAO() {
				unsigned int VAO, VBO;
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
				glBindVertexArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glDeleteBuffers(1, &VBO);

				this->registerVAO("Common_VAO_Cube", VAO);
			}
			void InitialRoomVAO() {
				unsigned int VAO, VBO;
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(roomVertices), &roomVertices[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
				glBindVertexArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glDeleteBuffers(1, &VBO);

				this->registerVAO("Common_VAO_Room", VAO);
			}
			void InitialQuadVAO() {
				unsigned int VAO, VBO;
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
				glBindVertexArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glDeleteBuffers(1, &VBO);

				this->registerVAO("Common_VAO_Quad", VAO);
			}
			void InitialSkyboxVAO() {
				unsigned int VAO, VBO;
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glBindVertexArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glDeleteBuffers(1, &VBO);

				this->registerVAO("Common_VAO_Skybox", VAO);
			}

			unsigned int findVAO(const std::string name) {
				unsigned int target = 0;
				std::map<std::string, unsigned int>::iterator it = this->storedVAO.find(name);
				if (it != this->storedVAO.end())
					target = it->second;

				return target;
			}
	};



	class CameraMatricesManager {
		public:
			CameraMatricesManager(std::string name, UBOBindingPoints point = UBOPOINT_CAMERA_MATRICES) : binding_point(point) {
				this->uniform_block_name = name;
				bindUniformBlockBuffer();
			}

			~CameraMatricesManager() {
				glDeleteBuffers(1, &(this->uniform_buffer));
			}

			void registerShader(Shader &shader) {
				unsigned int unifrom_block_index = glGetUniformBlockIndex(shader.ID, this->uniform_block_name.c_str());
				glUniformBlockBinding(shader.ID, unifrom_block_index, binding_point);
			}

			void updateView(glm::mat4 &viewMat) {
				glBindBuffer(GL_UNIFORM_BUFFER, (this->uniform_buffer));
				glBufferSubData(GL_UNIFORM_BUFFER, view_start_pos, view_size, glm::value_ptr(viewMat));
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}

			void updateProjection(glm::mat4 &projectMat) {
				glBindBuffer(GL_UNIFORM_BUFFER, (this->uniform_buffer));
				glBufferSubData(GL_UNIFORM_BUFFER, projection_start_pos, projection_size, glm::value_ptr(projectMat));
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}
		private:
			std::string uniform_block_name;
			unsigned int uniform_buffer;
			UBOBindingPoints binding_point;

			const GLsizeiptr view_start_pos = 0;
			const GLsizeiptr view_size = sizeof(glm::mat4);
			const GLsizeiptr view_next_bias = 0;
			const GLsizeiptr projection_start_pos = view_start_pos + view_size + view_next_bias;
			const GLsizeiptr projection_size = sizeof(glm::mat4);
			const GLsizeiptr projection_next_bias = 0;

			const GLsizeiptr size_of_total_block = view_size + view_next_bias + projection_size + projection_next_bias;

			void bindUniformBlockBuffer() {
				glGenBuffers(1, &(this->uniform_buffer));
				glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer);
				glBufferData(GL_UNIFORM_BUFFER, size_of_total_block, NULL, GL_DYNAMIC_DRAW);
				glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, (this->uniform_buffer));
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}
	};


	class GammaManager {
		public:
			GammaManager(std::string name, UBOBindingPoints point = UBOPOINT_GAMMA_CORRECTION) : binding_point(point) {
				this->uniform_block_name = name;
				this->gamma = 1.0f;
				bindUniformBlockBuffer();
			}

			~GammaManager() {
				glDeleteBuffers(1, &(this->uniform_buffer));
			}

			void registerShader(Shader &shader) {
				unsigned int unifrom_block_index = glGetUniformBlockIndex(shader.ID, this->uniform_block_name.c_str());
				glUniformBlockBinding(shader.ID, unifrom_block_index, binding_point);
			}

			void updateGamma(const float gamma) {
				this->gamma = gamma;
				glBindBuffer(GL_UNIFORM_BUFFER, (this->uniform_buffer));
				glBufferSubData(GL_UNIFORM_BUFFER, gamma_start_pos, gamma_size, &gamma);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}

			float getGamma() {
				return this->gamma;
			}

		private:
			std::string uniform_block_name;
			unsigned int uniform_buffer;
			UBOBindingPoints binding_point;
			float gamma;

			const GLsizeiptr gamma_start_pos = 0;
			const GLsizeiptr gamma_size = sizeof(float);
			const GLsizeiptr gamma_next_bias = 0;

			const GLsizeiptr size_of_total_block = gamma_start_pos + gamma_size + gamma_next_bias;

			void bindUniformBlockBuffer() {
				glGenBuffers(1, &(this->uniform_buffer));
				glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer);
				glBufferData(GL_UNIFORM_BUFFER, size_of_total_block, NULL, GL_DYNAMIC_DRAW);
				glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, (this->uniform_buffer));
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}
	};


	struct BlinnPhongLight_direct {
		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	struct BlinnPhongLight_point {
		glm::vec3 position;

		float constant;
		float linear;
		float quadratic;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	struct BlinnPhongLight_spot {
		glm::vec3 direction;
		glm::vec3 position;

		float innerCutOff;
		float outerCutOff;

		float constant;
		float linear;
		float quadratic;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};
	
	class GlobalBlinnPongLightManager {
		public:
			GlobalBlinnPongLightManager(std::string uniform_block_name, UBOBindingPoints point, const size_t dir_num, const size_t point_num, const size_t spot_num) : binding_point(point), max_of_dirLight(dir_num), max_of_pointLight(point_num), max_of_spotLight(spot_num) {
				// Set up unifrom block name
				this->uniform_block_name = uniform_block_name;
				dirLights = new BlinnPhongLight_direct[max_of_dirLight]();
				pointLights = new BlinnPhongLight_point[max_of_pointLight]();
				spotLights = new BlinnPhongLight_spot[max_of_spotLight]();
				// Initialize the unifrom block buffer
				bindUniformBlockBuffer();
			}

			// Delete all the sources which use dynamic memory and VRAM
			~GlobalBlinnPongLightManager() {
				glDeleteBuffers(1, &unifrom_buffer);
				delete[] dirLights;
				delete[] pointLights;
				delete[] spotLights;
			}

			// Bind the given shader onto the light uniform block points
			void registerShader(Shader& shader) {
				unsigned int unifrom_block_index = glGetUniformBlockIndex(shader.ID, uniform_block_name.c_str());
				glUniformBlockBinding(shader.ID, unifrom_block_index, this->binding_point);
			}

			// edit three type of lights : directional light
			bool updateDirLight(BlinnPhongLight_direct &light_data, const size_t index) {
				bool success = false;
				if (index < max_of_dirLight) {
					if (editDirLight(light_data, index)) {
						dirLights[index] = light_data;
						success = true;
					}
				}
				else {
					std::cerr << "ERROR::CUSTOMHELPER::LIGHTMANAGER::excess the max number of dirLight. (Index: " << index << ")\n";
				}

				return success;
			}

			// edit three type of lights : point light
			bool updatePointLight(BlinnPhongLight_point &light_data, const size_t index) {
				bool success = false;
				if (index < max_of_pointLight) {
					if (editPointLight(light_data, index)) {
						pointLights[index] = light_data;
						success = true;
					}
				}
				else {
					std::cerr << "ERROR::CUSTOMHELPER::LIGHTMANAGER::excess the max number of pointLight. (Index: " << index << ")\n";
				}

				return success;
			}

			// edit three type of lights : spot light
			bool updateSpotLight(BlinnPhongLight_spot &light_data, const size_t index) {
				bool success = false;
				if (index < max_of_spotLight) {
					if (editSpotLight(light_data, index)) {
							spotLights[index] = light_data;
							success = true;
					}
				}
				else {
					std::cerr << "ERROR::CUSTOMHELPER::LIGHTMANAGER::excess the max number of spotLight. (Index: " << index << ")\n";
				}

				return success;
			}

			// Return the index light data
			BlinnPhongLight_direct getDirLight(const size_t index) const {
				BlinnPhongLight_direct return_light = {};
				if (index < max_of_dirLight)
					return_light = dirLights[index];
				return return_light;
			}
			BlinnPhongLight_point getpointLight(const size_t index) const {
				BlinnPhongLight_point return_light = {};
				if (index < max_of_pointLight)
					return_light = pointLights[index];
				return return_light;
			}
			BlinnPhongLight_spot getspotLight(const size_t index) const {
				BlinnPhongLight_spot return_light = {};
				if (index < max_of_spotLight)
					return_light = spotLights[index];
				return return_light;
			}

		private:
			// Buffer Index
			unsigned int unifrom_buffer;
			// UBO binding point
			UBOBindingPoints binding_point;
			// Uniform Block Name
			std::string uniform_block_name;
			// lights data records
			BlinnPhongLight_direct *dirLights;
			BlinnPhongLight_point *pointLights;
			BlinnPhongLight_spot *spotLights;

			// Record the maximum number of each light
			size_t max_of_dirLight;
			size_t max_of_pointLight;
			size_t max_of_spotLight;

			// DirLight datas
			const GLsizeiptr dir_direction_pos = 0;
			const GLsizeiptr dir_direction_size = 3 * sizeof(float);
			const GLsizeiptr dir_ambient_pos = dir_direction_size + 4;
			const GLsizeiptr dir_ambient_size = 3 * sizeof(float);
			const GLsizeiptr dir_diffuse_pos = dir_ambient_pos + dir_ambient_size + 4;
			const GLsizeiptr dir_diffuse_size = 3 * sizeof(float);
			const GLsizeiptr dir_specular_pos = dir_diffuse_pos + dir_diffuse_size + 4;
			const GLsizeiptr dir_specular_size = 3 * sizeof(float);
			const GLsizeiptr dir_size = 64;
			const GLsizeiptr dir_total_size = dir_size * max_of_dirLight;
			const GLsizeiptr dir_arr_start_pos = 0;
			const GLsizeiptr dir_next_arr_bias = 0;

			// PointLight datas
			const GLsizeiptr point_position_pos = 0;
			const GLsizeiptr point_position_size = 3 * sizeof(float);
			const GLsizeiptr point_constant_pos = point_position_pos + point_position_size;
			const GLsizeiptr point_constant_size = sizeof(float);
			const GLsizeiptr point_linear_pos = point_constant_pos + point_constant_size;
			const GLsizeiptr point_linear_size = sizeof(float);
			const GLsizeiptr point_quadratic_pos = point_linear_pos + point_linear_size;
			const GLsizeiptr point_quadratic_size = sizeof(float);
			const GLsizeiptr point_ambient_pos = point_quadratic_pos + point_quadratic_size + 8;
			const GLsizeiptr point_ambient_size = 3 * sizeof(float);
			const GLsizeiptr point_diffuse_pos = point_ambient_pos + point_ambient_size + 4;
			const GLsizeiptr point_diffuse_size = 3 * sizeof(float);
			const GLsizeiptr point_specular_pos = point_diffuse_pos + point_diffuse_size + 4;
			const GLsizeiptr point_specular_size = 3 * sizeof(float);
			const GLsizeiptr point_size = 80;
			const GLsizeiptr point_total_size = point_size * max_of_pointLight;
			const GLsizeiptr point_arr_start_pos = dir_arr_start_pos + dir_total_size + dir_next_arr_bias;
			const GLsizeiptr point_next_arr_bias = 0;

			// SpotLight datas
			const GLsizeiptr spot_position_pos = 0;
			const GLsizeiptr spot_position_size = 3 * sizeof(float);
			const GLsizeiptr spot_direction_pos = spot_position_pos + spot_position_size + 4;
			const GLsizeiptr spot_direction_size = 3 * sizeof(float);
			const GLsizeiptr spot_innerCutOff_pos = spot_direction_pos + spot_direction_size;
			const GLsizeiptr spot_innerCutOff_size = sizeof(float);
			const GLsizeiptr spot_outerCutOff_pos = spot_innerCutOff_pos + spot_innerCutOff_size;
			const GLsizeiptr spot_outerCutOff_size = sizeof(float);
			const GLsizeiptr spot_constant_pos = spot_outerCutOff_pos + spot_outerCutOff_size;
			const GLsizeiptr spot_constant_size = sizeof(float);
			const GLsizeiptr spot_linear_pos = spot_constant_pos + spot_constant_size;
			const GLsizeiptr spot_linear_size = sizeof(float);
			const GLsizeiptr spot_quadratic_pos = spot_linear_pos + spot_linear_size;
			const GLsizeiptr spot_quadratic_size = sizeof(float);
			const GLsizeiptr spot_ambient_pos = spot_quadratic_pos + spot_quadratic_size;
			const GLsizeiptr spot_ambient_size = 3 * sizeof(float);
			const GLsizeiptr spot_diffuse_pos = spot_ambient_pos + spot_ambient_size + 4;
			const GLsizeiptr spot_diffuse_size = 3 * sizeof(float);
			const GLsizeiptr spot_specular_pos = spot_diffuse_pos + spot_diffuse_size + 4;
			const GLsizeiptr spot_specular_size = 3 * sizeof(float);
			const GLsizeiptr spot_arr_start_pos = point_arr_start_pos + point_total_size + point_next_arr_bias;
			const GLsizeiptr spot_size = 96;
			const GLsizeiptr spot_total_size = spot_size * max_of_spotLight;
			const GLsizeiptr spot_next_arr_bias = 0;

			// Unifrom block size constant
			// uniform total size
			const GLsizeiptr size_of_total_light = dir_total_size + dir_next_arr_bias
				+ point_total_size + point_next_arr_bias
				+ spot_total_size + spot_next_arr_bias;

			// Create a buffer to sotred lights data
			void bindUniformBlockBuffer() {
				glGenBuffers(1, &(this->unifrom_buffer));
				glBindBuffer(GL_UNIFORM_BUFFER, (this->unifrom_buffer));
				// Bind to GL_DYNAMIC_DRAW is really important that if don't do so, here will cause some buffer copy problems
				glBufferData(GL_UNIFORM_BUFFER, size_of_total_light, NULL, GL_DYNAMIC_DRAW);
				glBindBufferBase(GL_UNIFORM_BUFFER, this->binding_point, (this->unifrom_buffer));
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}
			
			// Update the directional light data
			bool editDirLight(BlinnPhongLight_direct &light_data, const size_t index) {
				bool success = false;

				// Map the uniform buffer and get its address
				glBindBuffer(GL_UNIFORM_BUFFER, unifrom_buffer);
				GLubyte * const startPos = static_cast<GLubyte *>(glMapBufferRange(GL_UNIFORM_BUFFER, dir_arr_start_pos + (index * dir_size), dir_size, GL_MAP_WRITE_BIT));

				if (startPos) {
					memcpy(startPos + dir_direction_pos, glm::value_ptr(light_data.direction), sizeof(light_data.direction));
					memcpy(startPos + dir_ambient_pos, glm::value_ptr(light_data.ambient), sizeof(light_data.ambient));
					memcpy(startPos + dir_diffuse_pos, glm::value_ptr(light_data.diffuse), sizeof(light_data.diffuse));
					memcpy(startPos + dir_specular_pos, glm::value_ptr(light_data.specular), sizeof(light_data.specular));
				}
				else {
					std::cerr << "ERROR::CUSTOMHELPER::LIGHTMANAGER::DIRLIGHT::fail to map the light uniform buffer\n";
				}

				GLenum map_success = glUnmapBuffer(GL_UNIFORM_BUFFER);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);

				if (map_success == GL_TRUE)
					success = true;
				else
					std::cerr << "ERROR::CUSTOMHELPER::LIGHTMANAGER::DIRLIGHT::map unsuccessful\n";

				return success;
			}

			// Update the point light data
			bool editPointLight(BlinnPhongLight_point &light_data, const size_t index) {
				bool success = false;

				// Map the uniform buffer and get its address
				glBindBuffer(GL_UNIFORM_BUFFER, unifrom_buffer);
				GLubyte *const startPos = static_cast<GLubyte *>(glMapBufferRange(GL_UNIFORM_BUFFER, point_arr_start_pos + (index * point_size), point_size, GL_MAP_WRITE_BIT));

				if (startPos) {
					memcpy(startPos + point_position_pos, glm::value_ptr(light_data.position), sizeof(light_data.position));
					memcpy(startPos + point_constant_pos, &light_data.constant, sizeof(light_data.constant));
					memcpy(startPos + point_linear_pos, &light_data.linear, sizeof(light_data.linear));
					memcpy(startPos + point_quadratic_pos, &light_data.quadratic, sizeof(light_data.quadratic));
					memcpy(startPos + point_ambient_pos, &light_data.ambient, sizeof(light_data.ambient));
					memcpy(startPos + point_diffuse_pos, &light_data.diffuse, sizeof(light_data.diffuse));
					memcpy(startPos + point_specular_pos, &light_data.specular, sizeof(light_data.specular));
				}
				else {
					std::cerr << "ERROR::CUSTOMHELPER::LIGHTMANAGER::POINTLIGHT::fail to map the light uniform buffer\n";
				}

				GLenum map_success = glUnmapBuffer(GL_UNIFORM_BUFFER);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);

				if (map_success == GL_TRUE)
					success = true;
				else
					std::cerr << "ERROR::CUSTOMHELPER::LIGHTMANAGER::POINTLIGHT::map unsuccessful\n";

				return success;
			}

			// Update the spot light data
			bool editSpotLight(BlinnPhongLight_spot &light_data, const size_t index) {
				bool success = false;

				// Map the uniform buffer and get its address
				glBindBuffer(GL_UNIFORM_BUFFER, unifrom_buffer);
				GLubyte *const startPos = static_cast<GLubyte *>(glMapBufferRange(GL_UNIFORM_BUFFER, spot_arr_start_pos + (index * spot_size), spot_size, GL_MAP_WRITE_BIT));

				if (startPos) {
					memcpy(startPos + spot_direction_pos, glm::value_ptr(light_data.direction), sizeof(light_data.direction));
					memcpy(startPos + spot_position_pos, glm::value_ptr(light_data.position), sizeof(light_data.position));
					memcpy(startPos + spot_innerCutOff_pos, &light_data.innerCutOff, sizeof(light_data.innerCutOff));
					memcpy(startPos + spot_outerCutOff_pos, &light_data.outerCutOff, sizeof(light_data.outerCutOff));
					memcpy(startPos + spot_constant_pos, &light_data.constant, sizeof(light_data.constant));
					memcpy(startPos + spot_linear_pos, &light_data.linear, sizeof(light_data.linear));
					memcpy(startPos + spot_quadratic_pos, &light_data.quadratic, sizeof(light_data.quadratic));
					memcpy(startPos + spot_ambient_pos, &light_data.ambient, sizeof(light_data.ambient));
					memcpy(startPos + spot_diffuse_pos, &light_data.diffuse, sizeof(light_data.diffuse));
					memcpy(startPos + spot_specular_pos, &light_data.specular, sizeof(light_data.specular));
				}
				else {
					std::cerr << "ERROR::CUSTOMHELPER::LIGHTMANAGER::SPOTLIGHT::fail to map the light uniform buffer\n";
				}

				GLenum map_success = glUnmapBuffer(GL_UNIFORM_BUFFER);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);

				if (map_success == GL_TRUE)
					success = true;
				else
					std::cerr << "ERROR::CUSTOMHELPER::LIGHTMANAGER::SPOTLIGHT::map unsuccessful\n";

				return success;
			}
	};


	class ShadowMatricesManager {
		public:
			ShadowMatricesManager(std::string name, UBOBindingPoints point, const size_t dir_num, const size_t spot_num) : binding_point(point), max_of_dirLight(dir_num), max_of_spotLight(spot_num) {
				this->uniform_block_name = name;
				this->dirMats = new glm::mat4[dir_num];
				this->spotMats = new glm::mat4[spot_num];
				bindUniformBlockBuffer();
			}

			~ShadowMatricesManager() {
				glDeleteBuffers(1, &(this->uniform_buffer));
			}

			void registerShader(Shader &shader) {
				unsigned int unifrom_block_index = glGetUniformBlockIndex(shader.ID, this->uniform_block_name.c_str());
				glUniformBlockBinding(shader.ID, unifrom_block_index, binding_point);
			}

			void updateDirLightMat(glm::mat4 &matrix, const size_t index) {
				if (index < max_of_dirLight) {
					this->dirMats[index] = matrix;
					glBindBuffer(GL_UNIFORM_BUFFER, (this->uniform_buffer));
					glBufferSubData(GL_UNIFORM_BUFFER, dirMat_start_pos + index * dirMat_size, dirMat_size, &matrix);
					glBindBuffer(GL_UNIFORM_BUFFER, 0);
				}
				else {
					std::cerr << "ERROR::CUSTOMHELPER::SHADOWMATRICES::excess the max number of dirLight. (Index: " << index << ")\n";
				}
			}

			void updateSpotLightMat(glm::mat4 &matrix, const size_t index) {
				if (index < max_of_spotLight) {
					this->spotMats[index] = matrix;
					glBindBuffer(GL_UNIFORM_BUFFER, (this->uniform_buffer));
					glBufferSubData(GL_UNIFORM_BUFFER, spotMat_start_pos + index * spotMat_size, spotMat_size, &matrix);
					glBindBuffer(GL_UNIFORM_BUFFER, 0);
				}
				else {
					std::cerr << "ERROR::CUSTOMHELPER::SHADOWMATRICES::excess the max number of spotLight. (Index: " << index << ")\n";
				}
			}

			glm::mat4 getDirMat(const size_t index) const {
				return this->dirMats[index];
			}

			glm::mat4 getSpotMat(const size_t index) const {
				return this->spotMats[index];
			}

		private:
			std::string uniform_block_name;
			unsigned int uniform_buffer;
			UBOBindingPoints binding_point;
			glm::mat4 *dirMats;
			glm::mat4 *spotMats;

			const size_t max_of_dirLight;
			const size_t max_of_spotLight;

			const GLsizeiptr dirMat_start_pos = 0;
			const GLsizeiptr dirMat_size = sizeof(glm::mat4);
			const GLsizeiptr dirMat_total_size = max_of_dirLight * dirMat_size;
			const GLsizeiptr dirMat_next_arr_bias = 0;

			const GLsizeiptr spotMat_start_pos = dirMat_start_pos + dirMat_total_size + dirMat_next_arr_bias;
			const GLsizeiptr spotMat_size = sizeof(glm::mat4);
			const GLsizeiptr spotMat_total_size = max_of_spotLight * spotMat_size;
			const GLsizeiptr spotMat_next_arr_bias = 0;

			const GLsizeiptr size_of_total_block = dirMat_total_size + dirMat_next_arr_bias + spotMat_total_size + spotMat_next_arr_bias;

			void bindUniformBlockBuffer() {
				glGenBuffers(1, &(this->uniform_buffer));
				glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer);
				glBufferData(GL_UNIFORM_BUFFER, size_of_total_block, NULL, GL_DYNAMIC_DRAW);
				glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, (this->uniform_buffer));
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}
	};


	class ShadowFarPlaneManager {
	public:
		ShadowFarPlaneManager(std::string name, UBOBindingPoints point, const size_t point_num) : binding_point(point), max_of_pointLight(point_num) {
			this->uniform_block_name = name;
			this->far_planes = new float[point_num];
			bindUniformBlockBuffer();
		}

		~ShadowFarPlaneManager() {
			delete[] far_planes;
			glDeleteBuffers(1, &(this->uniform_buffer));
		}

		void registerShader(Shader &shader) {
			unsigned int unifrom_block_index = glGetUniformBlockIndex(shader.ID, this->uniform_block_name.c_str());
			glUniformBlockBinding(shader.ID, unifrom_block_index, binding_point);
		}

		void updateFarPlane(float const &far_plane, const size_t index) {
			if (index < max_of_pointLight) {
				this->far_planes[index] = far_plane;
				glBindBuffer(GL_UNIFORM_BUFFER, (this->uniform_buffer));
				glBufferSubData(GL_UNIFORM_BUFFER, pointFar_start_pos + index * pointFar_size, pointFar_size, &far_plane);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}
			else {
				std::cerr << "ERROR::CUSTOMHELPER::FARPLANEMANAGER::excess the max number of pointLight. (Index: " << index << ")\n";
			}
		}

		float getFarPlane(const size_t index) const {
			return this->far_planes[index];
		}

	private:
		std::string uniform_block_name;
		unsigned int uniform_buffer;
		UBOBindingPoints binding_point;
		float *far_planes;
		
		const size_t max_of_pointLight;

		const GLsizeiptr pointFar_start_pos = 0;
		const GLsizeiptr pointFar_size = sizeof(float) + 3 * sizeof(float);    // Actually the float in array should as big as vec4, which is 4 * sizeof(float)
		const GLsizeiptr pointFar_total_size = max_of_pointLight * pointFar_size;
		const GLsizeiptr pointFar_next_arr_bias = 0;

		const GLsizeiptr size_of_total_block = pointFar_total_size + pointFar_next_arr_bias;

		void bindUniformBlockBuffer() {
			glGenBuffers(1, &(this->uniform_buffer));
			glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer);
			glBufferData(GL_UNIFORM_BUFFER, size_of_total_block, NULL, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, (this->uniform_buffer));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	};


	void GenDepthFramebuffers(const unsigned int n, unsigned int *const framebuffers) {
		glGenFramebuffers(n, framebuffers);
		for (unsigned int i = 0; i < n; i++) {
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[i]);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GenDirDepthMaps(const unsigned int n, unsigned int* const textures, const unsigned int width, const unsigned int height) {
		glGenTextures(n, textures);
		for (unsigned int i = 0; i < n; i++) {
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			// Set the image wraping to clamp to border and set the border to 1.0 so whenever the sample outside the depth map, it always return 1.0 and the shadow value will be 0.0
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GenCubeDepthMaps(const unsigned int n, unsigned int *const textures, const unsigned int width, const unsigned int height) {
		glGenTextures(n, textures);
		for (unsigned int i = 0; i < n; i++) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, textures[i]);
			for (unsigned int j = 0; j < 6; j++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}


	class GlobalBlinnPongShadowLightManager {
		public:
			GlobalBlinnPongShadowLightManager(
				std::string light_ubo_name,
				std::string matrices_ubo_name,
				std::string far_plane_ubo_name,
				UBOBindingPoints light_point,
				UBOBindingPoints matrices_point,
				UBOBindingPoints far_plane_point,
				size_t dir_num,
				size_t point_num,
				size_t spot_num,
				unsigned int dir_resolution,
				unsigned int point_resolution,
				unsigned int spot_resolution
			) : lightManager(light_ubo_name, light_point, dir_num, point_num, spot_num),
				matricesManager(matrices_ubo_name, matrices_point, dir_num, spot_num),
				farPlaneManager(far_plane_ubo_name, far_plane_point, point_num),
				num_of_dirLight(dir_num),
				num_of_pointLight(point_num),
				num_of_spotLight(spot_num),
				dirMapResolution(dir_resolution),
				pointMapResolution(point_resolution),
				spotMapResolution(spot_resolution),
				dir_bake_shader("shaders/bake/depth_map/vertex/dir-depth_map.vert", "shaders/bake/depth_map/fragment/dir-depth_map.frag"),
				cube_bake_shader("shaders/bake/depth_map/vertex/cube-depth_map.vert", "shaders/bake/depth_map/fragment/cube-depth_map.frag", "shaders/bake/depth_map/geometry/cube-depth_map.geom")
			{
				dir_depthMaps = new unsigned int[dir_num];
				point_depthMaps = new unsigned int[point_num];
				spot_depthMaps = new unsigned int[spot_num];
				//glGenFramebuffers(1, &this->framebuffer);
				GenDepthFramebuffers(1, &this->framebuffer);
				GenDirDepthMaps(num_of_dirLight, this->dir_depthMaps, dirMapResolution, dirMapResolution);
				GenCubeDepthMaps(num_of_pointLight, this->point_depthMaps, pointMapResolution, pointMapResolution);
				GenDirDepthMaps(num_of_spotLight, this->spot_depthMaps, spotMapResolution, spotMapResolution);
			}

			~GlobalBlinnPongShadowLightManager() {
				delete[] dir_depthMaps;
				delete[] point_depthMaps;
				delete[] spot_depthMaps;
				glDeleteFramebuffers(1, &this->framebuffer);
				glDeleteTextures(num_of_dirLight, dir_depthMaps);
				glDeleteTextures(num_of_pointLight, point_depthMaps);
				glDeleteTextures(num_of_spotLight, spot_depthMaps);
				dir_bake_shader.clear();
				cube_bake_shader.clear();
			}

			void registerShader(Shader &shader) {
				lightManager.registerShader(shader);
				matricesManager.registerShader(shader);
				farPlaneManager.registerShader(shader);
				shader.use();
				for (unsigned int i = 0; i < num_of_dirLight; i++)
					shader.setInt("shadowMaps.dirLights[" + std::to_string(i) + "]", SAMPLER_DIRSHADOW0);
				for (unsigned int i = 0; i < num_of_pointLight; i++)
					shader.setInt("shadowMaps.pointLights[" + std::to_string(i) + "]", SAMPLER_POINTSHADOW0 + i);
				for (unsigned int i = 0; i < num_of_pointLight; i++)
					shader.setInt("shadowMaps.spotLights[" + std::to_string(i) + "]", SAMPLER_SPOTSHADOW0 + i);
			}

			void changeDirShadowBakeShader(Shader &shadow_bake_shader) {
				this->dir_bake_shader = shadow_bake_shader;
			}
			void changeCubeShadowBakeShader(Shader &shadow_bake_shader) {
				this->cube_bake_shader = shadow_bake_shader;
			}

			void updateDirLight(BlinnPhongLight_direct &light, const size_t index, const float view_space, const float far_plane, const float line_bias, const glm::vec3 pos_bias, std::function<void(Shader &)> shadow_bake_function) {
				glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->dir_depthMaps[index], 0);
				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
					glm::vec3 lightPos = light.direction * -line_bias + pos_bias;
					// Default set the up direction of lookup matrix to positive y
					glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
					// If the light direction is parallel to y axis, set it to positive x
					if (glm::abs(light.direction) == glm::vec3(0.0f, 1.0f, 0.0))
						up = glm::vec3(1.0f, 0.0f, 0.0f);
					glm::mat4 lightProjection = glm::ortho(-view_space, view_space, -view_space, view_space, 0.1f, far_plane);
					glm::mat4 lightView = glm::lookAt(lightPos, lightPos + light.direction, up);
					glm::mat4 lightSpaceMatrix = lightProjection * lightView;
					
					this->lightManager.updateDirLight(light, index);
					this->matricesManager.updateDirLightMat(lightSpaceMatrix, index);

					glViewport(0, 0, this->dirMapResolution, this->dirMapResolution);
					glClear(GL_DEPTH_BUFFER_BIT);

					this->dir_bake_shader.use();
					this->dir_bake_shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
					// User should pass render command into this function
					shadow_bake_function(dir_bake_shader);

					glUseProgram(0);
				}
				else {
					std::cout << "ERROR::FRAMEBUFFER:: DirShadow map framebuffer is not complete!" << std::endl;
				}
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}

			void updatePointLight(BlinnPhongLight_point &light, const size_t index, const float far_plane, std::function<void(Shader &)> shadow_bake_function) {
				glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->point_depthMaps[index], 0);
				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
					glm::vec3 lightPos = light.position;
					glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), static_cast<float>(this->pointMapResolution) / static_cast<float>(this->pointMapResolution), 0.1f, far_plane);
					std::vector<glm::mat4> lightTransform = {
						lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
						lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
						lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
						lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
						lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
						lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
					};

					this->lightManager.updatePointLight(light, index);
					this->farPlaneManager.updateFarPlane(far_plane, index);

					glViewport(0, 0, this->pointMapResolution, this->pointMapResolution);
					glClear(GL_DEPTH_BUFFER_BIT);

					this->cube_bake_shader.use();
					for (unsigned int i = 0; i < 6; i++)
						this->cube_bake_shader.setMat4("shadowMatrices[" + std::to_string(i) + "]", lightTransform[i]);
					this->cube_bake_shader.setVec3("lightPos", lightPos);
					this->cube_bake_shader.setFloat("far_plane", far_plane);

					shadow_bake_function(this->cube_bake_shader);

					glUseProgram(0);
				}
				else {
					std::cout << "ERROR::FRAMEBUFFER:: PointShadow map framebuffer is not complete!" << std::endl;
				}
				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}

			void updateSpotLight(BlinnPhongLight_spot &light, const size_t index, const float angle, const float far_plane, std::function<void(Shader &)> shadow_bake_function) {
				glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->spot_depthMaps[index], 0);
				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
					// Default set the up direction of lookup matrix to positive y
					glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
					// If the light direction is parallel to y axis, set it to positive x
					if (glm::abs(light.direction) == glm::vec3(0.0f, 1.0f, 0.0))
						up = glm::vec3(1.0f, 0.0f, 0.0f);
					glm::mat4 lightProjection = glm::perspective(glm::radians(angle), static_cast<float>(this->spotMapResolution) / static_cast<float>(this->spotMapResolution), 0.1f, far_plane);
					glm::mat4 lightView = glm::lookAt(light.position, light.position + light.direction, up);
					glm::mat4 lightSpaceMatrix = lightProjection * lightView;
					
					this->lightManager.updateSpotLight(light, index);
					this->matricesManager.updateSpotLightMat(lightSpaceMatrix, index);

					glViewport(0, 0, this->spotMapResolution, this->spotMapResolution);
					glClear(GL_DEPTH_BUFFER_BIT);

					this->dir_bake_shader.use();
					this->dir_bake_shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
					// User should pass render command into this function
					shadow_bake_function(dir_bake_shader);

					glUseProgram(0);
				}
				else {
					std::cout << "ERROR::FRAMEBUFFER:: SpotShadow map framebuffer is not complete!" << std::endl;
				}
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}

			void bindShadowMaps() {
				for (unsigned int i = 0; i < num_of_dirLight; i++) {
					glActiveTexture(GL_TEXTURE0 + SAMPLER_DIRSHADOW0 + i);
					glBindTexture(GL_TEXTURE_2D, this->dir_depthMaps[i]);
				}
				for (unsigned int i = 0; i < num_of_pointLight; i++) {
					glActiveTexture(GL_TEXTURE0 + SAMPLER_POINTSHADOW0 + i);
					glBindTexture(GL_TEXTURE_CUBE_MAP, this->point_depthMaps[i]);
				}
				for (unsigned int i = 0; i < num_of_spotLight; i++) {
					glActiveTexture(GL_TEXTURE0 + SAMPLER_SPOTSHADOW0 + i);
					glBindTexture(GL_TEXTURE_2D, this->spot_depthMaps[i]);
				}
				glActiveTexture(GL_TEXTURE0);
			}

			void unbindShadowMaps() {
				for (unsigned int i = 0; i < num_of_dirLight; i++) {
					glActiveTexture(GL_TEXTURE0 + SAMPLER_DIRSHADOW0 + i);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
				for (unsigned int i = 0; i < num_of_pointLight; i++) {
					glActiveTexture(GL_TEXTURE0 + SAMPLER_POINTSHADOW0 + i);
					glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				}
				for (unsigned int i = 0; i < num_of_spotLight; i++) {
					glActiveTexture(GL_TEXTURE0 + SAMPLER_SPOTSHADOW0 + i);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
				glActiveTexture(GL_TEXTURE0);
			}

			unsigned int getDirDepthMap(size_t index) {
				return this->dir_depthMaps[index];
			}

			unsigned int getPointDepthMap(size_t index) {
				return this->point_depthMaps[index];
			}

			unsigned int getSpotDepthMap(size_t index) {
				return this->spot_depthMaps[index];
			}

		private:
			GlobalBlinnPongLightManager lightManager;
			ShadowMatricesManager matricesManager;
			ShadowFarPlaneManager farPlaneManager;
			const size_t num_of_dirLight;
			const size_t num_of_pointLight;
			const size_t num_of_spotLight;
			unsigned int framebuffer;
			unsigned int *dir_depthMaps;
			unsigned int *point_depthMaps;
			unsigned int *spot_depthMaps;
			const unsigned int dirMapResolution;
			const unsigned int pointMapResolution;
			const unsigned int spotMapResolution;
			Shader dir_bake_shader;
			Shader cube_bake_shader;
	};


	// Calculate the normal matrix
	glm::mat3 CalculateNormalMat(const glm::mat4 &modelMat) {
		return glm::mat3(glm::transpose(glm::inverse(modelMat)));
	}

	// Random color generator
	glm::vec3 GenerateRandomColor(glm::vec3 min_col = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 max_col = glm::vec3(1.0f, 1.0f, 1.0f)) {

		// Original generator version
		/*
		glm::vec3 color(0.0f);
		srand(glfwGetTime() * 1000000 + time(NULL));
		glm::vec3 range = (glm::abs(max_col - min_col) * glm::vec3(255.0f, 255.0f, 255.0f)) + glm::vec3(1.0f);
		color.r = (rand() % static_cast<int>(range.r)) / 255.0f + fminf(min_col.r, max_col.r);
		color.g = (rand() % static_cast<int>(range.g)) / 255.0f + fminf(min_col.g, max_col.g);
		color.b = (rand() % static_cast<int>(range.b)) / 255.0f + fminf(min_col.b, max_col.b);		
		*/

		// Lastest optimized version, using STL random generator
		typedef std::mt19937 random_engine;
		std::random_device rd;
		random_engine ran_gen(rd());
		std::uniform_real_distribution<> real_dist_r(min_col.r, max_col.r);
		std::uniform_real_distribution<> real_dist_g(min_col.g, max_col.g);
		std::uniform_real_distribution<> real_dist_b(min_col.b, max_col.b);

		glm::vec3 color(0.0f);
		color.r = static_cast<float>(real_dist_r(ran_gen));
		color.g = static_cast<float>(real_dist_g(ran_gen));
		color.b = static_cast<float>(real_dist_b(ran_gen));

		return color;
	}

	// Random vec3 generator
	glm::vec3 GenerateRandomVec3(const glm::vec3 min_vec3, const glm::vec3 max_vec3) {
		
		// Original generator version
		/*
		// Set up random seed and range
		srand(glfwGetTime() * 1000000 + time(NULL));
		glm::vec3 range = (glm::abs(max_vec3 - min_vec3) * glm::vec3(100.0f, 100.0f, 100.0f)) + glm::vec3(1.0f);
		glm::vec3 ran_vec;

		// Generate random vector
		ran_vec.x = (rand() % static_cast<int>(range.x)) / 100.0f + fminf(min_vec3.x, max_vec3.x);
		ran_vec.y = (rand() % static_cast<int>(range.y)) / 100.0f + fminf(min_vec3.y, max_vec3.y);
		ran_vec.z = (rand() % static_cast<int>(range.z)) / 100.0f + fminf(min_vec3.z, max_vec3.z);		
		*/

		// Lastest optimized version, using STL random generator
		// Set up distribute random number generator
		typedef std::mt19937 random_engine;
		std::random_device rd;
		random_engine ran_gen(rd());
		std::uniform_real_distribution<> real_dist_x(min_vec3.x, max_vec3.x);
		std::uniform_real_distribution<> real_dist_y(min_vec3.y, max_vec3.y);
		std::uniform_real_distribution<> real_dist_z(min_vec3.z, max_vec3.z);

		// Generate random vector
		glm::vec3 ran_vec;
		ran_vec.x = static_cast<float>(real_dist_x(ran_gen));
		ran_vec.y = static_cast<float>(real_dist_y(ran_gen));
		ran_vec.z = static_cast<float>(real_dist_z(ran_gen));


		return ran_vec;
	}

	float GenerateRandomAngle(const float min_angle, const float max_angle) {
		// Set up distribute random number generator
		typedef std::mt19937 random_engine;
		std::random_device rd;
		random_engine ran_gen(rd());
		std::uniform_real_distribution<> real_dist(min_angle, max_angle);

		// Generate random angle
		float angle = static_cast<float>(real_dist(ran_gen));

		return angle;
	}

	BlinnPhongLight_direct GenerateRandomGlobalBlinnPhongLight_dirLight(const glm::vec3 min_dir, const glm::vec3 max_dir, glm::vec3 min_col = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 max_col = glm::vec3(1.0f, 1.0f, 1.0f)) {
		BlinnPhongLight_direct light{};

		// Generate random color
		glm::vec3 color = GenerateRandomColor(min_col, max_col);
		// Generate random direction
		glm::vec3 direction = GenerateRandomVec3(min_dir, max_dir);

		light.direction = direction;
		light.ambient = color * 0.2f;
		light.diffuse = color * 0.5f;
		light.specular = color * 0.8f;

		return light;
	}

	BlinnPhongLight_point GenerateRandomGlobalBlinnPhongLight_pointLight(const glm::vec3 min_pos, const glm::vec3 max_pos, glm::vec3 min_col = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 max_col = glm::vec3(1.0f, 1.0f, 1.0f)) {
		BlinnPhongLight_point light{};

		// Generate random color
		glm::vec3 color = GenerateRandomColor(min_col, max_col);
		// Generate random position
		glm::vec3 position = GenerateRandomVec3(min_pos, max_pos);

		light.position = position;
		light.constant = 0.0f;
		light.linear = 0.0f;
		light.quadratic = 1.0f;
		light.ambient = color * 0.2f;
		light.diffuse = color * 0.8f;
		light.specular = color;

		return light;
	}

	BlinnPhongLight_spot GenerateRandomGlobalBlinnPhongLight_spotLight(const glm::vec3 min_dir, const glm::vec3 max_dir, 
																	   const glm::vec3 min_pos, const glm::vec3 max_pos, 
																	   const float min_angle  , const float max_angle,
																		glm::vec3 min_col = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 max_col = glm::vec3(1.0f, 1.0f, 1.0f)) {
		BlinnPhongLight_spot light{};

		// Generate color
		glm::vec3 color = GenerateRandomColor(min_col, max_col);
		// Generate random direction
		glm::vec3 direction = GenerateRandomVec3(min_dir, max_dir);
		// Generate random position
		glm::vec3 position = GenerateRandomVec3(min_pos, max_pos);
		// Generate random inner cutoff and outer cutoff. The inner cutoff is between 10 and 90 degree, and the outer cutoff is always 3 degrees larger than inner cutoff.
		float angle = GenerateRandomAngle(min_angle, max_angle);
		float innerCutoff = glm::cos(glm::radians(angle));
		float outerCutoff = glm::cos(glm::radians(angle + 3.0f));

		light.direction = direction;
		light.position = position;
		light.innerCutOff = innerCutoff;
		light.outerCutOff = outerCutoff;
		light.constant = 0.0f;
		light.linear = 0.0f;
		light.quadratic = 1.0f;
		light.ambient = color * 0.2f;
		light.diffuse = color * 0.8f;
		light.specular = color;
		
		return light;
	}

	// Draw the point lights in the global light uniform block
	void DrawGlobalPointLightCube(GlobalBlinnPongLightManager &manager,const unsigned int VAO,  Shader &shader, const float scale, const unsigned int number = MAX_NUM_POINTLIGHT) {
		shader.use();
		glBindVertexArray(VAO);
		BlinnPhongLight_point light = {};
		for (unsigned int i = 0; i < number; i++) {
			light = manager.getpointLight(i);
			// If the light isn't move, we assume it has not been set yet
			if (light.position == glm::vec3(0.0f))
				continue;
			glm::mat4 model(1.0f);
			model = glm::translate(model, manager.getpointLight(i).position);
			model = glm::scale(model, glm::vec3(scale));
			shader.setMat4("model", model);
			shader.setVec3("lightColor", manager.getpointLight(i).specular * 1.2f);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void DrawTexFloor(const unsigned int &quad_VAO, Shader & const shader, glm::vec3 & const viewPos, const unsigned int &diffuse_tex, const unsigned int &specular_tex, const float shininess, const glm::vec3 position = glm::vec3(0.0f), const float scale = 1.0f, const int tex_repeate_times = 1) {
		shader.use();
		shader.setInt("material.diffuse", 0);
		shader.setInt("material.specular", 1);
		shader.setFloat("material.shininess", shininess);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse_tex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular_tex);
		glm::mat4 model(1.0f);
		glm::mat3 normalMat(1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(scale));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		normalMat = CustomHelper::CalculateNormalMat(model);
		shader.setMat4("model", model);
		shader.setMat3("normalMat", normalMat);
		shader.setVec3("viewPos", viewPos);
		shader.setInt("repeat_times", tex_repeate_times);

		glBindVertexArray(quad_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
		glUseProgram(0);
	}

}

#endif