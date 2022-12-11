#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include <iostream>

Scene::Scene() :
	active_camera_index(-1),
	active_model_index(0)
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
