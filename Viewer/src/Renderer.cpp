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
	for (int i = 0; i < viewport_width; i++)
		delete[] zBuffer[i];
	delete[] zBuffer;
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
			{
				//float z = depth(glm::vec3(p1, z1), glm::vec3(p2, z2), x1, y1);
				PutPixel(x1, y1, color);
			}
			else {
				PutPixel(y1, x1, color);
			}
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
	glm::fvec4 worldX;
	glm::fvec4 worldY;
	worldX = mat * glm::fvec4(viewport_width + worldAxies[0], 0, 0, 1);
	worldY = mat * glm::fvec4(0, viewport_height + worldAxies[1], 0, 1);

	glm::fvec4 worldZ = mat * glm::fvec4(0, 0, viewport_height, 1);

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

	zBuffer = new float* [viewport_width];
	for (int i = 0; i < viewport_width; i++)
		zBuffer[i] = new float[viewport_height];

	for (int i = 0; i < viewport_width; i++)
		for (int j = 0; j < viewport_height; j++)
			zBuffer[i][j] = INFINITY;

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
	glm::mat4x4 mat = meshModel.worldTransMat();
	glm::mat4x4 mat1 = glm::mat4x4(1.0f);

	if (scene.GetActiveCameraIndex() != -1)
		mat1 = scene.GetActiveCamera().GetViewTransformation();

	mat = mat1 * mat;

	glm::fvec4 modelAxies = mat * glm::fvec4(0, 0, 0, 1);
	glm::fvec4 localX = mat * glm::fvec4(140, 0, 0, 1);
	glm::fvec4 localY = mat * glm::fvec4(0, 140, 0, 1);
	glm::fvec4 localZ = mat * glm::fvec4(0, 0, 140, 1);

	modelAxies.x /= modelAxies.w;
	modelAxies.y /= modelAxies.w;
	modelAxies.z /= modelAxies.w;


	localX.x /= localX.w;
	localX.y /= localX.w;
	localX.z /= localX.w;

	localY.x /= localY.w;
	localY.y /= localY.w;
	localY.z /= localY.w;

	localZ.x /= localZ.w;
	localZ.y /= localZ.w;
	localZ.z /= localZ.w;

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


void Renderer::drawBoudingBox(Scene& scene, MeshModel meshModel, bool isLocal)
{
	//we neeed to divide by f.w
	glm::mat4x4 mat = meshModel.worldTransMat();
	glm::mat4x4 mat1 = glm::mat4x4(1.0f);

	if (scene.GetActiveCameraIndex() != -1) {
		mat1 = scene.GetActiveCamera().GetViewTransformation();

	}

	if (isLocal) {
		mat = meshModel.transformationMat();
	}
	else {
		mat = glm::mat4x4(1.0f);
		meshModel.calculateExtremes();
	}
	mat = mat1 * mat;

	glm::fvec4 f0 = mat * glm::fvec4(meshModel.max_x, meshModel.max_y, meshModel.max_z, 1);
	glm::fvec4 f1 = mat * glm::fvec4(meshModel.max_x, meshModel.max_y, meshModel.min_z, 1);

	glm::fvec4 f4 = mat * glm::fvec4(meshModel.min_x, meshModel.max_y, meshModel.max_z, 1);

	glm::fvec4 f7 = mat * glm::fvec4(meshModel.min_x, meshModel.max_y, meshModel.min_z, 1);


	glm::fvec4 f6 = mat * glm::fvec4(meshModel.min_x, meshModel.min_y, meshModel.min_z, 1);

	glm::fvec4 f8 = mat * glm::fvec4(meshModel.min_x, meshModel.min_y, meshModel.max_z, 1);

	glm::fvec4 f2 = mat * glm::fvec4(meshModel.max_x, meshModel.min_y, meshModel.min_z, 1);

	glm::fvec4 f3 = mat * glm::fvec4(meshModel.max_x, meshModel.min_y, meshModel.max_z, 1);

	f0.x /= f0.w; f0.y /= f0.w; f0.z /= f0.w;
	f1.x /= f1.w; f1.y /= f1.w; f1.z /= f1.w;
	f2.x /= f2.w; f2.y /= f2.w; f2.z /= f2.w;
	f3.x /= f3.w; f3.y /= f3.w; f3.z /= f3.w;
	f4.x /= f4.w; f4.y /= f4.w; f4.z /= f4.w;
	f6.x /= f6.w; f6.y /= f6.w; f6.z /= f6.w;
	f7.x /= f7.w; f7.y /= f7.w; f7.z /= f7.w;
	f8.x /= f8.w; f8.y /= f8.w; f8.z /= f8.w;


	DrawLine(f0, f1, glm::vec3(1, 0, 0));
	DrawLine(f0, f4, glm::vec3(1, 0, 0));
	DrawLine(f0, f3, glm::vec3(1, 0, 0));

	DrawLine(f7, f1, glm::vec3(1, 0, 0));
	DrawLine(f7, f4, glm::vec3(1, 0, 0));
	DrawLine(f7, f6, glm::vec3(1, 0, 0));

	DrawLine(f2, f3, glm::vec3(1, 0, 0));
	DrawLine(f2, f1, glm::vec3(1, 0, 0));
	DrawLine(f6, f2, glm::vec3(1, 0, 0));

	DrawLine(f8, f4, glm::vec3(1, 0, 0));
	DrawLine(f8, f3, glm::vec3(1, 0, 0));
	DrawLine(f8, f6, glm::vec3(1, 0, 0));
}

void Renderer::drawCameras(Scene& scene)
{
	//we need to draw it in the right way.. 
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


		glm::vec4 transformP2 = mat * glm::vec4(cords[1], 1);
		cords[1].x = transformP2.x;
		cords[1].y = transformP2.y;
		cords[1].z = transformP2.z;


		glm::vec4 transformP3 = mat * glm::vec4(cords[2], 1);
		cords[2].x = transformP3.x;
		cords[2].y = transformP3.y;
		cords[2].z = transformP3.z;

		if (cords[0].x > cords[1].x)
			DrawTriangle(cords[2], cords[1], cords[0], glm::fvec3(0));
		else if (cords[2].x > cords[0].x)
			DrawTriangle(cords[1], cords[0], cords[2], glm::fvec3(0));
		else
			DrawTriangle(cords[0], cords[2], cords[1], glm::fvec3(0));
	}

	//}
}


void Renderer::Render(Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
	//DrawFlower();

	for (int i = 0; i < viewport_width; i++){
		for (int j = 0; j < viewport_height; j++) {
			zBuffer[i][j] = INFINITY;
			color_buffer[INDEX(viewport_width, i, j, 0)] = color_buffer[INDEX(viewport_width, i, j, 1)]
				= color_buffer[INDEX(viewport_width, i, j, 2)] = 0.8f;

		}
	}

	if (scene.GetActiveCameraIndex() != -1 && scene.GetActiveCamera().drawWorldAxisFlag)
		drawWorldAxies(scene.GetActiveCamera());
	if (scene.drawCameras) {
		drawCameras(scene);
	}

	vector<MeshModel> meshModel = scene.GetActiveModels(scene.GetActiveModelsIndexes());

	this->isZbuff = false;

	for (int i = 0; i < meshModel.size(); i++) {
		std::vector<Face> faces = meshModel[i].getFaces();
		std::vector<glm::vec3> vertices = meshModel[i].getVertices();

		//we need to multiply by camera transformation
		glm::mat4x4 mat1 = glm::mat4x4(1.0f);
		if (scene.GetActiveCameraIndex() != -1)
			mat1 = scene.GetActiveCamera().GetViewTransformation();

		glm::mat4x4 mat = meshModel[i].transformationMat();
		mat = mat1 * mat;
		meshModel[i].updateZPoints(mat);
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

			
			glm::vec3 coll = meshModel[i].getColor();
			if (meshModel[i].showRandom) {
				float rand1 = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
				float rand2 = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
				float rand3 = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
				coll = glm::vec3(rand1, rand2, rand3);
			}
			if (meshModel[i].showZbuff) {
				coll = glm::vec3(1, 1, 1);
				this->isZbuff = true;
				this->maxZ = meshModel[i].maxZpoint;
				this->minZ = meshModel[i].minZpoint;
			}
			else if(meshModel[i].showColorbuff) {
				this->maxZ = meshModel[i].maxZpoint;
				this->minZ = meshModel[i].minZpoint;
				coll = meshModel[i].getColor();
			}
			DrawTriangle(cords[0], cords[1], cords[2], coll, &meshModel[i]);

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
			//a feature that can be controlled by the user
			if (meshModel[i].faceNormals) {

				glm::vec3 middleOfFace = (cords[1] + cords[0] + cords[2]);
				middleOfFace /= 3;
				glm::vec3 faceNormal = glm::normalize(glm::cross(cords[1] - cords[0], cords[2] - cords[0]));
				faceNormal *= 40;
				DrawLine(middleOfFace, glm::vec2(cords[0].x + faceNormal.x, cords[0].y + faceNormal.y), meshModel[i].getColor());
			}
		}


		if (meshModel[i].boundBox) {
			drawBoudingBox(scene, meshModel[i], 1);
		}
		if (meshModel[i].boundBoxWorld) {
			drawBoudingBox(scene, meshModel[i], 0);
		}
		if (meshModel[i].modelAxis) {
			drawModelAxies(scene, meshModel[i]); //Drawing the model axis.
		}

	}

}

void Renderer::DrawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& color, MeshModel* meshModel)
{
	float maxX = std::max(p1.x, p2.x), maxY = std::max(p1.y, p2.y), minX = std::min(p1.x, p2.x), minY = std::min(p1.y, p2.y);
	maxX = std::max(maxX, p3.x); maxY = std::max(maxY, p3.y); minX = std::min(minX, p3.x); minY = std::min(minY, p3.y);
	if (meshModel && meshModel->boundingRectangle) {
		glm::vec3 boundingColor = glm::vec3(((p1.z + p2.z + p3.z)/3 - meshModel->minZpoint) / (meshModel->maxZpoint - meshModel->minZpoint + 1), 
			((p1.z + p2.z + p3.z) / 3 - meshModel->minZpoint) / (meshModel->maxZpoint - meshModel->minZpoint + 1),
			((p1.z + p2.z + p3.z) / 3 - meshModel->minZpoint) / (meshModel->maxZpoint - meshModel->minZpoint + 1));
		DrawLine(glm::ivec2(minX, maxY), glm::ivec2(maxX, maxY), boundingColor);
		DrawLine(glm::ivec2(minX, maxY), glm::ivec2(minX, minY), boundingColor);
		DrawLine(glm::ivec2(minX, minY), glm::ivec2(maxX, minY), boundingColor);
		DrawLine(glm::ivec2(maxX, minY), glm::ivec2(maxX, maxY), boundingColor);
	}
	if (meshModel && !meshModel->showRandom && !meshModel->showZbuff && !meshModel->showColorbuff) {
		DrawLine(glm::ivec2(p1.x, p1.y), glm::ivec2(p2.x, p2.y), color);
		DrawLine(glm::ivec2(p1.x, p1.y), glm::ivec2(p3.x, p3.y), color);
		DrawLine(glm::ivec2(p3.x, p3.y), glm::ivec2(p2.x, p2.y), color);
		if (!meshModel->showRaterized)
			return;
	}

	if (!meshModel)
		return;

	glm::vec3 p1_temp = p1, p2_temp = p2, p3_temp = p3;

	if (p1_temp.y > p2_temp.y)
		std::swap(p1_temp, p2_temp);
	if(p2_temp.y > p3_temp.y)
		std::swap(p3_temp, p2_temp);
	if (p1_temp.y > p2_temp.y)
		std::swap(p1_temp, p2_temp);

	if(p1_temp.y == p2_temp.y && p1_temp.x > p2_temp.x)
		std::swap(p1_temp, p2_temp);
	if(p2_temp.y == p3_temp.y && p2_temp.x > p3_temp.x)
		std::swap(p3_temp, p2_temp);

	if (p1_temp.x == p2_temp.x && p1_temp.x == p3_temp.x || p1_temp.y == p2_temp.y && p1_temp.y == p3_temp.y)
		return;

	float a1 = (p3_temp.x - p1_temp.x) / (float)(p3_temp.y - p1_temp.y), b1 = p3_temp.x - a1 * p3_temp.y;
	int theX = (int)(a1 * p2_temp.y + b1);
	if (theX > p2_temp.x)
		raterizeTriangle(p1_temp, p2_temp, p3_temp, color, true, meshModel);
	else
		raterizeTriangle(p1_temp, p2_temp, p3_temp, color, false, meshModel);
}

//Edge Walking implementation
void Renderer::raterizeTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color, bool flag, MeshModel* meshModel)
{
	float a1 = 0, a2 = 0, currentX1, currentX2, z1 = 1, z2 = 1, z_val = 1;
	if (p2.y != p1.y)
		a1 = (float)(p2.x - p1.x) / (float)(p2.y - p1.y);
	if (p3.y != p1.y)
		a2 = (float)(p3.x - p1.x) / (float)(p3.y - p1.y);

	currentX1 = currentX2 = p1.x;

	// walk along the two sides of the triangle and fill the pixels
	for (int scanlineY = (int)p1.y; scanlineY < (int)p2.y; scanlineY++) {

		int startX = (int)currentX1;
		int endX = (int)currentX2;
		if (startX > endX)
			std::swap(startX, endX);

		for (int x = startX; x <= endX; x++) {
			if (meshModel->showZbuff || meshModel->showColorbuff) {
				PutPixelCheck(p1, p2, p3, x, scanlineY, color, meshModel->showColorbuff);
				continue;
			}
			PutPixel(x, (int)scanlineY, color);
		}

		currentX1 += a1;
		currentX2 += a2;
	}
	if (currentX1 > currentX2)
		std::swap(currentX1, currentX2);

	if (flag) {
		currentX1 = p2.x;
		if (p2.y != p3.y)
			a1 = (float)(p3.x - p2.x) / (float)(p3.y - p2.y);
		if (p3.y != p1.y)
			a2 = (float)(p3.x - p1.x) / (float)(p3.y - p1.y);
	}
	else {
		currentX2 = p2.x;
		if (p3.y != p1.y)
			a1 = (float)(p3.x - p1.x) / (float)(p3.y - p1.y);
		if (p2.y != p3.y)
			a2 = (float)(p3.x - p2.x) / (float)(p3.y - p2.y);
	}

	for (int scanlineY = (int)p2.y; scanlineY < (int)p3.y; scanlineY++) {

		int startX = (int)currentX1;
		int endX = (int)currentX2;
		if (startX > endX)
			std::swap(startX, endX);

		for (int x = startX; x <= endX; x++) {
			if (meshModel->showZbuff || meshModel->showColorbuff) {
				PutPixelCheck(p1, p2, p3, x, scanlineY, color, meshModel->showColorbuff);
				continue;
			}
			PutPixel(x, (int)scanlineY, color);
		}

		currentX1 += a1;
		currentX2 += a2;
	}
}

void Renderer::PutPixelCheck(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int x, int y, glm::vec3 color, bool showColorbuff) {
	float z_val = 1;
	z_val = findDepth(p1, p2, p3, x, y);
	float d_min = std::abs(z_val - minZ), d_max = std::abs(z_val - maxZ);
	float ratio;
	if (d_max != 0)
		ratio = d_min / d_max;
	else
		ratio = 1;
	glm::vec3 pixCol = glm::vec3(((1.0 - ratio) / 1.5),
		((1.0 - ratio) / 1.5), ((1.0 - ratio) / 1.5));

	if (showColorbuff) {
		pixCol = glm::vec3(color.x * pixCol.x, color.y * pixCol.y, color.z * pixCol.z);
	}

	int row = std::min(viewport_width - 1, std::max(x, 0)), col = std::min(viewport_height - 1, std::max(y, 0));

	if (z_val <= zBuffer[row][col]) {
		zBuffer[row][col] = z_val;
		PutPixel(x, y, pixCol);
	}
}

float Renderer::findDepth(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int x, int y) {
	float A1 = std::abs((p3.x * p2.y + p2.x * y + x * p3.y)
		- (p3.y * p2.x + p2.y * x + y * p3.x)) / 2;

	float A2 = std::abs((p1.x * p3.y + p3.x * y + x * p1.y)
		- (p1.y * p3.x + p3.y * x + y * p1.x)) / 2;

	float A3 = std::abs((p1.x * p2.y + p2.x * y + x * p1.y)
		- (p1.y * p2.x + p2.y * x + y * p1.x)) / 2;

	float A = A1 + A2 + A3;
	float z = p1.z * A1 / A + p2.z * A2 / A + p3.z * A3 / A;
	return z;
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

int Renderer::GetViewportWidth()
{
	return viewport_width;
}

int Renderer::GetViewportHeight()
{
	return viewport_height;
}