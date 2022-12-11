#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>
#include "Renderer.h"
#include "InitShader.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width),
	viewport_height(viewport_height)
{
	InitOpenglRendering();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width) return;
	if (j < 0) return; if (j >= viewport_height) return;

	color_buffer[INDEX(viewport_width, i, j, 0)] = color.x;
	color_buffer[INDEX(viewport_width, i, j, 1)] = color.y;
	color_buffer[INDEX(viewport_width, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	// TODO: Implement bresenham algorithm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

	int dx = abs(p2[0] - p1[0]);
	int dy = abs(p2[1] - p1[1]);

	int e = 2 * dy - dx;

	int x1 = p1[0];
	int y1 = p1[1];

	int x2 = p2[0];
	int y2 = p2[1];

	bool trueFalse = true; //Tells us if slope(m) > 1 or < 1

	if (dx < dy) { //If the slope(m) > 1 then we need to just modify the points (to swap each x with y) and change the error  
		e = 2 * dx - dy;

		int temp = dx;
		dx = dy;
		dy = temp;

		temp = y1;
		y1 = x1;
		x1 = temp;

		temp = y2;
		y2 = x2;
		x2 = temp;



		trueFalse = false;
	}

	for (int i = 0; i <= dx; i++) {
		if (x1 < x2) x1++; else x1--;

		if (e < 0) {
			if (trueFalse) //tells us if we're in the second situation
				PutPixel(x1, y1, color);
			else
				PutPixel(y1, x1, color);
			e += 2 * dy;
		}
		else {
			if (y1 < y2) y1++; else y1--;

			if (trueFalse)//tells us if we're in the second situation
				PutPixel(x1, y1, color);
			else
				PutPixel(y1, x1, color);

			e += 2 * dy - 2 * dx;
		}
	}

}

void Renderer::drawWorldAxies(Camera camera)
{
	//we're not sure that we need to translate the world axies
	glm::mat4x4 mat = camera.GetViewTransformation();
	glm::fvec4 worldAxies = mat * glm::fvec4(0, 0, 0, 1);
	glm::fvec4 worldX = mat * glm::fvec4(viewport_width + worldAxies[0], 0, 0, 1);
	glm::fvec4 worldY = mat * glm::fvec4(0, viewport_height + worldAxies[1], 0, 1);
	glm::fvec4 worldZ = mat * glm::fvec4(0, 0, viewport_width + worldAxies[0], 1);

	worldAxies.x /= worldAxies.w;
	worldAxies.y /= worldAxies.w;
	worldAxies.z /= worldAxies.w;

	worldX.x /= worldX.w;
	worldX.y /= worldX.w;
	worldX.z /= worldX.w;

	worldY.x /= worldY.w;
	worldY.y /= worldY.w;
	worldY.z /= worldY.w;

	DrawLine(worldAxies, worldX, glm::fvec3(0.3, 0.3, 1));
	DrawLine(worldAxies, worldY, glm::fvec3(0.8, 0, 0.8));
	DrawLine(worldAxies, worldZ, glm::fvec3(0.1, 0.9, 0.5));
}

void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenglBuffer(); //Do not remove this line.
	if (color_buffer)
		delete[] color_buffer;
	color_buffer = new float[3 * w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenglRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[] = {
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[] = {
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1 };

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vtc));

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"), 0);
}

void Renderer::CreateOpenglBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width, viewport_height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width, viewport_height);
}

void Renderer::SwapBuffers()
{

	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width, viewport_height, GL_RGB, GL_FLOAT, color_buffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{

	for (int i = 0; i < viewport_width; i++)
	{
		for (int j = 0; j < viewport_height; j++)
		{
			PutPixel(i, j, color);
		}
	}
}

void Renderer::DrawCircle(int a, int r, int half_width, int half_height, const glm::vec3& color)
{
	glm::vec2 p1 = glm::ivec2(0, viewport_height);
	glm::vec2 p2 = glm::ivec2(viewport_width / 2, viewport_height / 2);

	glm::vec2 q2 = glm::ivec2(viewport_width, viewport_height);
	glm::vec2 q1 = glm::ivec2(viewport_width / 2, viewport_height / 2);
	double pi = 2 * acos(0.0);

	for (int i = 0; i < a; i++) {
		DrawLine(glm::ivec2(half_width, half_height), glm::ivec2(half_width + (r * sin((2 * pi * i) / a)),
			half_height + (r * cos((2 * pi * i) / a))), color);
	}
}

void Renderer::drawModelAxies(Scene& scene, MeshModel meshModel)
{
	//A function to draw the model axies
	//here we only move the bouding box by world transformations or camera transformations
	glm::mat4x4 mat = meshModel.worldTransMat();
	glm::mat4x4 mat1 = glm::mat4x4(1.0f);

	if (scene.GetActiveCameraIndex() != -1)
		mat1 = scene.GetActiveCamera().GetViewTransformation();

	mat = mat1 * mat;

	glm::fvec4 modelAxies = mat * glm::fvec4(0, 0, 0, 1);
	glm::fvec4 localX = mat * glm::fvec4(80, 0, 0, 1);
	glm::fvec4 localY = mat * glm::fvec4(0, 80, 0, 1);
	glm::fvec4 localZ = mat * glm::fvec4(0, 0, 80, 1);

	DrawLine(modelAxies, localX, glm::fvec3(0, 0, 0));
	DrawLine(modelAxies, localY, glm::fvec3(0, 1, 1));
	DrawLine(modelAxies, localZ, glm::fvec3(1, 0, 1));
}

void Renderer::DrawFlower()
{
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
	int frac1 = viewport_width * 7 / 8, frac2 = viewport_width * 6.5 / 8;

	for (int i = 0; i < 50; i++) {
		DrawLine(glm::ivec2(frac1 + i, 0), glm::ivec2(frac2 + i, half_height - 180),
			glm::vec3(1.0f, 0.5f, 0.31f));

		DrawLine(glm::ivec2(frac2 + i, half_height - 180), glm::ivec2(half_width, half_height),
			glm::vec3(1.0f, 0.5f, 0.31f));
	}

	DrawCircle(21600, 200, half_width - 150, half_height - 150, glm::vec3(1.0f, 0.5f, 0.31f));

	DrawCircle(21600, 70, half_width - 150, half_height - 150, glm::vec3(1.0f, 0.5f, 0.31f));
	DrawCircle(21600, 70, half_width - 200, half_height, glm::vec3(1.0f, 0.5f, 0.31f));
	DrawCircle(21600, 70, half_width - 150, half_height + 150, glm::vec3(1.0f, 0.5f, 0.31f));
	DrawCircle(21600, 70, half_width, half_height - 200, glm::vec3(1.0f, 0.5f, 0.31f));
	DrawCircle(21600, 70, half_width, half_height + 200, glm::vec3(1.0f, 0.5f, 0.31f));
	DrawCircle(21600, 70, half_width + 150, half_height + 150, glm::vec3(1.0f, 0.5f, 0.31f));
	DrawCircle(21600, 70, half_width + 200, half_height, glm::vec3(1.0f, 0.5f, 0.31f));
	DrawCircle(21600, 70, half_width + 150, half_height - 150, glm::vec3(1.0f, 0.5f, 0.31f));
	DrawCircle(21600, 200, half_width, half_height, glm::vec4(0.2f, 0.13f, 0.5f, 1.00f));
}


void Renderer::drawBoudingBox(Scene& scene, MeshModel meshModel)
{
	//A function to draw the bouding box
	//here we only move the bouding box by world transformations or camera transformations
	glm::mat4x4 mat = meshModel.worldTransMat();
	glm::mat4x4 mat1 = glm::mat4x4(1.0f);

	if (scene.GetActiveCameraIndex() != -1)
		mat1 = scene.GetActiveCamera().GetViewTransformation();

	mat = mat1 * mat;
	glm::fvec4 f0 = mat * glm::fvec4(meshModel.max_x, meshModel.max_y, meshModel.max_z, 1);

	glm::fvec4 f1 = mat * glm::fvec4(meshModel.max_x, meshModel.max_y, meshModel.min_z, 1);

	glm::fvec4 f4 = mat * glm::fvec4(meshModel.min_x, meshModel.max_y, meshModel.max_z, 1);

	glm::fvec4 f7 = mat * glm::fvec4(meshModel.min_x, meshModel.max_y, meshModel.min_z, 1);


	glm::fvec4 f6 = mat * glm::fvec4(meshModel.min_x, meshModel.min_y, meshModel.min_z, 1);

	glm::fvec4 f8 = mat * glm::fvec4(meshModel.min_x, meshModel.min_y, meshModel.max_z, 1);

	glm::fvec4 f2 = mat * glm::fvec4(meshModel.max_x, meshModel.min_y, meshModel.min_z, 1);

	glm::fvec4 f3 = mat * glm::fvec4(meshModel.max_x, meshModel.min_y, meshModel.max_z, 1);

	//we connect every coordinate with the neighbors
	DrawLine(f0, f1, glm::vec3(1, 1, 1));
	DrawLine(f0, f4, glm::vec3(1, 1, 1));
	DrawLine(f0, f3, glm::vec3(1, 1, 1));

	DrawLine(f7, f1, glm::vec3(1, 1, 1));
	DrawLine(f7, f4, glm::vec3(1, 1, 1));
	DrawLine(f7, f6, glm::vec3(1, 1, 1));

	DrawLine(f2, f3, glm::vec3(1, 1, 1));
	DrawLine(f2, f1, glm::vec3(1, 1, 1));
	DrawLine(f6, f2, glm::vec3(1, 1, 1));

	DrawLine(f8, f4, glm::vec3(1, 1, 1));
	DrawLine(f8, f3, glm::vec3(1, 1, 1));
	DrawLine(f8, f6, glm::vec3(1, 1, 1));
}

void Renderer::drawCameras(Scene& scene)
{
	//A function to draw the active cameras
	std::vector<Face> faces = scene.GetActiveCamera().getFaces();
	std::vector<glm::fvec3> vertices = scene.GetActiveCamera().getVertices();
	glm::mat4x4 mat = scene.GetActiveCamera().GetWorldTransformations();
	for (int j = 0; j < faces.size(); j++) {
		int v1 = faces[j].GetVertexIndex(0) - 1;
		int v2 = faces[j].GetVertexIndex(1) - 1;
		int v3 = faces[j].GetVertexIndex(2) - 1;

		glm::vec3 cords[] = { vertices.at(v1), vertices.at(v2), vertices.at(v3) };

		glm::vec4 transformP1 = mat * glm::vec4(cords[0], 1);
		cords[0].x = transformP1.x;
		cords[0].y = transformP1.y;
		cords[0].z = transformP1.z;

		cords[0].z = transformP1.w;

		glm::vec4 transformP2 = mat * glm::vec4(cords[1], 1);
		cords[1].x = transformP2.x;
		cords[1].y = transformP2.y;
		cords[1].z = transformP2.z;
		cords[1].z = transformP2.w;


		glm::vec4 transformP3 = mat * glm::vec4(cords[2], 1);
		cords[2].x = transformP3.x;
		cords[2].y = transformP3.y;
		cords[2].z = transformP3.z;

		cords[2].z = transformP3.w;

		if (cords[0].x > cords[1].x)
			DrawTriangle(cords[2], cords[1], cords[0], glm::fvec3(0));
		else if (cords[2].x > cords[0].x)
			DrawTriangle(cords[1], cords[0], cords[2], glm::fvec3(0));
		else
			DrawTriangle(cords[0], cords[2], cords[1], glm::fvec3(0));
	}
}


void Renderer::Render(const Scene& scene)

{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
	//DrawFlower();

	if (scene.GetActiveCameraIndex() != -1 && scene.GetActiveCamera().drawWorldAxisFlag)
		drawWorldAxies(scene.GetActiveCamera());
	if (scene.drawCameras) {
		drawCameras(scene);
	}

	vector<MeshModel> meshModel = scene.GetActiveModels(scene.GetActiveModelsIndexes());

	for (int i = 0; i < meshModel.size(); i++) {
		std::vector<Face> faces = meshModel[i].getFaces();
		std::vector<glm::vec3> vertices = meshModel[i].getVertices();

		//we need to multiply by camera transformation
		glm::mat4x4 mat1 = glm::mat4x4(1.0f);
		// && scene.GetActiveModel(scene.getActiveModelToWork()).GetModelName() == meshModel[i].GetModelName()
		//we need to find a solution that more than one camera can be and every camera can take a model and change only this model
		if (scene.GetActiveCameraIndex() != -1)
			mat1 = scene.GetActiveCamera().GetViewTransformation();

		glm::mat4x4 mat = meshModel[i].transformationMat();
		mat = mat1 * mat;
		for (int j = 0; j < faces.size(); j++) {
			int v1 = faces[j].GetVertexIndex(0) - 1;
			int v2 = faces[j].GetVertexIndex(1) - 1;
			int v3 = faces[j].GetVertexIndex(2) - 1;

			glm::vec3 cords[] = { vertices.at(v1), vertices.at(v2), vertices.at(v3) };

			glm::vec4 transformP1 = mat * glm::vec4(cords[0], 1);

			if (transformP1.w != 0) {
				cords[0].x = transformP1.x / transformP1.w;
				cords[0].y = transformP1.y / transformP1.w;
				cords[0].z = transformP1.z / transformP1.w;
			}
			else {
				cords[0].x = transformP1.x;
				cords[0].y = transformP1.y;
				cords[0].z = transformP1.z;
			}

			glm::vec4 transformP2 = mat * glm::vec4(cords[1], 1);
			if (transformP2.w != 0) {
				cords[1].x = transformP2.x / transformP2.w;
				cords[1].y = transformP2.y / transformP2.w;
				cords[1].z = transformP2.z / transformP2.w;
			}
			else {
				cords[1].x = transformP2.x;
				cords[1].y = transformP2.y;
				cords[1].z = transformP2.z;
			}
			glm::vec4 transformP3 = mat * glm::vec4(cords[2], 1);
			if (transformP3.w != 0) {
				cords[2].x = transformP3.x / transformP3.w;
				cords[2].y = transformP3.y / transformP3.w;
				cords[2].z = transformP3.z / transformP3.w;
			}
			else {
				cords[2].x = transformP3.x;
				cords[2].y = transformP3.y;
				cords[2].z = transformP3.z;
			}

			if (cords[0].x > cords[1].x)
				DrawTriangle(cords[2], cords[1], cords[0], meshModel[i].getColor());
			else if (cords[2].x > cords[0].x)
				DrawTriangle(cords[1], cords[0], cords[2], meshModel[i].getColor());
			else
				DrawTriangle(cords[0], cords[2], cords[1], meshModel[i].getColor());

			if (meshModel[i].vertexNormals) {
				glm::vec3 normal1 = meshModel[i].GetNormal(faces[j].GetNormalIndex(0) - 1);
				glm::vec3 normal2 = meshModel[i].GetNormal(faces[j].GetNormalIndex(1) - 1);
				glm::vec3 normal3 = meshModel[i].GetNormal(faces[j].GetNormalIndex(2) - 1);
				glm::vec2 distance0 = glm::vec2(cords[0].x + (normal1.x * 40), cords[0].y + (normal1.y * 40));
				glm::vec2 distance1 = glm::vec2(cords[1].x + (normal2.x * 40), cords[1].y + (normal2.y * 40));
				glm::vec2 distance2 = glm::vec2(cords[2].x + (normal3.x * 40), cords[2].y + (normal3.y * 40));

				DrawLine(cords[0], distance0, meshModel[i].getColor());
				DrawLine(cords[1], distance1, meshModel[i].getColor());
				DrawLine(cords[2], distance2, meshModel[i].getColor());
			}
			if (meshModel[i].faceNormals) {

				glm::vec3 middleOfFace = (cords[1] + cords[0] + cords[2]);
				middleOfFace /= 3;
				glm::vec3 faceNormal = glm::normalize(glm::cross(cords[1] - cords[0], cords[2] - cords[0]));
				faceNormal *= 40;
				DrawLine(middleOfFace, glm::vec2(cords[0].x + faceNormal.x, cords[0].y + faceNormal.y), meshModel[i].getColor());
			}
		}

		if (meshModel[i].boundBox) {
			drawBoudingBox(scene, meshModel[i]);
		}
		drawModelAxies(scene, meshModel[i]);

	}

}

void Renderer::DrawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& color)
{
	DrawLine(glm::ivec2(p1.x, p1.y), glm::ivec2(p2.x, p2.y), color);
	DrawLine(glm::ivec2(p1.x, p1.y), glm::ivec2(p3.x, p3.y), color);
	DrawLine(glm::ivec2(p3.x, p3.y), glm::ivec2(p2.x, p2.y), color);
}

void Renderer::updateViewport()
{
	CreateBuffers(this->viewport_width, this->viewport_height);
}


void Renderer::setViewportHeight(int height)
{
	this->viewport_height = height;
}

void Renderer::setViewportWidth(int width)
{
	this->viewport_width = width;

}

int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}