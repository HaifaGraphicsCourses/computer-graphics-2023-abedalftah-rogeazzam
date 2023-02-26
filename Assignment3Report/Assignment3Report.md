# Computer Graphics 203.3710
## Assignmet 3 report.

Name: Abed Alftah Abed Alftah

Student Number: 206529836

Name: Roge Azzam

Student Number: 207177098



## Requirement 1:

1) We create a unique identifier for an opengl texture.
2) We do the same for vertex array object which we call VAO, VAO is a set of buffers that describe a renderable object.
3) Then we bind it and create a buffer in the vertex array we made in step (2).
4) Initialize two arrays vtc is coordinate arrays for the vertices, tex is coordinates for the texture mapping
5) Then we bind buffer to GL_ARRAY_BUFFER and after this we can allocate memory for that data
6) By glBufferData we allocate memory to the gpu
7) glBufferSubData is to copy the allocated memeory to the buffer
8) Then we initiate and compile shader
9) After that we tell the shader where to look for the vertex position data, and the data dimensions, same for texture coordinates data finally we pass them to draw on the screen.



## Requirement 2:

We made two new buffers VBO and VAO and a struct Vertex so every vertex have a position, normal, texture.
we do the same as Requirement 1 (except 8, 9 we do in the renderer) :


	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);


## Requirement 3:

Here we apply the transformations previously made in the Render to the vertices/normals and send them to the "Clipper and primitive assembler"...


    #version 330 core
    
    layout(location = 0) in vec3 pos;
    layout(location = 1) in vec3 normal;
    layout(location = 2) in vec2 texCoords;

    // The model/view/projection matrices
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    
    // These outputs will be available in the fragment shader as inputs
    out vec3 orig_fragPos;
    out vec3 fragPos;
    out vec3 fragNormal;
    out vec2 fragTexCoords;

    void main()
    {
	   // Apply the model transformation to the 'position' and 'normal' properties of the vertex,
	   // so the interpolated values of these properties will be available for usi n the fragment shader
	   orig_fragPos = vec3(vec4(pos, 1.0f));
	   fragPos = vec3(projection * view * model * vec4(pos, 1.0f));
	   fragNormal = mat3(transpose(inverse(model))) * normal;
       
       
       
	   // This is an internal OpenGL variable, we must set a value to this variable
	   gl_Position = projection * view *  model * vec4(pos, 1.0f);
       
       fragTexCoords = texCoords;
    }



## Requirement 4:


    #version 330 core

    out vec4 frag_color;

    void main()
    {
        frag_color = vec4(0.8,0.5,0.6,1);
    }
    


## Requirement 5:

    void Renderer::Render(Scene& scene)
    {
	   if (scene.GetActiveCameraIndex() == -1)
		  return;

	   vector<int> meshModel = scene.GetActiveModelsIndexes();

	   for (int i = 0; i < meshModel.size(); i++) {
		  // Activate the 'colorShader' program (vertex and fragment shaders)
		  colorShader.use();

		  // Set the uniform variables
		  colorShader.setUniform("model", scene.GetActiveModel(meshModel[i]).transformationMat());
		  colorShader.setUniform("view", scene.GetActiveCamera().GetViewTransformation());
		  colorShader.setUniform("projection", scene.GetActiveCamera().GetProjectionTransformation());
		  colorShader.setUniform("material.textureMap", 0);
          
		  // Set 'texture1' as the active texture at slot #0
		  texture1.bind(0);
          
		  // Drag our model's faces (triangles) in fill mode
		  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		  glBindVertexArray(scene.GetActiveModel(0).vao);
		  glDrawArrays(GL_TRIANGLES, 0, scene.GetActiveModel(meshModel[i]).modelVertices.size());
		  glBindVertexArray(0);
          
		  // Unset 'texture1' as the active texture at slot #0
		  texture1.unbind(0);
          
		  colorShader.setUniform("color", glm::vec3(0, 0, 0));
		
		  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		  glBindVertexArray(scene.GetActiveModel(0).vao);
		  glDrawArrays(GL_TRIANGLES, 0, scene.GetActiveModel(meshModel[i]).modelVertices.size());
		  glBindVertexArray(0);

	   }
    }



Here is a picture that every thing is working:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement5.png)



## Requirement 6:

We loaded a new mesh "Stanford Bunny" from the link provided, as you can see the program now is much faster ( ~450 fps) due to loading it in the GPU, where previously it was slow:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement6_1.png)


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement6_2.png)


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement6_3.png)


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement6_4.png)


### And one showing for old mesh:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement6_5.png)



## Requirement 7:

As was shown in the lecture, when applying each type of canonical project with "squares" on the teapot (the new one downloaded from the link) we get the following results:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement7_1.png)


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement7_2.png)


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement7_3.png)


### The crate with normal texture:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement7_4.png)


### Now we found a banana mesh with textures and a UV image online and added it to the Data, the obj file name is "banana_tex" and the image is banana.jpg, We got the following result:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement7_5.gif)



## Requirement 8:

### Normal Mapping:

Before:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement8_1.png)


After:


1)

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement8_2.png)


2)

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement8_2_1.png)



### Environment Mapping:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement8_3.gif)



While this is the world around it:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/environmentAxis.png)


### Toon Mapping:


Toon Mapping with different lightnings and number of transitions:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement8_4.png)



![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement8_5.png)



![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment3Report/Requirement8_6.png)
