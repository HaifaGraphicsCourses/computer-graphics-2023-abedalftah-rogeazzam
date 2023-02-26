#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "MeshModel.h"
#include "Light.h"

using namespace std;

class Scene {
public:
	Scene();
	void AddModel(const shared_ptr<MeshModel>& mesh_model);
	int GetModelCount() const;
	MeshModel& GetModel(int index);
	vector<MeshModel> GetActiveModels(vector<int> index) const;

	void AddCamera(const shared_ptr<Camera>& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index);
	Camera& GetActiveCamera();

	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	void TurnOffActiveModel(int index);
	vector<int> GetActiveModelsIndexes() const;
	MeshModel& GetActiveModel(int index);

	void addLight(shared_ptr<Light> l);
	Light& getActiveLight();
	Light& getLight(int index);
	void SetActiveLightIndex(int index);
	int GetActiveLightIndex();
	int getLengthLights();
	vector<Light> getAllLights();
	glm::fvec3 computeColor(MeshModel scene, glm::fvec3 middleOfFace, glm::fvec3 normal);

	bool drawCameras = false;
	bool lightOn = false;
	bool flatShade = false;
	bool gouraudShade = false;
	bool phongShade = false;
	bool reflect_direction = false;

	int type = 0, mapping = 0;
	int transitionsNum = 4;

private:
	vector<shared_ptr<MeshModel>> mesh_models;
	vector<shared_ptr<Camera>> cameras;
	vector<shared_ptr<Light>> lights;

	int active_camera_index;
	int active_light_index;
	vector<int> active_model_index;//to change to an array
};