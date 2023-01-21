#include "Light.h"

Light::Light() :
	pos(glm::vec3(0, 0, 0)),
	ambient(glm::vec3(0, 0, 1)),
	diffuse(glm::vec3(0.0f)),
	specular(glm::vec3(0.0f))
{
}

Light::~Light()
{
}