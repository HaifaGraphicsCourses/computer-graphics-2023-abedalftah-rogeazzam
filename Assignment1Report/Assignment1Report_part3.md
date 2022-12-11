# Computer Graphics 203.3710
## Assignmet 1 part C report.

Name: Abed Alftah Abed Alftah

Student Number: 206529836

Name: Roge Azzam

Student Number: 207177098

Requirement 1:

We opened the "cow.obj", the initialization looked like this:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/FirstReq1.png)


We changed the ups,downs,... , and got the following results:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/FirstReq2.png)


As you can see the near and far have no impact on the result.



Requirement 2:

Just Like the mesh transformations from the previous assignment, we apply transformations by multiplying world * local (implementation done in Camera class), in this order: translate * rotate, then we apply inverse to the result matrix (Instead of transforming the camera, we inverse transform the mesh's and keep the camera at the "eye" location looking at "at" coordinate and the "up" being Y axis).
Here's an example of applying:
translate x - 300
translate y - -200
rotate y - 50
rotate z - -30

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/SecondReq.png)

Also, By pressing the "Show World Axis" checkbox and choosing incremental changes, you can start translating the camera's X axis left and right by pressing R and L keyboard keys.


Requirement 3:

Opening "dolphin.obj" mesh model:
Step 1:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/ThirdReq1.png)

Step 2:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/ThirdReq2.png)

Step 3:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/ThirdReq3.png)


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



Requirement 5:

Making transformations in the local frame then in the world frame:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/local-world.gif)

Making approximatly same transformations in the world frame then in the local frame:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/world-local.gif)



Requirement 6:

Initially, loading "bunny.obj" and showing the bounding box by pressing "Advanced features" checkbox, then pressing "bound box" checkbox:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/SixthReq1.png)


Transforming (translating then rotating) the mesh in the local frame: (The bounding box appears in white)

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/SixthReq2.png)

Transforming (translating then rotating) the mesh in the world frame:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/SixthReq3.png)


Requirement 7:

Showing face normals after making transformations:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/SeventhReq1.jpeg)

Showing vertices normals after making transformations:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/SeventhReq2.jpeg)


Requirement 8:

Implementing Orthographic projection:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/EigthReq1.png)

Implementing Orthographic projection and moving the camera closer (as you can see there's no visible change):

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/EigthReq2.png)

Implementing Perspective projection:

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

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/TwelvthReq1.png)

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/TwelvthReq2.jpeg)


Requirement 13:

Here you can show the World Axis, Bound Box, Vertices normals and face normals, for the mesh written in the "which model?" bar:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/TherteenReq1.png)

After opening the Camera Control Window, you can choose either Orthographic or Perspective projection, by choosing Orthographic a windo for changing up, down,... values will open:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/TherteenReq2.png)

By choosing Perspective a different window for fovy, aspect, near, far values will open:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/TherteenReq3.png)

Choosing incremental changes a window for the camera transformations will open (Inverse will be made to the mesh):

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment1Report/TherteenReq4.png)


Requirement 14:

By pressing the "Show World Axis" checkbox or "Bound Box" you can show or hide the world axis/bound box.

