# Computer Graphics 203.3710
## Assignmet 2 part A report.

Name: Abed Alftah Abed Alftah

Student Number: 206529836

Name: Roge Azzam

Student Number: 207177098

Requirement 1:

we calculated this equation such that we check if the z value of the bounding rectangle is lowest then it is black
and when we get farther the color of the rectangle is changed in gray levels until we get the highest z value then it is white.
((p1.z + p2.z + p3.z) / 3 - meshModel->minZpoint) / (meshModel->maxZpoint - meshModel->minZpoint + 1)


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q1.gif)


there is a button in the advanced features named Triangles Bounding Rectangles to trun on/off the feature.

Requirement 2:

we implmented Edge Walking Algorithm.


Requirement 3:

We generated random color for each triangle, and then called raterizeTriangle function to color the face using edge walking algorithm, and we can see triangles overlapping.

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q3.gif)


Requirement 4:

We can see in the image that the problem that we mentioned in the pervious question is not happening here because of the Z-Buffer,
we clearly see that the face of the bunny and there is no overlapping triangles, and we also loaded demo, but because it is further on the Z-Axis the bunny comes in front of it.

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q4_Z_Buffer_1.png)


another view of the Z Buffer, after moving the demo forward (translating Z-Axis), and it became in front of the bunny:

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q4_Z_Buffer_2.png)


Color Buffer, where the Demo object is further.

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q4_Z_Buffer_Color_Buffer_1.jpeg)


another view of the Color Buffer, where the Bunny is closer.

![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q4_Z_Buffer_Color_Buffer_2.jpeg)

