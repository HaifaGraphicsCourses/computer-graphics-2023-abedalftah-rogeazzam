#include "MeshModel.h"
#include <iostream>
#include <glm/ext/matrix_transform.hpp>

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> texture, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals),
	texture(texture),
	model_name(model_name)
{
	glm::mat3 s(1);
	s[2][2] = 1;
	float max_x = 0, max_y = 0;
	for (int i = 0; i < vertices.size(); i++) {
		if (this->vertices[i].x > max_x)
			max_x = this->vertices[i].x;
		if (this->vertices[i].y > max_y)
			max_y = this->vertices[i].y;
	}

	s[0][0] = s[1][1] = (320 / max_x > 180 / max_y ? 180 / max_y : 320 / max_x);

	for (int i = 0; i < vertices.size(); i++) {
		this->vertices[i] = s * this->vertices[i];
	}

	double maxX = 0, maxY = 0, minX = 0, minY = 0, midX, midY;
	for (int i = 0; i < this->vertices.size(); i++) {

		std::cout << "v " << this->vertices[i].x <<
			" " << this->vertices[i].y << " "
			<< this->vertices[i].z << std::endl;

		if (i == 0) {
			minX = this->vertices[i].x;
			minY = this->vertices[i].y;
			maxX = this->vertices[i].x;
			maxY = this->vertices[i].y;
		}

		if (this->vertices[i].x < minX)
			minX = this->vertices[i].x;
		if (minY > this->vertices[i].y)
			minY = this->vertices[i].y;

		if (this->vertices[i].x > maxX)
			maxX = this->vertices[i].x;
		if (maxY < this->vertices[i].y)
			maxY = this->vertices[i].y;
	}

	midX = (maxX + minX) / 2;
	midY = (maxY + minY) / 2;

	for (int i = 0; i < this->vertices.size(); i++) {
		this->vertices[i].x -= midX;
		this->vertices[i].y -= midY;
	}

	for (int i = 0; i < faces.size(); i++) {
		std::cout << "f " << faces[i].GetVertexIndex(0) << "/" << "/" << faces[i].GetNormalIndex(0) << " ";
		std::cout << faces[i].GetVertexIndex(1) << "/" << "/" << faces[i].GetNormalIndex(1) << " ";
		std::cout << faces[i].GetVertexIndex(2) << "/" << "/" << faces[i].GetNormalIndex(2) << std::endl;
	}

}

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces[index];
}

int MeshModel::GetFacesCount() const
{
	return faces.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}

std::vector<Face> MeshModel::getFaces()
{
	return faces;
}

std::vector<glm::vec3> MeshModel::getVertices()
{
	return vertices;
}

glm::fvec3 MeshModel::getColor()
{
	return color;
}

void MeshModel::setColor(glm::fvec3 color)
{
	this->color = color;
}

void MeshModel::updateWorldMatrix()
{
	worldMatrixS = glm::scale(glm::identity<glm::mat4>(), worldScaleVec);

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
}

void MeshModel::updateLocalMatrix()
{
	localMatrixS = glm::scale(glm::identity<glm::mat4>(), this->localScaleVec);

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
}

glm::mat4 MeshModel::transformationMat() const
{
	return worldMatrixT * worldMatrixS * worldMatrixRX * worldMatrixRY * worldMatrixRZ *
		localMatrixT * localMatrixRX * localMatrixRY * localMatrixRZ * localMatrixS;
}
