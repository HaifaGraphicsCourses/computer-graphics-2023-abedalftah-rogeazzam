#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/ext/matrix_transform.hpp>

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"
#include <iostream>

/**
 * Fields
 */
bool show_demo_window = false;
bool show_another_window = false;
bool useMouse = false;
bool useKeyboard = false;
static bool localTrans = false;
static bool worldTrans = false;
static char modelName[100];
static double mouseX = 640;
static double mouseY = 360;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
glm::vec4 model_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.00f);

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char **argv)
{
	int windowWidth = 1280, windowHeight = 720;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	//glm::fmat4x4 worldMatrix = glm::fmat4(1.0f);
	//glm::fmat4x4 w = glm::translate(worldMatrix, glm::vec3(5, 5, 5));
	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 4; j++) {
	//		std::cout << w[i][j] << " ";
	//	}
	//	std::cout << std::endl;
	//}

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();


	//we changed the private fields in meshmodel to public
	//the solution of the first question




	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);
		RenderFrame(window, scene, renderer, io);
    }

	Cleanup(window);
    return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	
	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);


	
	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		// TODO: Set new aspect ratio
	}

	if (!io.WantCaptureKeyboard && useKeyboard)
	{
		
		// TODO: Handle keyboard events here
		vector<int> indexes = scene.GetActiveModelsIndexes();
		if (io.KeysDown['B'])
		{
			for (int i = 0; i < indexes.size(); i++) {
				if (scene.GetActiveModel(indexes[i]).GetModelName() == modelName) {
					if (localTrans) {
						glm::fvec3 s = scene.GetActiveModel(indexes[i]).localScaleVec;
						scene.GetActiveModel(indexes[i]).localScaleVec = glm::fvec3(s[0] + 1.5, s[1] + 1.5, s[2] + 1.5);
						scene.GetActiveModel(indexes[i]).updateLocalMatrix();
					}
					else if (worldTrans) {
						glm::fvec3 s = scene.GetActiveModel(indexes[i]).worldScaleVec;
						scene.GetActiveModel(indexes[i]).worldScaleVec = glm::fvec3(s[0] + 1.2, s[1] + 1.2, s[2] + 1.2);
						scene.GetActiveModel(indexes[i]).updateWorldMatrix();
					}
				}
			}
		}
		else if (io.KeysDown['M'])
		{
			for (int i = 0; i < indexes.size(); i++) {
				if (scene.GetActiveModel(indexes[i]).GetModelName() == modelName) {
					if (localTrans) {
						glm::fvec3 s = scene.GetActiveModel(indexes[i]).localScaleVec;
						if (s[0] - 0.2 >= 0) {
							scene.GetActiveModel(indexes[i]).localScaleVec = glm::fvec3(s[0] - 0.2, s[1] - 0.2, s[2] - 0.2);
							scene.GetActiveModel(indexes[i]).updateLocalMatrix();
						}
					}
					else if (worldTrans) {
						glm::fvec3 s = scene.GetActiveModel(indexes[i]).worldScaleVec;
						if (s[0] - 0.2 >= 0) {
							scene.GetActiveModel(indexes[i]).worldScaleVec = glm::fvec3(s[0] - 0.2, s[1] - 0.2, s[2] - 0.2);
							scene.GetActiveModel(indexes[i]).updateWorldMatrix();
						}
					}
				}
			}
		}
		else if (io.KeysDown['W'])
		{
			for (int i = 0; i < indexes.size(); i++) {
				if (scene.GetActiveModel(indexes[i]).GetModelName() == modelName) {
					if (localTrans) {
						glm::fvec3 s = scene.GetActiveModel(indexes[i]).localTransVec;
						scene.GetActiveModel(indexes[i]).localTransVec = glm::fvec3(s[0], s[1] + 10, s[2]);
						scene.GetActiveModel(indexes[i]).updateLocalMatrix();
					}
					else if (worldTrans) {
						glm::fvec3 s = scene.GetActiveModel(indexes[i]).worldTransVec;
						scene.GetActiveModel(indexes[i]).worldTransVec = glm::fvec3(s[0], s[1] + 10, s[2]);
						scene.GetActiveModel(indexes[i]).updateWorldMatrix();
					}
				}
			}
		}
		else if (io.KeysDown['D'])
		{
			for (int i = 0; i < indexes.size(); i++) {
				if (scene.GetActiveModel(indexes[i]).GetModelName() == modelName) {
					if (localTrans) {
						glm::fvec3 s = scene.GetActiveModel(indexes[i]).localTransVec;
						scene.GetActiveModel(indexes[i]).localTransVec = glm::fvec3(s[0] + 10, s[1], s[2]);
						scene.GetActiveModel(indexes[i]).updateLocalMatrix();
					}
					else if (worldTrans) {
						glm::fvec3 s = scene.GetActiveModel(indexes[i]).worldTransVec;
						scene.GetActiveModel(indexes[i]).worldTransVec = glm::fvec3(s[0] + 10, s[1], s[2]);
						scene.GetActiveModel(indexes[i]).updateWorldMatrix();
					}
				}
			}
		}
		else if (io.KeysDown['A']) {
			for (int i = 0; i < indexes.size(); i++) {
				if (scene.GetActiveModel(indexes[i]).GetModelName() == modelName) {
					if (localTrans) {
						glm::fvec3 s = scene.GetActiveModel(indexes[i]).localTransVec;
						scene.GetActiveModel(indexes[i]).localTransVec = glm::fvec3(s[0] - 10, s[1], s[2]);
						scene.GetActiveModel(indexes[i]).updateLocalMatrix();
					}
					else if (worldTrans) {
						glm::fvec3 s = scene.GetActiveModel(indexes[i]).worldTransVec;
						scene.GetActiveModel(indexes[i]).worldTransVec = glm::fvec3(s[0] - 10, s[1], s[2]);
						scene.GetActiveModel(indexes[i]).updateWorldMatrix();
					}
				}
			}
		}
		else if (io.KeysDown['S']) {
			for (int i = 0; i < indexes.size(); i++) {
				if (scene.GetActiveModel(indexes[i]).GetModelName() == modelName) {
					if (localTrans) {
						glm::fvec3 s = scene.GetActiveModel(indexes[i]).localTransVec;
						scene.GetActiveModel(indexes[i]).localTransVec = glm::fvec3(s[0], s[1] - 10, s[2]);
						scene.GetActiveModel(indexes[i]).updateLocalMatrix();
					}
					else if (worldTrans) {
						glm::fvec3 s = scene.GetActiveModel(indexes[i]).worldTransVec;
						scene.GetActiveModel(indexes[i]).worldTransVec = glm::fvec3(s[0], s[1] - 10, s[2]);
						scene.GetActiveModel(indexes[i]).updateWorldMatrix();
					}
				}
			}
		}
	}
	
	if (!io.WantCaptureMouse && useMouse)
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

		if (io.MouseDown[0])
		{
			vector<int> indexes = scene.GetActiveModelsIndexes();
			for (int i = 0; i < indexes.size(); i++) {
				if (scene.GetActiveModel(indexes[i]).GetModelName() == modelName) {
					if (localTrans) {
						glm::fvec3 s = scene.GetActiveModel(indexes[i]).localTransVec;
						scene.GetActiveModel(indexes[i]).localTransVec = glm::fvec3(io.MousePos[0] - mouseX, mouseY - io.MousePos[1], s[2]);
						scene.GetActiveModel(indexes[i]).updateLocalMatrix();
					}
					else if (worldTrans) {
						glm::fvec3 s = scene.GetActiveModel(indexes[i]).worldTransVec;
						scene.GetActiveModel(indexes[i]).localTransVec = glm::fvec3(io.MousePos[0] - mouseX, mouseY - io.MousePos[1], s[2]);
						scene.GetActiveModel(indexes[i]).updateWorldMatrix();
					}
				}
			}
		}

	}

	renderer.ClearColorBuffer(clear_color);
	renderer.Render(scene);
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	/**
	 * MeshViewer menu
	 */
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
					scene.AddModel(Utils::LoadMeshModel(outPath));
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

	// Controls
	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	ImGui::ColorEdit3("model Color", (float*)&model_color);
	// TODO: Add more controls as needed
	
	ImGui::End();

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{

		static float f = 0.0f;
		static float g = 0.0f;
		static int counter = 0;

		
		static bool activeModel = false;
		int modelIndex = -1;

		static bool transWindow = false;

		

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);
		ImGui::Checkbox("Object Control Window", &transWindow);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		if (transWindow) {
			ImGui::Begin("Object Control");
			ImGui::InputText("Which model? ", modelName, 100);
			ImGui::Checkbox("Turn on/off the model", &activeModel);
			ImGui::Checkbox("Move Obj by Mouse", &useMouse);
			ImGui::Checkbox("Move Obj by keyBoard", &useKeyboard);
			ImGui::ColorEdit3("model color", (float*)&model_color); // Edit 3 floats representing a color
			ImGui::Checkbox("Local Transformations", &localTrans);
			ImGui::Checkbox("World Transformations", &worldTrans);
			ImGui::End();

			if (useKeyboard) {
				ImGui::Begin("KeyBoard Control");
				ImGui::Text("Letters: use \"A\" \"S\" \"D\" \"W\" to move the object");
				ImGui::Text("Letter: use \"M\" to minimize the object");
				ImGui::Text("Letter: use \"B\" to maximize the object");
				ImGui::End();
			}

			for (int i = 0; i < scene.GetModelCount(); i++) {
				if (scene.GetModel(i).GetModelName() == modelName && activeModel) {
					scene.GetModel(i).setColor(glm::vec3(model_color[0], model_color[1], model_color[2]));
					scene.SetActiveModelIndex(i);
					modelIndex = i;
				}
				else if (scene.GetModel(i).GetModelName() == modelName && !activeModel) {
					scene.TurnOffActiveModel(i);
				}
			}

			//if we update the object by local or world transformations I did not save the 
			//transformation vectors, if we change the selected object the other object will have the same
			//transformations we did for the old one and if we change back to the old object
			//we'll have a different transformations
			//solved

			if (localTrans && modelIndex != -1) {
				MeshModel& model = scene.GetActiveModel(modelIndex);
				model.localTrans = true;

				ImGui::Begin("Local Transformations", &localTrans);
				
				ImGui::SliderFloat("Scale x", &model.localScaleVec[0], 0.01, 10);
				ImGui::SliderFloat("Scale y", &model.localScaleVec[1], 0.01, 10);
				ImGui::SliderFloat("Scale z", &model.localScaleVec[2], 0.01, 10);
				ImGui::SliderFloat("Rotate x", &model.localRotateVec[0], -360, 360);
				ImGui::SliderFloat("Rotate y", &model.localRotateVec[1], -360, 360);
				ImGui::SliderFloat("Rotate z", &model.localRotateVec[2], -360, 360);
				ImGui::SliderFloat("Translate x", &model.localTransVec[0], -720, 720);
				ImGui::SliderFloat("Translate y", &model.localTransVec[1], -1280, 1280);
				ImGui::SliderFloat("Translate z", &model.localTransVec[2], -720, 720);

				model.updateLocalMatrix();
				ImGui::End();

			}
			else if (worldTrans && modelIndex != -1) {
				MeshModel& model = scene.GetActiveModel(modelIndex);
				model.localTrans = false;

				ImGui::Begin("World Transformations", &localTrans);
				ImGui::SliderFloat("Scale x", &model.worldScaleVec[0], 1, 100);
				ImGui::SliderFloat("Scale y", &model.worldScaleVec[1], 1, 100);
				ImGui::SliderFloat("Scale z", &model.worldScaleVec[2], 1, 100);
				ImGui::SliderFloat("Rotate x", &model.worldRotateVec[0], -360, 360);
				ImGui::SliderFloat("Rotate y", &model.worldRotateVec[1], -360, 360);
				ImGui::SliderFloat("Rotate z", &model.worldRotateVec[2], -360, 360);
				ImGui::SliderFloat("Translate x", &model.worldTransVec[0], 0, 100);
				ImGui::SliderFloat("Translate y", &model.worldTransVec[1], 0, 100);
				ImGui::SliderFloat("Translate z", &model.worldTransVec[2], 0, 100);

				model.updateWorldMatrix();
				ImGui::End();
			}
			else {
				for (int i = 0; i < scene.GetModelCount(); i++) {
					scene.GetModel(i).localTrans = false;					
				}
			}
		}
		else {
			for (int i = 0; i < 100; i++) {
				modelName[i] = '\0';
				if (i < 3)
					model_color[i] = 0;
			}
			useMouse = false;
			useKeyboard = false;
			activeModel = false;
			localTrans = false;
			worldTrans = false;

			for (int i = 0; i < scene.GetModelCount(); i++) {
				scene.GetModel(i).localTrans = false;
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