#pragma once

#include <glm/glm.hpp>

class Light
{
public:
	Light();
	~Light();

	glm::vec3 pos;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};
