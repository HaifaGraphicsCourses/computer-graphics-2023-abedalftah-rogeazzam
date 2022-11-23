#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"


class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> texture, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	glm::mat4 MeshModel::transformationMat() const;
	std::vector<Face> getFaces();
	std::vector<glm::vec3> getVertices();
	glm::fvec3 MeshModel::getColor();
	void MeshModel::setColor(glm::fvec3 color);
	//std::vector<glm::fvec3> MeshModel::updateVertices();

	void updateWorldMatrix();
	void updateLocalMatrix();

	bool localTrans = false;
	//double midX = 0;
	//double midY = 0;

	//void setX(float x);
	//float getX();
	//void setY(float y);
	//float getY();

	glm::vec3 localScaleVec = glm::fvec3(1, 1, 1);
	glm::vec3 localRotateVec = glm::fvec3(0, 0, 0);
	float localDegreeToRotate = 0;
	glm::vec3 localTransVec = glm::fvec3(0, 0, 0);

	glm::vec3 worldScaleVec = glm::fvec3(1, 1, 1);
	glm::vec3 worldRotateVec = glm::fvec3(0, 0, 0);
	float worldDegreeToRotate = 0;
	glm::vec3 worldTransVec = glm::fvec3(0, 0, 0);
private:

	float x = 0;
	float y = 0;
	std::vector<Face> faces;
	std::vector<glm::fvec3> vertices;
	std::vector<glm::fvec3> normals;
	std::vector<glm::fvec2> texture;
	std::string model_name;

	glm::fmat4x4 worldMatrixS = glm::fmat4(1.0f);
	glm::fmat4x4 worldMatrixRX = glm::fmat4(1.0f);
	glm::fmat4x4 worldMatrixRY = glm::fmat4(1.0f);
	glm::fmat4x4 worldMatrixRZ = glm::fmat4(1.0f);
	glm::fmat4x4 worldMatrixT = glm::fmat4(1.0f);

	glm::fmat4x4 localMatrixS = glm::fmat4(1.0f);
	glm::fmat4x4 localMatrixRX = glm::fmat4(1.0f);
	glm::fmat4x4 localMatrixRY = glm::fmat4(1.0f);
	glm::fmat4x4 localMatrixRZ = glm::fmat4(1.0f);
	glm::fmat4x4 localMatrixT = glm::fmat4(1.0f);

	glm::fvec3 color = glm::fvec3(0.0f, 0.0f, 0.0f);
};