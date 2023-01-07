# Computer Graphics 203.3710
## Assignmet 1 part C report.

Name: Abed Alftah Abed Alftah

Student Number: 206529836

Name: Roge Azzam

Student Number: 207177098

Requirement 1:

We removed the centering trick made in the previous assignment in the renderer class (changing each coordinate
 (x,y) to (x+half_width, y+half_height) ), and replaced it by translating the camera itself (multiplying by adjustMat in camera class) such that the mesh will appear at the middle of the viewport.
 
 We opened the "cow.obj", the initialization looked like this:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/FirstReq1.png)


We changed the ups,downs,left,right and near,far, then applied the glm::orthographic function on the new values, then multiply it by look at and transformation matrices (implement look at matrix by applying glm::lookAt function, giving it eye, at, up values) , and got the following results:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/FirstReq2.png)


As you can see the near and far have no impact on the result.



Requirement 2:

Just Like the mesh transformations from the previous assignment, we apply transformations by multiplying world * local (implementation done in Camera class), in this order: translate * rotate, then we apply inverse to the result matrix (Instead of transforming the camera, we inverse transform the mesh's and keep the camera at the "eye" location looking at "at" coordinate and the "up" being Y axis).
Here's an example of applying:
translate x - 300 (world) 
translate y - -150 (world) 
rotate y - 50 (local) 
rotate z - -30 (local) 

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/SecondReq.png)

Also, By pressing the "Incremental Changes" checkbox and choosing world transformations, you can start translating the camera's X axis left and right by pressing R and L keyboard keys.


Requirement 3:

Opening "dolphin.obj" mesh model:
Step 1:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/ThirdReq1.png)

Step 2:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/ThirdReq2.png)

Step 3:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/ThirdReq3.png)

When we change the window size we implement new renderer and push the new viewports sizes to the buffer (implement in main RendererFrame function).


Requirement 4:

Transforming in the model frame: translate -> rotate by X -> rotate by Y, you can see the world and model axiss collide:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/FourthReq1.png)

Same transforming but in the world frame: translate -> rotate by X -> rotate by Y

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/FourthReq2.png)

Another Example:
Translating in the model frame, then rotating in the world frame:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/FourthReq3.png)

Translating in the world frame, then rotating in the model frame:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/FourthReq4.png)

The implementation: we apply world transformations on the local axis (and camera inverse transformation)



Requirement 5:

Making transformations in the local frame then in the world frame:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/local-world.gif)

Making approximatly same transformations in the world frame then in the local frame:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/world-local.gif)

The reason for the differnces (for example seeing the difference between translating local than rotating world, and translating world and rotating local) is that when making world transformations the local axis (that starts in the middle of the mesh) moves along with the mesh, wheras the local transformations moves the mesh according to the local axis. 



Requirement 6:

Transforming (translating then rotating) the mesh both in the local and world frame, and showing the local Bounding Box:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/SixthReq1.png)

Transforming (translating then rotating) the mesh both in the local and world frame, and showing the world Bounding Box:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/SixthReq2.png)


The world bounding box will always keep the sides parellal to the X,Y,Z axis's, wheras the local bounding box will transform along with the mesh.



Requirement 7:

Showing face normals after making transformations:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/SeventhReq1.png)

Showing vertices normals after making transformations:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/SeventhReq2.png)


Requirement 8:

Implementing Orthographic projection (Using glm::orthographic to get the orthographic project, and multiplying it by look at and transformation matrices):

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/EigthReq1.png)

Implementing Orthographic projection and moving the camera away (as you can see there's no visible change):

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/EigthReq2.png)

Implementing Perspective projection (Using glm::perspective):

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/EigthReq3.png)

Implementing Perspective projection and moving the camera away:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/EigthReq4.png)


Requirement 9:

Loading the "dolphin.obj" mesh, first view:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/NinthReq1.png)

After changing the zoom (zooming in):

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/NinthReq2.png)

After moving closer:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/NinthReq3.png)

After changing the zoom (zooming out):

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/NinthReq4.png)

After moving further:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/NinthReq5.png)


Requirement 10:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/TenthReq.png)


Requirement 11:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/EleventhReq1.png)

Making Dolly zoom:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/EleventhReq2.png)


Requirement 12:

You can choose which camera index you'd like to use (each has it's own orthographic/perspective, look at and transformations)

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/TwelvthReq1.png)

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/TwelvthReq2.jpeg)


Requirement 13:

Here you can choose Object control window checkbox, and show the mesh written in the "which model?" bar (and pressing Turn On checkbox):

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/TherteenReq1.png)

By pressing the "Advanced features" checkbox you can control the local/world Bounding Box, and showing faces/vertices normals:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/TherteenReq2.png)

After opening the Camera Control Window, you can choose either Orthographic or Perspective projection, by choosing Orthographic a window for changing up, down,lft,right and near,far values(for the orthographic matrix) will open:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/TherteenReq3.png)

By choosing Perspective a different window for fovy, aspect, near, far values(for the perspective matrix) will open:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/TherteenReq4.png)

By choosing Look At values, a window for changin the Eye values (That will affect the position of the camera, by changing the look at matrix for the camera)

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/TherteenReq5.png)


Choosing incremental changes will give two options: World or Local:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/TherteenReq6.png)


After choosing one a window for the camera transformations will open (Inverse will be made to the mesh):

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/TherteenReq7.png)


Requirement 14:

By pressing the "Show World Axis" checkbox or "Bound Box" you can show or hide the world axis/bound box.

The ability to change the model color.

