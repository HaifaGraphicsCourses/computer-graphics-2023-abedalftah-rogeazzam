#include "Camera.h"
#include <iostream>

Camera::Camera()
{
	adjustMat[3][0] = centerX;
	adjustMat[3][1] = centerY;
	this->updateOrth();
	this->updatePers();
	this->updateLocalTransformation();
	this->updateWorldTransformation();
	this->DrawCameraHelp();
}

Camera::Camera(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> texture, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals),
	texture(texture),
	model_name(model_name)
{
	adjustMat[3][0] = centerX;
	adjustMat[3][1] = centerY;
	this->updateOrth();
	this->updatePers();
	this->updateLocalTransformation();
	this->updateWorldTransformation();
	this->DrawCameraHelp();
}

Camera::~Camera()
{

}

void Camera::DrawCameraHelp()
{
	glm::mat3 s(1);
	s[2][2] = 1;
	float max_x = 0, max_y = 0, max_z = 0, min_x = 0, min_y = 0, min_z = 0;
	for (int i = 0; i < vertices.size(); i++) {
		if (this->vertices[i].x > max_x || i == 0)
			max_x = this->vertices[i].x;
		if (this->vertices[i].y > max_y || i == 0)
			max_y = this->vertices[i].y;
		if (this->vertices[i].z > max_z || i == 0)
			max_z = this->vertices[i].z;

		if (this->vertices[i].x < min_x || i == 0)
			min_x = this->vertices[i].x;
		if (this->vertices[i].y < min_y || i == 0)
			min_y = this->vertices[i].y;
		if (this->vertices[i].z < min_z || i == 0)
			min_z = this->vertices[i].z;
	}

	s[0][0] = s[1][1] = s[2][2] = (320 / max_x > 180 / max_y ? 180 / max_y : 320 / max_x);

	for (int i = 0; i < vertices.size(); i++) {
		this->vertices[i] = s * this->vertices[i];
		this->vertices[i].x = this->vertices[i].x + 640;
		this->vertices[i].y = this->vertices[i].y + 360;
	}
}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation()
{
	glm::mat4x4 mat = glm::lookAt(eye, at, up);
	glm::mat4x4 scaleDown = glm::mat4(1.0f);
	scaleDown[0][0] = scaleDown[1][1] = 0.3;
	scaleDown[2][2] = 0.001;
	if (!isItPers) {
		mat = adjustMat * ortho * mat * invMat;
	}
	else {
		mat = glm::fmat4(adjustMat) * pers * mat * invMat * scaleDown;
	}
	return mat;
}
void Camera::updateLocalTransformation()
{
	float x = glm::radians(localRotateVec.x);
	localMatrixRX[1][1] = cos(x);
	localMatrixRX[1][2] = sin(x);
	localMatrixRX[2][1] = -1 * sin(x);
	localMatrixRX[2][2] = cos(x);

	float y = glm::radians(localRotateVec.y);
	localMatrixRY[0][0] = cos(y);
	localMatrixRY[0][2] = sin(y);
	localMatrixRY[2][0] = -1 * sin(y);
	localMatrixRY[2][2] = cos(y);

	float z = glm::radians(localRotateVec.z);
	localMatrixRZ[0][0] = cos(z);
	localMatrixRZ[0][1] = sin(z);
	localMatrixRZ[1][0] = -1 * sin(z);
	localMatrixRZ[1][1] = cos(z);

	localMatrixT = glm::translate(glm::identity<glm::mat4>(), this->localTransVec);

	invMat = glm::inverse(worldMatrixT * worldMatrixRX * worldMatrixRY * worldMatrixRZ *
		localMatrixT * localMatrixRX * localMatrixRY * localMatrixRZ);
}

void Camera::updateWorldTransformation()
{
	float x = glm::radians(worldRotateVec.x);
	worldMatrixRX[1][1] = cos(x);
	worldMatrixRX[1][2] = sin(x);
	worldMatrixRX[2][1] = -1 * sin(x);
	worldMatrixRX[2][2] = cos(x);

	float y = glm::radians(worldRotateVec.y);
	worldMatrixRY[0][0] = cos(y);
	worldMatrixRY[0][2] = sin(y);
	worldMatrixRY[2][0] = -1 * sin(y);
	worldMatrixRY[2][2] = cos(y);

	float z = glm::radians(worldRotateVec.z);
	worldMatrixRZ[0][0] = cos(z);
	worldMatrixRZ[0][1] = sin(z);
	worldMatrixRZ[1][0] = -1 * sin(z);
	worldMatrixRZ[1][1] = cos(z);

	worldMatrixT = glm::translate(glm::identity<glm::mat4>(), worldTransVec);

	invMat = glm::inverse(worldMatrixT * worldMatrixRX * worldMatrixRY * worldMatrixRZ *
		localMatrixT * localMatrixRX * localMatrixRY * localMatrixRZ);

}

void Camera::setWorldTrans(float x, float y, float z)
{
	worldTransVec.x = x;
	worldTransVec.y = y;
	worldTransVec.z = z;
}

void Camera::updateOrth()
{
	ortho = glm::ortho(left, right, bottom, top, nearr, farr);
}

void Camera::updatePers()
{
	/*pers = glm::mat4(1.0f);
	pers[3][3] = 0;
	pers[0][0] = 1 / (tan(fovy / 2) * aspect);
	pers[1][1] = 1 / (tan(fovy / 2));
	pers[2][2] = -1 * (perfar + pernear) / (pernear - perfar);
	pers[3][2] = 1;
	pers[2][3] = 2 * perfar * pernear / (pernear - perfar);*/

	pers = glm::perspective(fovy, aspect, pernear, perfar);
}

std::vector<glm::fvec3> Camera::getVertices() const
{
	return this->vertices;
}

std::vector<Face> Camera::getFaces() const
{
	return this->faces;
}

glm::mat4x4& Camera::GetWorldTransformations()
{
	return worldMatrixT * worldMatrixRX * worldMatrixRY * worldMatrixRZ *
		localMatrixT * localMatrixRX * localMatrixRY * localMatrixRZ;
}

void Camera::updateDolly()
{
	this->fovy = glm::pi<float>() / 4;
	if (worldTransVec[2] + 1 != 0) {
		fovy /= worldTransVec[2] + 1;
	}
}