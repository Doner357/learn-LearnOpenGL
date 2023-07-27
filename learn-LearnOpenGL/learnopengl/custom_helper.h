#ifndef COMMON_VAO_CREATOR_H
#define COMMON_VAO_CREATOR_H

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <vector>
#include <map>

enum VAOType {
	VAO_CUBE,
	VAO_QUAD,
	VAO_SKYBOX,
};

namespace CustomHelper {
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
}

#endif