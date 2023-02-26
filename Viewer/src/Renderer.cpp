#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <random>
#include "Renderer.h"
#include "InitShader.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::Render(const std::shared_ptr<Scene>& scene)
{
	if (scene->GetActiveCameraIndex() == -1)
		return;
	vector<int> meshModel = scene->GetActiveModelsIndexes();

	for (int i = 0; i < meshModel.size(); i++) {
		colorShader.use();
		// Set the uniform variables
		colorShader.setUniform("model", scene->GetActiveModel(meshModel[i]).transformationMat());
		colorShader.setUniform("view", scene->GetActiveCamera().GetViewTransformation());
		colorShader.setUniform("projection", scene->GetActiveCamera().GetProjectionTransformation());
		colorShader.setUniform("material.textureMap", 0);
		colorShader.setUniform("material.environmentMap", 0);
		colorShader.setUniform("material.ambient", scene->GetActiveModel(meshModel[i]).getAmbient());
		colorShader.setUniform("material.diffuse", scene->GetActiveModel(meshModel[i]).getDiffuse());
		colorShader.setUniform("material.specular", scene->GetActiveModel(meshModel[i]).getSpecular());
		colorShader.setUniform("transitionsNum", scene->transitionsNum);
		colorShader.setUniform("type", scene->type);
		colorShader.setUniform("map", scene->mapping);
		colorShader.setUniform("max_z", scene->GetActiveModel(meshModel[i]).max_z);
		colorShader.setUniform("mid", scene->GetActiveModel(meshModel[i]).mid);
		if (scene->GetActiveLightIndex() != -1) {
			colorShader.setUniform("light.ambient", scene->getActiveLight().ambient);
			colorShader.setUniform("light.diffuse", scene->getActiveLight().diffuse);
			colorShader.setUniform("light.specular", scene->getActiveLight().specular);
			colorShader.setUniform("light.pos", scene->getActiveLight().pos);
		}
		colorShader.setUniform("camera_pos", scene->GetActiveCamera().eye);

		texture1.bind(0);
		// Drag our model's faces (triangles) in fill mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(scene->GetActiveModel(meshModel[i]).vao);
		glDrawArrays(GL_TRIANGLES, 0, scene->GetActiveModel(meshModel[i]).modelVertices.size());
		glBindVertexArray(0);
		texture1.unbind(0);
	}

	if (scene->mapping == 2) {
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		colorShader.use();
		colorShader.setUniform("model", glm::mat4(1.0f));
		colorShader.setUniform("view", glm::mat4(glm::mat3(scene->GetActiveCamera().GetViewTransformation())));
		colorShader.setUniform("projection", scene->GetActiveCamera().GetProjectionTransformation());
		colorShader.setUniform("material.textureMap", 0);
		colorShader.setUniform("material.environmentMap", 0);
		colorShader.setUniform("map", 5);

		// skybox cube
		glBindVertexArray(skyboxVAO);
		texture2.bind(cubeMapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		texture2.unbind(cubeMapTexture);
		glDepthFunc(GL_LESS);
	}
}

void Renderer::LoadShaders()
{
	colorShader.loadShaders("vshader.glsl", "fshader.glsl");
}

void Renderer::LoadTextures()
{
	texture1.loadTexture("..\\Data\\wall.jpeg", true);

	float skyboxVertices[] = {
		// positions          
		-10.0f,  10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,
		 10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		 10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		-10.0f,  10.0f, -10.0f,
		 10.0f,  10.0f, -10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		 10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		 10.0f, -10.0f,  10.0f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	
	vector<std::string> faces =	{
		"..\\Data\\posx.jpg",
		"..\\Data\\negx.jpg",
		"..\\Data\\posy.jpg",
		"..\\Data\\negy.jpg",
		"..\\Data\\posz.jpg",
		"..\\Data\\negz.jpg"
	};
	cubeMapTexture = texture2.loadCubemap(faces);
}