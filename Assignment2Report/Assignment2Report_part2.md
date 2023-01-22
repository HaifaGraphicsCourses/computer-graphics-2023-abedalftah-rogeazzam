# Computer Graphics 203.3710
## Assignmet 2 part B report.

Name: Abed Alftah Abed Alftah

Student Number: 206529836

Name: Roge Azzam

Student Number: 207177098

## Requirement 1:

As required, you can add several lights and choose the light you would like to control by writing the index, and control the position and the RGB components, similarly you can specify the material of each model.

First we loaded a Cow model with the following ambient:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q1_1.png)

As we can see, by changing the ambient of the light the color and the atmosphere changes, as expected since the ambient can change the color and the atmosphere, and creates a general and uniform lighting:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q1_2.png)



In addition changing the light position won't change the coloring of the faces, since for each face we compute: Ia = La * Ka (Light ambient * Model ambient), which has nothing to do with the position of the light,


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q1_3.png)



## Requirement 2:


We computed the diffuse color using the following formula: kd*(l*n)*Ld.

Since it's flat shading, for every face we compute it's own color independantly from other faces, That is the transition won't be smooth, as appears in the picture:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q2_1.png)



## Requirement 3:


As expected, we can see more smoothness in Gouraud shading, that's because we computed three colors for each of the triangle vertices, then applied interpolation of the three colors on each pixel, which gives smoother result:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q3_1.png)


Changing the position of the light:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q3_2.png)



## Requirement 4:


We computed the direction of the light to the center of the face and it appears as the blue lines, and the reflection from the faces appears as orange lines, using the formula  r = 2 * normal * cos(teta) - I ,  where teta is the angle between the normal and I (I is the vector between the point on the model and the light position):


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q4_1.png)


After moving the light, you can see the directions has changed:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q4_2.png)



## Requirement 5:


The phong shading is the smoothest among all, since for each pixel we compute the color by finding it's own normal (by interpolation of the triangle vertices normals), and then find the color, In addition we computed the specular light using the formula:  model specular * cos(alpha) * light specular , where alpha is the angle between r (calculated previously) to the vector v (v is the vector between the camera position and the point on the model).


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q5_1.png)


Changing the position of the light:


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q5_2.png)



## Requirement 6:


Here We put two different meshes with different colors and lights (We change the light position from picture to another and/or add light which sometimes might appear at the bottom of the screen as orange square)


![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q6_1.png)



![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q6_2.png)



![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q6_3.png)



![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q6_4.png)



![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q6_5.png)



![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q6_6.png)



![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q6_7.png)



![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q6_8.png)



![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q6_9.png)



![alt text](https://github.com/HaifaGraphicsCourses/computer-graphics-2023-abedalftah-rogeazzam/blob/master/Assignment2Report/Q6_10.png)
