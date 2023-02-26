#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <Texture2D.h>
#include <ShaderProgram.h>

class Renderer
{
public:
	Renderer();
	virtual ~Renderer();
	void Render(const std::shared_ptr<Scene>& scene);

	void drawAllLights(Scene& s);
	void LoadShaders();
	void LoadTextures();

private:
	ShaderProgram textureShader;
	GLuint cubeMapTexture;
	GLuint cubeVAO, cubeVBO, cubeEBO;
	GLuint skyboxVAO, skyboxVBO;
	Texture2D texture2;
	Texture2D texture3;
	Texture2D texture4;
	ShaderProgram shader;
	ShaderProgram colorShader;
	Texture2D texture1;
};