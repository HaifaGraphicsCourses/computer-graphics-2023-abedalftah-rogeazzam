#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <Face.h>

class Camera
{
public:
	Camera();
	Camera(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> texture, const std::string& model_name);
	virtual ~Camera();

	void SetCameraLookAt();

	const glm::mat4x4 GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation();
	void updateLocalTransformation();
	void updateWorldTransformation();
	void setWorldTrans(float x, float y, float z);
	void updateOrth();
	void updatePers();
	void DrawCameraHelp();
	std::vector<glm::fvec3> getVertices() const;
	std::vector<Face> getFaces() const;
	const glm::mat4x4& GetWorldTransformations();
	void updateDolly();

	glm::vec3 localRotateVec = glm::fvec3(0, 0, 0);
	float localDegreeToRotate = 0;
	glm::vec3 localTransVec = glm::fvec3(0, 0, 0);

	glm::vec3 worldRotateVec = glm::fvec3(0, 0, 0);
	float worldDegreeToRotate = 0;
	glm::vec3 worldTransVec = glm::fvec3(0, 0, 0);
	glm::fmat4 adjustMat = glm::mat4(1.0f);

	float left = -1, bottom = -1, nearr = 1, farr = -1, right = 1, top = 1;
	bool drawWorldAxisFlag = false;

	float perfar = 40.0f, pernear = 0.1f, fovy = glm::pi<float>() / 4, aspect = 2;
	bool isItPers = false;
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 at = glm::vec3(0, 0, 0);
	glm::vec3 eye = glm::vec3(0, 0, 10);
private:

	std::vector<Face> faces;
	std::vector<glm::fvec3> vertices;
	std::vector<glm::fvec3> normals;
	std::vector<glm::fvec2> texture;
	std::string model_name;

	glm::mat4x4 lookAt = glm::mat4x4(1.0f);
	glm::mat4x4 invMat = glm::mat4x4(1.0f);
	glm::mat4x4 ortho = glm::mat4x4(1.0f);
	glm::mat4x4 pers = glm::mat4x4(1.0f);
	glm::mat4x4 view_transformation = glm::mat4x4(1.0f);
	glm::mat4x4 projection_transformation = glm::mat4x4(1.0f);


	glm::fmat4x4 worldMatrixRX = glm::fmat4(1.0f);
	glm::fmat4x4 worldMatrixRY = glm::fmat4(1.0f);
	glm::fmat4x4 worldMatrixRZ = glm::fmat4(1.0f);
	glm::fmat4x4 worldMatrixT = glm::fmat4(1.0f);

	glm::fmat4x4 localMatrixRX = glm::fmat4(1.0f);
	glm::fmat4x4 localMatrixRY = glm::fmat4(1.0f);
	glm::fmat4x4 localMatrixRZ = glm::fmat4(1.0f);
	glm::fmat4x4 localMatrixT = glm::fmat4(1.0f);

	int centerX = 640, centerY = 320;
};