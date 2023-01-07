#pragma once
#include <glm/glm.hpp>
#include <string>
#include <cmath>
#include "Face.h"


class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> texture, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	glm::vec3 GetNormal(int index);
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	glm::mat4 MeshModel::transformationMat() const;
	glm::mat4 worldTransMat();
	std::vector<Face> getFaces();
	std::vector<glm::vec3> getVertices();
	glm::fvec3 MeshModel::getColor();
	void MeshModel::setColor(glm::fvec3 color);
	void calculateExtremes();

	void updateWorldMatrix();
	void updateLocalMatrix();
	void updateZPoints(glm::fmat4);

	bool localTrans = false;
	bool boundingRectangle = false;

	glm::vec3 localScaleVec = glm::fvec3(1, 1, 1);
	glm::vec3 localRotateVec = glm::fvec3(0, 0, 0);
	float localDegreeToRotate = 0;
	glm::vec3 localTransVec = glm::fvec3(0, 0, 0);

	glm::vec3 worldScaleVec = glm::fvec3(1, 1, 1);
	glm::vec3 worldRotateVec = glm::fvec3(0, 0, 0);
	float worldDegreeToRotate = 0;
	glm::vec3 worldTransVec = glm::fvec3(0, 0, 0);

	float mid_x = 0, mid_y = 0, mid_z = 0;
	float max_x = 0, max_y = 0, max_z = 0, min_x = 0, min_y = 0, min_z = 0;
	float maxZpoint = -INFINITY, minZpoint = INFINITY;

	bool faceNormals = false, vertexNormals = false, boundBox = false, boundBoxWorld = false, modelAxis = false;
	bool showRandom = false, showZbuff = false, showColorbuff = false, showRaterized = false;
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

	glm::fvec3 color = glm::fvec3(0.0f, 0.0f, 1.0f);
};