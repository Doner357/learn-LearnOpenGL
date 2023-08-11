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

namespace CustomHelper {

	enum VAOType {
		VAO_CUBE,
		VAO_QUAD,
		VAO_SKYBOX,
	};

	enum UBOBindingPoints {
		UBOPOINT_CAMERA_MATRICES = 0,
		UBOPOINT_BLINPHONG_LIGHTING = 1
	};

	enum LightType {
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT
	};

	enum LightProperties {
		MAX_NUM_DIRECTIONALLIGHT = 4,
		MAX_NUM_POINTLIGHT = 32,
		MAX_NUM_SPOTLIGHT = 8
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
			CameraMatricesManager(std::string name) {
				this->uniform_block_name = name;
				bindUniformBlockBuffer();
			}

			~CameraMatricesManager() {
				glDeleteBuffers(1, &(this->uniform_buffer));
			}

			void registerShader(Shader &shader) {
				unsigned int unifrom_block_index = glGetUniformBlockIndex(shader.ID, this->uniform_block_name.c_str());
				glUniformBlockBinding(shader.ID, unifrom_block_index, UBOPOINT_CAMERA_MATRICES);
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
				glBindBufferBase(GL_UNIFORM_BUFFER, UBOPOINT_CAMERA_MATRICES, (this->uniform_buffer));
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
			GlobalBlinnPongLightManager(std::string uniform_block_name) {
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
				glUniformBlockBinding(shader.ID, unifrom_block_index, UBOPOINT_BLINPHONG_LIGHTING);
			}

			// edit three type of lights : directional light
			bool editDirLight(BlinnPhongLight_direct &light_data, const size_t index) {
				bool success = false;
				if (index < max_of_dirLight) {
					if (updateDirLight(light_data, index)) {
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
			bool editPointLight(BlinnPhongLight_point &light_data, const size_t index) {
				bool success = false;
				if (index < max_of_pointLight) {
					if (updatePointLight(light_data, index)) {
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
			bool editSpotLight(BlinnPhongLight_spot &light_data, const size_t index) {
				bool success = false;
				if (index < max_of_spotLight) {
					if (updateSpotLight(light_data, index)) {
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

			// Print the debug message
			void debug() {
				debuger();
			}

		private:
			// Buffer Index
			unsigned int unifrom_buffer;
			// Uniform Block Name
			std::string uniform_block_name;
			// lights data records
			BlinnPhongLight_direct *dirLights;
			BlinnPhongLight_point *pointLights;
			BlinnPhongLight_spot *spotLights;

			// Record the maximum number of each light
			const size_t max_of_dirLight = MAX_NUM_DIRECTIONALLIGHT;
			const size_t max_of_pointLight = MAX_NUM_POINTLIGHT;
			const size_t max_of_spotLight = MAX_NUM_SPOTLIGHT;

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
			const GLsizeiptr dir_arr_bottom_pos = 0;
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
			const GLsizeiptr point_arr_bottom_pos = dir_arr_bottom_pos + dir_total_size + dir_next_arr_bias;
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
			const GLsizeiptr spot_arr_bottom_pos = point_arr_bottom_pos + point_total_size + point_next_arr_bias;
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
				glBindBufferBase(GL_UNIFORM_BUFFER, UBOPOINT_BLINPHONG_LIGHTING, (this->unifrom_buffer));
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}
			
			// Update the directional light data
			bool updateDirLight(BlinnPhongLight_direct &light_data, const size_t index) {
				bool success = false;

				// Map the uniform buffer and get its address
				glBindBuffer(GL_UNIFORM_BUFFER, unifrom_buffer);
				GLubyte * const bottomPos = static_cast<GLubyte *>(glMapBufferRange(GL_UNIFORM_BUFFER, dir_arr_bottom_pos + (index * dir_size), dir_size, GL_MAP_WRITE_BIT));

				if (bottomPos) {
					memcpy(bottomPos + dir_direction_pos, glm::value_ptr(light_data.direction), sizeof(light_data.direction));
					memcpy(bottomPos + dir_ambient_pos, glm::value_ptr(light_data.ambient), sizeof(light_data.ambient));
					memcpy(bottomPos + dir_diffuse_pos, glm::value_ptr(light_data.diffuse), sizeof(light_data.diffuse));
					memcpy(bottomPos + dir_specular_pos, glm::value_ptr(light_data.specular), sizeof(light_data.specular));
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
			bool updatePointLight(BlinnPhongLight_point &light_data, const size_t index) {
				bool success = false;

				// Map the uniform buffer and get its address
				glBindBuffer(GL_UNIFORM_BUFFER, unifrom_buffer);
				GLubyte *const bottomPos = static_cast<GLubyte *>(glMapBufferRange(GL_UNIFORM_BUFFER, point_arr_bottom_pos + (index * point_size), point_size, GL_MAP_WRITE_BIT));

				if (bottomPos) {
					memcpy(bottomPos + point_position_pos, glm::value_ptr(light_data.position), sizeof(light_data.position));
					memcpy(bottomPos + point_constant_pos, &light_data.constant, sizeof(light_data.constant));
					memcpy(bottomPos + point_linear_pos, &light_data.linear, sizeof(light_data.linear));
					memcpy(bottomPos + point_quadratic_pos, &light_data.quadratic, sizeof(light_data.quadratic));
					memcpy(bottomPos + point_ambient_pos, &light_data.ambient, sizeof(light_data.ambient));
					memcpy(bottomPos + point_diffuse_pos, &light_data.diffuse, sizeof(light_data.diffuse));
					memcpy(bottomPos + point_specular_pos, &light_data.specular, sizeof(light_data.specular));
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
			bool updateSpotLight(BlinnPhongLight_spot &light_data, const size_t index) {
				bool success = false;

				// Map the uniform buffer and get its address
				glBindBuffer(GL_UNIFORM_BUFFER, unifrom_buffer);
				GLubyte *const bottomPos = static_cast<GLubyte *>(glMapBufferRange(GL_UNIFORM_BUFFER, spot_arr_bottom_pos + (index * spot_size), spot_size, GL_MAP_WRITE_BIT));

				if (bottomPos) {
					memcpy(bottomPos + spot_direction_pos, glm::value_ptr(light_data.direction), sizeof(light_data.direction));
					memcpy(bottomPos + spot_position_pos, glm::value_ptr(light_data.position), sizeof(light_data.position));
					memcpy(bottomPos + spot_innerCutOff_pos, &light_data.innerCutOff, sizeof(light_data.innerCutOff));
					memcpy(bottomPos + spot_outerCutOff_pos, &light_data.outerCutOff, sizeof(light_data.outerCutOff));
					memcpy(bottomPos + spot_constant_pos, &light_data.constant, sizeof(light_data.constant));
					memcpy(bottomPos + spot_linear_pos, &light_data.linear, sizeof(light_data.linear));
					memcpy(bottomPos + spot_quadratic_pos, &light_data.quadratic, sizeof(light_data.quadratic));
					memcpy(bottomPos + spot_ambient_pos, &light_data.ambient, sizeof(light_data.ambient));
					memcpy(bottomPos + spot_diffuse_pos, &light_data.diffuse, sizeof(light_data.diffuse));
					memcpy(bottomPos + spot_specular_pos, &light_data.specular, sizeof(light_data.specular));
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

			void debuger() {
				std::cout << "total size: " << size_of_total_light << std::endl;

				std::cout << "\n";
				std::cout << "dir_direction_pos : " << dir_direction_pos << std::endl;
				std::cout << "dir_direction_size : " << dir_direction_size << std::endl;
				std::cout << "dir_ambient_pos : " << dir_ambient_pos << std::endl;
				std::cout << "dir_ambient_size : " << dir_ambient_size << std::endl;
				std::cout << "dir_diffuse_pos : " << dir_diffuse_pos << std::endl;
				std::cout << "dir_diffuse_size : " << dir_diffuse_size << std::endl;
				std::cout << "dir_specular_pos : " << dir_specular_pos << std::endl;
				std::cout << "dir_specular_size : " << dir_specular_size << std::endl;
				std::cout << "dir_size : " << dir_size << std::endl;
				std::cout << "dir_total_size : " << dir_total_size << std::endl;
				std::cout << "dir_arr_bottom_pos : " << dir_arr_bottom_pos << std::endl;
				std::cout << "dir_next_arr_bias : " << dir_next_arr_bias << std::endl;

				std::cout << "\n";
				// PointLight datas
				std::cout << "point_position_pos : " << point_position_pos << std::endl;
				std::cout << "point_position_size : " << point_position_size << std::endl;
				std::cout << "point_constant_pos : " << point_constant_pos << std::endl;
				std::cout << "point_constant_size : " << point_constant_size << std::endl;
				std::cout << "point_linear_pos : " << point_linear_pos << std::endl;
				std::cout << "point_linear_size : " << point_linear_size << std::endl;
				std::cout << "point_quadratic_pos : " << point_quadratic_pos << std::endl;
				std::cout << "point_quadratic_size : " << point_quadratic_size << std::endl;
				std::cout << "point_ambient_pos : " << point_ambient_pos << std::endl;
				std::cout << "point_ambient_size : " << point_ambient_size << std::endl;
				std::cout << "point_diffuse_pos : " << point_diffuse_pos << std::endl;
				std::cout << "point_diffuse_size : " << point_diffuse_size << std::endl;
				std::cout << "point_specular_pos : " << point_specular_pos << std::endl;
				std::cout << "point_specular_size : " << point_specular_size << std::endl;
				std::cout << "point_size : " << point_size << std::endl;
				std::cout << "point_total_size : " << point_total_size << std::endl;
				std::cout << "point_arr_bottom_pos : " << point_arr_bottom_pos << std::endl;
				std::cout << "point_next_arr_bias : " << point_next_arr_bias << std::endl;

				std::cout << "\n";
				// SpotLight datas
				std::cout << "spot_position_pos : " << spot_position_pos << std::endl;
				std::cout << "spot_position_size : " << spot_position_size << std::endl;
				std::cout << "spot_direction_pos : " << spot_direction_pos << std::endl;
				std::cout << "spot_direction_size : " << spot_direction_size << std::endl;
				std::cout << "spot_innerCutOff_pos : " << spot_innerCutOff_pos << std::endl;
				std::cout << "spot_innerCutOff_size : " << spot_innerCutOff_size << std::endl;
				std::cout << "spot_outerCutOff_pos : " << spot_outerCutOff_pos << std::endl;
				std::cout << "spot_outerCutOff_size : " << spot_outerCutOff_size << std::endl;
				std::cout << "spot_constant_pos : " << spot_constant_pos << std::endl;
				std::cout << "spot_constant_size : " << spot_constant_size << std::endl;
				std::cout << "spot_linear_pos : " << spot_linear_pos << std::endl;
				std::cout << "spot_linear_size : " << spot_linear_size << std::endl;
				std::cout << "spot_quadratic_pos : " << spot_quadratic_pos << std::endl;
				std::cout << "spot_quadratic_size : " << spot_quadratic_size << std::endl;
				std::cout << "spot_ambient_pos : " << spot_ambient_pos << std::endl;
				std::cout << "spot_ambient_size : " << spot_ambient_size << std::endl;
				std::cout << "spot_diffuse_pos : " << spot_diffuse_pos << std::endl;
				std::cout << "spot_diffuse_size : " << spot_diffuse_size << std::endl;
				std::cout << "spot_specular_pos : " << spot_specular_pos << std::endl;
				std::cout << "spot_specular_size : " << spot_specular_size << std::endl;
				std::cout << "spot_arr_bottom_pos : " << spot_arr_bottom_pos << std::endl;
				std::cout << "spot_size : " << spot_size << std::endl;
				std::cout << "spot_total_size : " << spot_total_size << std::endl;
				std::cout << "spot_next_arr_bias : " << spot_next_arr_bias << std::endl;
			}
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
		light.constant = 1.0f;
		light.linear = 0.09f;
		light.quadratic = 0.032f;
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
		light.constant = 1.0f;
		light.linear = 0.09f;
		light.quadratic = 0.025f;
		light.ambient = color * 0.2f;
		light.diffuse = color * 0.8f;
		light.specular = color;
		
		return light;
	}

	// Draw the point lights in the global light uniform block
	void DrawGlobalPointLightCube(GlobalBlinnPongLightManager &manager,const unsigned int VAO,  Shader &shader, const float scale, const float gamma, const unsigned int number = MAX_NUM_POINTLIGHT) {
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
			shader.setVec3("lightColor", manager.getpointLight(i).specular);
			shader.setFloat("gamma", gamma);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void DrawTexFloor(const unsigned int &quad_VAO, Shader & const shader, glm::vec3 & const viewPos, const unsigned int &diffuse_tex, const unsigned int &specular_tex, const float shininess, float gamma, const glm::vec3 position = glm::vec3(0.0f), const float scale = 1.0f, const int tex_repeate_times = 1) {
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
		shader.setFloat("gamma", gamma);
		shader.setInt("repeat_times", tex_repeate_times);

		glBindVertexArray(quad_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
		glUseProgram(0);
	}

}

#endif