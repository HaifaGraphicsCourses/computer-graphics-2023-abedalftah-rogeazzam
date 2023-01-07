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
	void DrawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& color, MeshModel* meshModel = NULL);

	void raterizeTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color, bool flag, MeshModel* meshModel);
	void PutPixelCheck(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int x, int y, glm::vec3 color, bool showColorbuff);
	float findDepth(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int x, int y);
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
};
