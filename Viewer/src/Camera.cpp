#include "Camera.h"
#include <iostream>

Camera::Camera()
{
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

const glm::mat4x4 Camera::GetProjectionTransformation() const
{
	return invMat * projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation()
{
	view_transformation = glm::lookAt(eye, at, up);
	return view_transformation;
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
	projection_transformation = glm::ortho(left, right, bottom, top, nearr, farr);
}

void Camera::updatePers()
{
	projection_transformation = glm::perspective(fovy, aspect, pernear, perfar);
}

std::vector<glm::fvec3> Camera::getVertices() const
{
	return this->vertices;
}

std::vector<Face> Camera::getFaces() const
{
	return this->faces;
}

const glm::mat4x4& Camera::GetWorldTransformations()
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