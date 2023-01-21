#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth();
	int GetViewportHeight();
	void updateViewport();
	void setViewportWidth(int width);
	void setViewportHeight(int height);
	void drawWorldAxies(Camera camera);
	void drawModelAxies(Scene& scene, MeshModel meshModel);
	void drawBoudingBox(Scene& scene, MeshModel meshModel, bool isLocal);
	void drawCameras(Scene& scene);
	void DrawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, glm::vec3& c1, glm::vec3& c2, glm::vec3& c3, MeshModel* meshModel = NULL, Scene& scene = Scene());

	void raterizeTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::fvec3& c1, glm::fvec3& c2, glm::fvec3& c3, bool flag, MeshModel* meshModel);
	void PutPixelCheck(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int x, int y, glm::vec3& color1, glm::vec3& color2, glm::vec3& color3, int type);
	float findDepth(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int x, int y, float& A, float& A1, float& A2, float& A3);
	void drawAllLights(Scene& s);
	glm::fvec3 computeColor(vector<Light> lights, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 middleOfFace, glm::vec3 normal, glm::vec3 cameraPos);
	void showReflection(glm::vec3 r, glm::vec3 middleOfFace, glm::vec3 pos);
private:
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
	void DrawCircle(int a, int r, int x0, int y0, const glm::vec3& color);
	void DrawFlower();

	void CreateBuffers(int w, int h);
	void CreateOpenglBuffer();
	void InitOpenglRendering();

	float** zBuffer;
	float* color_buffer = NULL;
	int viewport_width;
	int viewport_height;
	GLuint gl_screen_tex;
	GLuint gl_screen_vtc;

	int z1 = 1, z2 = 1, maxZ = 1, minZ = 0;
	bool isZbuff = false;
	bool reflect_direction = false;
	glm::fvec3* normal1 = NULL, *normal2 = NULL, *normal3 = NULL;

	vector<Light> lights;
	glm::fvec3 ambient, diffuse, specular, cameraPos;
};
