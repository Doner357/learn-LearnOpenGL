#ifndef LOOKAT_H
#define LOOKAT_H

#include <glm/glm.hpp>

glm::mat4 LookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
	glm::vec3 zaxis = glm::normalize(position - target);
	glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(up), zaxis));
	glm::vec3 yaxis = glm::cross(zaxis, xaxis);
	// translation matrix
	glm::mat4 translation = glm::mat4(1.0f);
	translation[3][0] = -position.x;
	translation[3][1] = -position.y;
	translation[3][2] = -position.z;
	// rotation matrix
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0][0] = xaxis.x;
	rotation[1][0] = xaxis.y;
	rotation[2][0] = xaxis.z;
	rotation[0][1] = yaxis.x;
	rotation[1][1] = yaxis.y;
	rotation[2][1] = yaxis.z;
	rotation[0][2] = zaxis.x;
	rotation[1][2] = zaxis.y;
	rotation[2][2] = zaxis.z;
	
	return rotation * translation;
}

#endif // !LOOKAT_H