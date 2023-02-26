#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include <iostream>

Scene::Scene() :
	active_camera_index(-1),
	active_model_index(0),
	active_light_index(-1)
{

}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
	mesh_models.push_back(mesh_model);
}

int Scene::GetModelCount() const
{
	return mesh_models.size();
}

MeshModel& Scene::GetModel(int index)
{
	return *mesh_models[index];
}

vector<MeshModel> Scene::GetActiveModels(vector<int> index) const
{
	vector<MeshModel> mesh;

	for (int i = 0; i < index.size(); i++)
		mesh.push_back(*mesh_models[index[i]]);

	return mesh;
}

void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
	cameras.push_back(camera);
}

int Scene::GetCameraCount() const
{
	return cameras.size();
}

Camera& Scene::GetCamera(int index)
{
	return *cameras[index];
}

Camera& Scene::GetActiveCamera()
{
	return *cameras[active_camera_index];
}

void Scene::SetActiveCameraIndex(int index)
{
	active_camera_index = index;
}

int Scene::GetActiveCameraIndex() const
{
	return active_camera_index;
}

void Scene::SetActiveModelIndex(int index)
{
	bool itsIn = false;

	for (int i = 0; i < active_model_index.size(); i++)
		if (active_model_index[i] == index)
			itsIn = true;

	if (!itsIn)
		active_model_index.push_back(index);
}


void Scene::TurnOffActiveModel(int index)
{

	int i = 0;
	for (; i < active_model_index.size(); i++)
		if (active_model_index[i] == index)
			break;


	if (!active_model_index.empty() && i < active_model_index.size())
		active_model_index.erase(active_model_index.begin() + i);


}

vector<int> Scene::GetActiveModelsIndexes() const
{
	return active_model_index;
}

MeshModel& Scene::GetActiveModel(int index)
{
	return *mesh_models[index];
}

void Scene::addLight(shared_ptr<Light> l)
{
	active_light_index++;
	lights.push_back(l);
}

Light& Scene::getActiveLight()
{
	return *lights.at(active_light_index);
}

Light& Scene::getLight(int index)
{
	return *lights.at(index);
}

void Scene::SetActiveLightIndex(int index)
{
	active_light_index = index;
}

int Scene::GetActiveLightIndex()
{
	return active_light_index;
}

int Scene::getLengthLights()
{
	return lights.size();
}

vector<Light> Scene::getAllLights()
{
	vector<Light> lights1;
	for (int i = 0; i < lights.size(); i++)
		lights1.push_back(*lights.at(i));
	return lights1;
}

glm::fvec3 Scene::computeColor(MeshModel mesh, glm::fvec3 middleOfFace, glm::fvec3 normal)
{
	glm::fvec3 finalColor = mesh.getAmbient();
	for (int i = 0; i < getLengthLights(); i++) {
		Light l = getLight(i);
		glm::fvec3 iA = mesh.getAmbient() * l.ambient;


		glm::fvec3 lightVec = middleOfFace - l.pos;

		float dotProduct = glm::dot(lightVec, normal);
		float vector1Magnitude = glm::length(lightVec);
		float vector2Magnitude = glm::length(normal);

		float cosAngle = dotProduct / (vector1Magnitude * vector2Magnitude);
		float angleRadians = acos(cosAngle);

		float angleDegrees = glm::degrees(angleRadians);

		glm::fvec3 iD = mesh.getDiffuse() * angleDegrees * l.diffuse;



		finalColor += iA + iD;
	}
	return finalColor;
}

