#define _USE_MATH_DEFINES
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <nfd.h>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <sstream>
#include <stdlib.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "Utils.h"

bool plane = false, cylinder = false, sphere = false, toonMapping = false, normalMapping = false, environmentMapping = false,
noneType = true, normalTexture = false, noneMapping = true;
bool show_demo_window = false;
bool show_another_window = false;
bool useMouse = false;
bool useKeyboard = false;
static bool localTrans = false;
static bool worldTrans = false;
static bool incChanges = false;
static bool cameraWorldTrans = false;
static bool cameraLocalTrans = false;
static bool lookatCB = false;
static char modelName[100];
static double mouseX = 640;
static double mouseY = 360;
static float fov = 45;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.00f);
glm::vec4 diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.00f);
glm::vec4 specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.00f);
static int num = 0, number = 0, number1 = 0;
static int lightNum = 0, lightNumber = 0, lightNumber1 = 0;
static bool lPos = false, lColor = false;
static float dollyTrans = 0;


double zoomFactor = 1;
int windowWidth = 1280;
int windowHeight = 720;
char* windowTitle = "OpenGL Demo";
glm::vec4 clearColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
bool zoomChanged = false;
std::shared_ptr<Scene> scene;

ImGuiIO* imgui;
GLFWwindow* window;

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupImgui(GLFWwindow* window);
bool Setup(int windowWidth, int windowHeight, const char* windowName);
void Cleanup();

static void GlfwErrorCallback(int error, const char* description);
void RenderFrame(GLFWwindow* window, std::shared_ptr<Scene> scene, Renderer& renderer, ImGuiIO& io);

void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height);

float GetAspectRatio();
void DrawImguiMenus();
void HandleImguiInput();

int main(int argc, char** argv)
{

	if (!Setup(windowWidth, windowHeight, windowTitle))
	{
		std::cerr << "Setup failed" << std::endl;
		return -1;
	}

	scene = std::make_shared<Scene>();
	scene->AddCamera(Utils::LoadCamera("..\\Data\\camera.obj"));
	scene->SetActiveCameraIndex(0);

	Renderer renderer;
	renderer.LoadShaders();
	renderer.LoadTextures();

	while (!glfwWindowShouldClose(window))
	{
		// Poll and process events
		glfwPollEvents();

		// Imgui stuff
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		DrawImguiMenus();
		ImGui::Render();
		HandleImguiInput();

		// Clear the screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render scene
		renderer.Render(scene);

		// Imgui stuff
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap front and back buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

bool Setup(int windowWidth, int windowHeight, const char* windowName)
{
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, windowName);
	if (!window)
	{
		std::cerr << "Window setup failed" << std::endl;
		return false;
	}

	imgui = &SetupImgui(window);

	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glEnable(GL_DEPTH_TEST);

	return true;
}

GLFWwindow* SetupGlfwWindow(int windowWidth, int windowHeight, const char* windowName)
{
	// Intialize GLFW
	if (!glfwInit())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create an OpenGL 3.3 core, forward compatible context window
	window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window's context the current one
	glfwMakeContextCurrent(window);

	// Setup window events callbacks
	glfwSetFramebufferSizeCallback(window, glfw_OnFramebufferSize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// An error occured
		std::cerr << "GLAD initialization failed" << std::endl;
		return false;
	}

	return window;
}

ImGuiIO& SetupImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Setup style
	ImGui::StyleColorsDark();

	glfwSetScrollCallback(window, glfw_OnMouseScroll);

	return io;
}

void HandleImguiInput()
{
	if (!imgui->WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here
		vector<int> indexes = scene->GetActiveModelsIndexes();
		if (imgui->KeysDown['B'])
		{
			for (int i = 0; i < indexes.size(); i++) {
				if (scene->GetActiveModel(indexes[i]).GetModelName() == modelName) {
					if (localTrans) {
						glm::fvec3 s = scene->GetActiveModel(indexes[i]).localScaleVec;
						scene->GetActiveModel(indexes[i]).localScaleVec = glm::fvec3(s[0] + 0.02, s[1] + 0.02, s[2] + 0.02);
						scene->GetActiveModel(indexes[i]).updateLocalMatrix();
					}
					else if (worldTrans) {
						glm::fvec3 s = scene->GetActiveModel(indexes[i]).worldScaleVec;
						scene->GetActiveModel(indexes[i]).worldScaleVec = glm::fvec3(s[0] + 0.02, s[1] + 0.02, s[2] + 0.02);
						scene->GetActiveModel(indexes[i]).updateWorldMatrix();
					}
				}
			}
		}
		else if (imgui->KeysDown['M'])
		{
			for (int i = 0; i < indexes.size(); i++) {
				if (scene->GetActiveModel(indexes[i]).GetModelName() == modelName) {
					if (localTrans) {
						glm::fvec3 s = scene->GetActiveModel(indexes[i]).localScaleVec;
						if (s[0] - 0.2 >= 0) {
							scene->GetActiveModel(indexes[i]).localScaleVec = glm::fvec3(s[0] - 0.02, s[1] - 0.02, s[2] - 0.02);
							scene->GetActiveModel(indexes[i]).updateLocalMatrix();
						}
					}
					else if (worldTrans) {
						glm::fvec3 s = scene->GetActiveModel(indexes[i]).worldScaleVec;
						if (s[0] - 0.2 >= 0) {
							scene->GetActiveModel(indexes[i]).worldScaleVec = glm::fvec3(s[0] - 0.02, s[1] - 0.02, s[2] - 0.02);
							scene->GetActiveModel(indexes[i]).updateWorldMatrix();
						}
					}
				}
			}
		}
		else if (imgui->KeysDown['W'])
		{
			for (int i = 0; i < indexes.size(); i++) {
				if (scene->GetActiveModel(indexes[i]).GetModelName() == modelName) {
					if (localTrans) {
						glm::fvec3 s = scene->GetActiveModel(indexes[i]).localTransVec;
						scene->GetActiveModel(indexes[i]).localTransVec = glm::fvec3(s[0], s[1] + 0.1, s[2]);
						scene->GetActiveModel(indexes[i]).updateLocalMatrix();
					}
					else if (worldTrans) {
						glm::fvec3 s = scene->GetActiveModel(indexes[i]).worldTransVec;
						scene->GetActiveModel(indexes[i]).worldTransVec = glm::fvec3(s[0], s[1] + 0.1, s[2]);
						scene->GetActiveModel(indexes[i]).updateWorldMatrix();
					}
				}
			}
		}
		else if (imgui->KeysDown['D'])
		{
			for (int i = 0; i < indexes.size(); i++) {
				if (scene->GetActiveModel(indexes[i]).GetModelName() == modelName) {
					if (localTrans) {
						glm::fvec3 s = scene->GetActiveModel(indexes[i]).localTransVec;
						scene->GetActiveModel(indexes[i]).localTransVec = glm::fvec3(s[0] + 0.1, s[1], s[2]);
						scene->GetActiveModel(indexes[i]).updateLocalMatrix();
					}
					else if (worldTrans) {
						glm::fvec3 s = scene->GetActiveModel(indexes[i]).worldTransVec;
						scene->GetActiveModel(indexes[i]).worldTransVec = glm::fvec3(s[0] + 0.1, s[1], s[2]);
						scene->GetActiveModel(indexes[i]).updateWorldMatrix();
					}
				}
			}
		}
		else if (imgui->KeysDown['A']) {
			for (int i = 0; i < indexes.size(); i++) {
				if (scene->GetActiveModel(indexes[i]).GetModelName() == modelName) {
					if (localTrans) {
						glm::fvec3 s = scene->GetActiveModel(indexes[i]).localTransVec;
						scene->GetActiveModel(indexes[i]).localTransVec = glm::fvec3(s[0] - 0.1, s[1], s[2]);
						scene->GetActiveModel(indexes[i]).updateLocalMatrix();
					}
					else if (worldTrans) {
						glm::fvec3 s = scene->GetActiveModel(indexes[i]).worldTransVec;
						scene->GetActiveModel(indexes[i]).worldTransVec = glm::fvec3(s[0] - 0.1, s[1], s[2]);
						scene->GetActiveModel(indexes[i]).updateWorldMatrix();
					}
				}
			}
		}
		else if (imgui->KeysDown['S']) {
			for (int i = 0; i < indexes.size(); i++) {
				if (scene->GetActiveModel(indexes[i]).GetModelName() == modelName) {
					if (localTrans) {
						glm::fvec3 s = scene->GetActiveModel(indexes[i]).localTransVec;
						scene->GetActiveModel(indexes[i]).localTransVec = glm::fvec3(s[0], s[1] - 0.1, s[2]);
						scene->GetActiveModel(indexes[i]).updateLocalMatrix();
					}
					else if (worldTrans) {
						glm::fvec3 s = scene->GetActiveModel(indexes[i]).worldTransVec;
						scene->GetActiveModel(indexes[i]).worldTransVec = glm::fvec3(s[0], s[1] - 0.1, s[2]);
						scene->GetActiveModel(indexes[i]).updateWorldMatrix();
					}
				}
			}
		}
	}
	//we need to get the active cameras
	if (!imgui->WantCaptureKeyboard && cameraWorldTrans) {
		if (imgui->KeysDown['L']) {
			scene->GetCamera(0).worldTransVec[0] -= 10;
		}
		else if (imgui->KeysDown['R']) {
			scene->GetCamera(0).worldTransVec[0] += 10;
		}
		scene->GetCamera(0).updateWorldTransformation();
	}
	if (!imgui->WantCaptureMouse && useMouse)
	{
		// TODO: Handle mouse events here
		// there is a bug sometimes it moves the inappropriate object
		// because modelName has the latest name that we inserted or made it active
		// because we do not need to close the window everytime we need to active an object
		//solved
		// we need to make a variable that the value can change appropriate to the center of the object
		// that because we did not update the vertices when we multiplied by a scalar or translated it
		// solved, we need to take care when we scale and rotate
		// tried to connect the keyboard to the mouse it's hard because the vector that we use to
		// change the silders it's used to matrices and it makes problem
		// what an idiot problem that took so much time, solved.

		if (imgui->MouseDown[0])
		{
			vector<int> indexes = scene->GetActiveModelsIndexes();
			for (int i = 0; i < indexes.size(); i++) {
				if (scene->GetActiveModel(indexes[i]).GetModelName() == modelName) {
					if (localTrans) {
						glm::fvec3 s = scene->GetActiveModel(indexes[i]).localTransVec;
						scene->GetActiveModel(indexes[i]).localTransVec = glm::fvec3(imgui->MousePos[0] - mouseX, mouseY - imgui->MousePos[1], s[2]);
						scene->GetActiveModel(indexes[i]).updateLocalMatrix();
					}
					else if (worldTrans) {
						glm::fvec3 s = scene->GetActiveModel(indexes[i]).worldTransVec;
						scene->GetActiveModel(indexes[i]).localTransVec = glm::fvec3(imgui->MousePos[0] - mouseX, mouseY - imgui->MousePos[1], s[2]);
						scene->GetActiveModel(indexes[i]).updateWorldMatrix();
					}
				}
			}
		}

	}
}

void Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, windowWidth, windowHeight);
	scene->GetActiveCamera().aspect = GetAspectRatio();
}

void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	zoomFactor = glm::pow(1.1, -yoffset);
	zoomChanged = true;
}

float GetAspectRatio()
{
	return static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
}

void DrawImguiMenus()
{
	ImGui::Begin("MeshViewer Menu");

	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene->AddModel(Utils::LoadMeshModel(outPath));
					free(outPath);

				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	ImGui::End();


	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{

		static float f = 0.0f;
		static float g = 0.0f;
		static int counter = 0;


		static bool activeModel = false;
		int modelIndex = -1;

		static bool transWindow = false;
		static bool cameraWindow = false;
		static bool ortho = false;
		static bool persp = false;
		static bool moreFeatures = false;


		ImGui::Begin("A Lot Of Things");
		ImGui::Checkbox("Object Control Window", &transWindow);
		ImGui::Checkbox("Camera Control Window", &cameraWindow);
		ImGui::Checkbox("Light Control Window", &scene->lightOn);
		ImGui::ColorEdit3("Clear Color", (float*)&clear_color);



		if (scene->lightOn) {
			ImGui::Begin("Light Control Window");
			if (ImGui::Button("Add Light")) {
				Light l = Light();
				scene->addLight(make_shared<Light>(l));
			}
			ImGui::InputInt("Light Index", &lightNum, lightNumber, lightNumber1, false);
			if (lightNum >= 0 && lightNum < scene->getLengthLights())
				scene->SetActiveLightIndex(lightNum);
			else {
				ImGui::Begin("Error Light Index");
				ImGui::Text("Please enter a valid light Index from 0 to %d", scene->getLengthLights() - 1);
				ImGui::End();
			}
			ImGui::Checkbox("Light position", &lPos);
			ImGui::Checkbox("Color position", &lColor);
			if (lColor && scene->getLengthLights() >= 1) {
				ImGui::ColorEdit3("ambient", (float*)&scene->getActiveLight().ambient);
				ImGui::ColorEdit3("diffuse", (float*)&scene->getActiveLight().diffuse);
				ImGui::ColorEdit3("specular", (float*)&scene->getActiveLight().specular);
			}
			if (lPos && scene->getLengthLights() >= 1) {
				ImGui::SliderFloat("position x", &scene->getActiveLight().pos[0], 0, 1280);
				ImGui::SliderFloat("position y", &scene->getActiveLight().pos[1], 0, 720);
				ImGui::SliderFloat("position z", &scene->getActiveLight().pos[2], 0, 2000);
			}
			else if (lPos && scene->getLengthLights() < 1) {
				ImGui::Begin("Error Light Index");
				ImGui::Text("Please enter a valid light Index from 0 to %d", scene->getLengthLights() - 1);
				ImGui::End();
			}
			ImGui::End();
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		if (transWindow) {
			ImGui::Begin("Object Control");
			ImGui::InputText("Which model? ", modelName, 100);
			ImGui::Checkbox("Turn on/off the model", &activeModel);
			ImGui::ColorEdit3("ambient", (float*)&ambient);
			ImGui::ColorEdit3("diffuse", (float*)&diffuse);
			ImGui::ColorEdit3("specular", (float*)&specular);
			ImGui::Checkbox("Local Transformations", &localTrans);
			ImGui::SameLine();
			ImGui::Checkbox("World Transformations", &worldTrans);

			ImGui::NewLine();



			if (ImGui::Checkbox("No Type   ", &noneType) && noneType) {
				scene->type = 0;
				plane = false;
				cylinder = false;
				sphere = false;
			}

			ImGui::SameLine();

			if (ImGui::Checkbox("No Mapping", &noneMapping) && noneMapping) {
				scene->mapping = 0;
				toonMapping = false;
				normalMapping = false;
				environmentMapping = false;
				normalTexture = false;
			}

			if (ImGui::Checkbox("Plane     ", &plane) && plane) {
				scene->type = 1;
				noneType = false;
				cylinder = false;
				sphere = false;
			}

			ImGui::SameLine();

			if (ImGui::Checkbox("Normal Mapping", &normalMapping) && normalMapping) {
				scene->mapping = 1;
				noneMapping = false;
				toonMapping = false;
				environmentMapping = false;
				normalTexture = false;
			}

			if (ImGui::Checkbox("Cylinder  ", &cylinder) && cylinder) {
				scene->type = 2;
				noneType = false;
				plane = false;
				sphere = false;
			}

			ImGui::SameLine();

			if (ImGui::Checkbox("Environment Mapping", &environmentMapping) && environmentMapping) {
				scene->mapping = 2;
				noneMapping = false;
				normalMapping = false;
				toonMapping = false;
				normalTexture = false;
			}

			if (ImGui::Checkbox("Sphere    ", &sphere) && sphere) {
				scene->type = 3;
				noneType = false;
				cylinder = false;
				plane = false;
			}

			ImGui::SameLine();

			if (ImGui::Checkbox("Toon Mapping", &toonMapping) && toonMapping) {
				scene->mapping = 3;
				noneMapping = false;
				normalMapping = false;
				environmentMapping = false;
				normalTexture = false;
			}

			if (ImGui::Checkbox("Normal Texture", &normalTexture) && normalTexture) {
				scene->mapping = 4;
				noneMapping = false;
				normalMapping = false;
				environmentMapping = false;
				toonMapping = false;
			}

			if (toonMapping) {
				ImGui::SliderInt("Number Of Transitions", &scene->transitionsNum, 2, 16);
			}

			ImGui::End();

			if (useKeyboard) {
				ImGui::Begin("KeyBoard Control");
				ImGui::Text("Letters: use \"A\" \"S\" \"D\" \"W\" to move the object");
				ImGui::Text("Letter: use \"M\" to minimize the object");
				ImGui::Text("Letter: use \"B\" to maximize the object");
				ImGui::End();
			}

			for (int i = 0; i < scene->GetModelCount(); i++) {
				if (scene->GetModel(i).GetModelName() == modelName && activeModel) {
					scene->GetModel(i).setColor(ambient,
						diffuse,
						specular);
					scene->SetActiveModelIndex(i);

					modelIndex = i;
				}
				else if (scene->GetModel(i).GetModelName() == modelName && !activeModel) {
					scene->TurnOffActiveModel(i);
				}
			}
			//if we update the object by local or world transformations I did not save the 
			//transformation vectors, if we change the selected object the other object will have the same
			//transformations we did for the old one and if we change back to the old object
			//we'll have a different transformations
			//solved

			if (localTrans && modelIndex != -1) {
				MeshModel& model = scene->GetActiveModel(modelIndex);
				model.localTrans = true;

				ImGui::Begin("Local Transformations", &localTrans);

				ImGui::SliderFloat("Scale x", &model.localScaleVec[0], -1, 10);
				ImGui::SliderFloat("Scale y", &model.localScaleVec[1], -1, 10);
				ImGui::SliderFloat("Scale z", &model.localScaleVec[2], -1, 10);
				ImGui::SliderFloat("Rotate x", &model.localRotateVec[0], -360, 360);
				ImGui::SliderFloat("Rotate y", &model.localRotateVec[1], -360, 360);
				ImGui::SliderFloat("Rotate z", &model.localRotateVec[2], -360, 360);
				ImGui::SliderFloat("Translate x", &model.localTransVec[0], -1, 1);
				ImGui::SliderFloat("Translate y", &model.localTransVec[1], -1, 1);
				ImGui::SliderFloat("Translate z", &model.localTransVec[2], -1, 1);

				model.updateLocalMatrix();
				ImGui::End();

			}
			else if (worldTrans && modelIndex != -1) {
				MeshModel& model = scene->GetActiveModel(modelIndex);
				model.localTrans = false;

				ImGui::Begin("World Transformations", &worldTrans);
				ImGui::SliderFloat("Scale x", &model.worldScaleVec[0], -1, 10);
				ImGui::SliderFloat("Scale y", &model.worldScaleVec[1], -1, 10);
				ImGui::SliderFloat("Scale z", &model.worldScaleVec[2], -1, 10);
				ImGui::SliderFloat("Rotate x", &model.worldRotateVec[0], -360, 360);
				ImGui::SliderFloat("Rotate y", &model.worldRotateVec[1], -360, 360);
				ImGui::SliderFloat("Rotate z", &model.worldRotateVec[2], -360, 360);
				ImGui::SliderFloat("Translate x", &model.worldTransVec[0], -1, 1);
				ImGui::SliderFloat("Translate y", &model.worldTransVec[1], -1, 1);
				ImGui::SliderFloat("Translate z", &model.worldTransVec[2], -1, 1);

				model.updateWorldMatrix();
				ImGui::End();
			}
			else {
				for (int i = 0; i < scene->GetModelCount(); i++) {
					scene->GetModel(i).localTrans = false;
				}
			}
		}
		else {
			for (int i = 0; i < 100; i++) {
				modelName[i] = '\0';
				if (i < 3) {
					ambient[i] = 0;
					diffuse[i] = 0;
					specular[i] = 0;
				}
			}
			ambient[2] = 1.0f;
			useMouse = false;
			useKeyboard = false;
			activeModel = false;
			localTrans = false;
			worldTrans = false;

			for (int i = 0; i < scene->GetModelCount(); i++) {
				scene->GetModel(i).localTrans = false;
			}
		}
		//we need to get for all the active cameras
		if (cameraWindow) {

			ImGui::Begin("Camera Control");
			ImGui::Checkbox("Draw Cameras", &scene->drawCameras);
			ImGui::InputInt("Camera Index", &num, number, number1, false);
			ImGui::Checkbox("Orthographic", &ortho);
			ImGui::Checkbox("Perspective", &persp);
			ImGui::Checkbox("Look At Values", &lookatCB);
			ImGui::Checkbox("Incremental Changes", &incChanges);
			if (incChanges) {
				ImGui::Checkbox("World Transformations", &cameraWorldTrans);
				ImGui::Checkbox("Local Transformations", &cameraLocalTrans);
			}

			if (ImGui::Button("Add Camera")) {
				scene->AddCamera(Utils::LoadCamera("..\\Data\\camera.obj"));
			}
			if (num >= 0 && num < scene->GetCameraCount())
				scene->SetActiveCameraIndex(num);
			else {
				ImGui::Begin("Error Camera Index");
				ImGui::Text("Please enter a valid camera Index from 0 to %d", scene->GetCameraCount() - 1);
				ImGui::End();
			}
			ImGui::End();
			if (ortho && scene->GetActiveCameraIndex() != -1) {
				ImGui::Begin("Orthographic buttons");
				ImGui::SliderFloat("bottom", &scene->GetActiveCamera().bottom, -10, 10);
				ImGui::SliderFloat("top", &scene->GetActiveCamera().top, -10, 10);
				ImGui::SliderFloat("left", &scene->GetActiveCamera().left, -10, 10);
				ImGui::SliderFloat("right", &scene->GetActiveCamera().right, -10, 10);
				ImGui::SliderFloat("near", &scene->GetActiveCamera().nearr, -100, 100);
				ImGui::SliderFloat("far", &scene->GetActiveCamera().farr, -100, 100);
				//ImGui::SliderFloat("far", &scene->GetActiveCamera()., -100, 100);
				scene->GetActiveCamera().updateOrth();
				ImGui::End();
			}
			else if (persp && scene->GetActiveCameraIndex() != -1) {
				fov = glm::degrees(scene->GetActiveCamera().fovy);
				dollyTrans = scene->GetActiveCamera().worldTransVec[2];
				scene->GetActiveCamera().isItPers = true;
				ImGui::Begin("Perspective buttons");
				ImGui::SliderFloat("Fovy", &fov, -180, 180);
				ImGui::SliderFloat("aspect", &scene->GetActiveCamera().aspect, 0, 10);
				ImGui::SliderFloat("near", &scene->GetActiveCamera().pernear, -10, 10);
				ImGui::SliderFloat("far", &scene->GetActiveCamera().perfar, -10, 10);
				scene->GetActiveCamera().fovy = glm::radians(fov);
				scene->GetActiveCamera().updatePers();
				if (ImGui::SliderFloat("Dolly Zoom", &dollyTrans, 0, 20)) {
					scene->GetActiveCamera().worldTransVec[2] = dollyTrans;
					scene->GetActiveCamera().updateDolly();
					scene->GetActiveCamera().updateWorldTransformation();
				}

				ImGui::End();
			}
			if (!persp && scene->GetActiveCameraIndex() != -1)
				scene->GetActiveCamera().isItPers = false;
			if (incChanges) {
				if (cameraWorldTrans && scene->GetActiveCameraIndex() != -1) {
					ImGui::Begin("Camera Translate");
					ImGui::SliderFloat("Translate x", &scene->GetActiveCamera().worldTransVec[0], -1, 1);
					ImGui::SliderFloat("Translate y", &scene->GetActiveCamera().worldTransVec[1], -1, 1);
					ImGui::SliderFloat("Translate z", &scene->GetActiveCamera().worldTransVec[2], -1, 1);

					ImGui::SliderFloat("Rotate x", &scene->GetActiveCamera().worldRotateVec[0], -360, 360);
					ImGui::SliderFloat("Rotate y", &scene->GetActiveCamera().worldRotateVec[1], -360, 360);
					ImGui::SliderFloat("Rotate z", &scene->GetActiveCamera().worldRotateVec[2], -360, 360);
					ImGui::End();

					scene->GetActiveCamera().updateWorldTransformation();
				}
				if (cameraLocalTrans && scene->GetActiveCameraIndex() != -1) {
					ImGui::Begin("Camera Translate");
					ImGui::SliderFloat("Translate x", &scene->GetActiveCamera().localTransVec[0], -1, 1);
					ImGui::SliderFloat("Translate y", &scene->GetActiveCamera().localTransVec[1], -1, 1);
					ImGui::SliderFloat("Translate z", &scene->GetActiveCamera().localTransVec[2], -1, 1);

					ImGui::SliderFloat("Rotate x", &scene->GetActiveCamera().localRotateVec[0], -360, 360);
					ImGui::SliderFloat("Rotate y", &scene->GetActiveCamera().localRotateVec[1], -360, 360);
					ImGui::SliderFloat("Rotate z", &scene->GetActiveCamera().localRotateVec[2], -360, 360);
					ImGui::End();

					scene->GetActiveCamera().updateLocalTransformation();
				}
			}

			if (lookatCB) {
				ImGui::SliderFloat("Eye X Value", &scene->GetActiveCamera().eye.x, -30, 30);
				ImGui::SliderFloat("Eye Y Value", &scene->GetActiveCamera().eye.y, -30, 30);
				ImGui::SliderFloat("Eye Z Value", &scene->GetActiveCamera().eye.z, -30, 30);
			}
		}

	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}
