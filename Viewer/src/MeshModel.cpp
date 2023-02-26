#include "MeshModel.h"
#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include <algorithm>

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> texture, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals),
	textureCoords(texture),
	model_name(model_name)
{
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
	mid.x = (max_x + min_x) / 2;
	mid.y = (max_y + min_y) / 2;
	mid.z = (max_z + min_z) / 2;

	bool computedNormal = false;

	modelVertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;
			Vertex vertex;
			vertex.position = vertices[vertexIndex];

			if (normals.size() > 0) {
				int normalIndex = currentFace.GetNormalIndex(j) - 1;
				vertex.normal = normals[normalIndex];
			}
			else {
				if(!computedNormal)
					this->normals = CalculateNormals(vertices, faces);
				computedNormal = true;
				vertex.normal = this->normals[vertexIndex];
			}

			if (texture.size() > 0)
			{
				int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
				vertex.textureCoords = texture[textureCoordsIndex];
			}
			modelVertices.push_back(vertex);
		}
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);

}

MeshModel::~MeshModel()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

const Face& MeshModel::GetFace(int index) const
{
	return faces[index];
}

glm::vec3 MeshModel::GetNormal(int index)
{
	return normals[index];
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

std::vector<glm::vec3> MeshModel::getNormals()
{
	return normals;
}

glm::fvec3 MeshModel::getAmbient()
{
	return ambient;
}

glm::fvec3 MeshModel::getSpecular()
{
	return specular;
}

glm::fvec3 MeshModel::getDiffuse()
{
	return diffuse;
}


void MeshModel::setColor(glm::fvec3 ambient, glm::fvec3 diffuse, glm::fvec3 specular)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
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

glm::mat4 MeshModel::NoTranslateMat() const
{
	return worldMatrixS * worldMatrixRX * worldMatrixRY * worldMatrixRZ *
		localMatrixRX * localMatrixRY * localMatrixRZ * localMatrixS;
}

glm::mat4 MeshModel::worldTransMat() {
	return worldMatrixT * worldMatrixS * worldMatrixRX * worldMatrixRY * worldMatrixRZ;
}

void MeshModel::calculateExtremes()
{
	for (int i = 0; i < vertices.size(); i++) {
		glm::fvec4 vertex = this->transformationMat() * glm::fvec4(this->vertices[i], 1);
		if (vertex.x > max_x || i == 0)
			max_x = vertex.x;
		if (vertex.y > max_y || i == 0)
			max_y = vertex.y;
		if (vertex.z > max_z || i == 0)
			max_z = vertex.z;

		if (vertex.x < min_x || i == 0)
			min_x = vertex.x;
		if (vertex.y < min_y || i == 0)
			min_y = vertex.y;
		if (vertex.z < min_z || i == 0)
			min_z = vertex.z;
	}
}

void MeshModel::updateZPoints(glm::fmat4 mat)
{
	for (int i = 0; i < faces.size(); i++) {
		int v1 = faces[i].GetVertexIndex(0) - 1;
		int v2 = faces[i].GetVertexIndex(1) - 1;
		int v3 = faces[i].GetVertexIndex(2) - 1;

		glm::vec3 cords[] = { vertices.at(v1), vertices.at(v2), vertices.at(v3) };

		glm::vec4 transformP1 = mat * glm::vec4(cords[0], 1);
		if (transformP1.w != 0) {
			cords[0].z = transformP1.z / transformP1.w;
		}
		else {
			cords[0].z = transformP1.z;
		}

		glm::vec4 transformP2 = mat * glm::vec4(cords[1], 1);
		if (transformP2.w != 0) {
			cords[1].z = transformP2.z / transformP2.w;
		}
		else {
			cords[1].z = transformP2.z;
		}
		glm::vec4 transformP3 = mat * glm::vec4(cords[2], 1);
		if (transformP3.w != 0) {
			cords[2].z = transformP3.z / transformP3.w;
		}
		else {
			cords[2].z = transformP3.z;
		}
		maxZpoint = std::max(maxZpoint, cords[0].z);
		maxZpoint = std::max(maxZpoint, cords[1].z);
		maxZpoint = std::max(maxZpoint, cords[2].z);

		minZpoint = std::min(minZpoint, cords[0].z);
		minZpoint = std::min(minZpoint, cords[1].z);
		minZpoint = std::min(minZpoint, cords[2].z);
	}
}

std::vector<glm::vec3> MeshModel::CalculateNormals(std::vector<glm::vec3> vertices, std::vector<Face> faces)
{
	std::vector<glm::vec3> normals(vertices.size());
	std::vector<int> adjacent_faces_count(vertices.size());

	for (int i = 0; i < adjacent_faces_count.size(); i++)
	{
		adjacent_faces_count[i] = 0;
	}

	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);

		int index0 = currentFace.GetVertexIndex(0) - 1;
		int index1 = currentFace.GetVertexIndex(1) - 1;
		int index2 = currentFace.GetVertexIndex(2) - 1;

		glm::vec3 v0 = vertices.at(index0);
		glm::vec3 v1 = vertices.at(index1);
		glm::vec3 v2 = vertices.at(index2);

		glm::vec3 u = v0 - v1;
		glm::vec3 v = v2 - v1;
		glm::vec3 face_normal = glm::normalize(-glm::cross(u, v));

		normals.at(index0) += face_normal;
		normals.at(index1) += face_normal;
		normals.at(index2) += face_normal;

		adjacent_faces_count.at(index0) += 1;
		adjacent_faces_count.at(index1) += 1;
		adjacent_faces_count.at(index2) += 1;
	}

	for (int i = 0; i < normals.size(); i++)
	{
		normals[i] /= adjacent_faces_count[i];
		normals[i] = glm::normalize(normals[i]);
	}

	return normals;
}