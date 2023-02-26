#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int type = 10;
uniform float max_z;
uniform vec3 mid;

// These outputs will be available in the fragment shader as inputs
out vec3 orig_fragPos;
out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;


const float PI = 3.14159;
void main()
{
	// Apply the model transformation to the 'position' and 'normal' properties of the vertex,
	// so the interpolated values of these properties will be available for usi n the fragment shader
	orig_fragPos = vec3(vec4(pos, 1.0f));
	fragPos = vec3(projection * view *  model * vec4(pos, 1.0f));
	fragNormal = transpose(inverse(mat3(model))) * normal;



	// This is an internal OpenGL variable, we must set a value to this variable
	gl_Position = projection * view *  model * vec4(pos, 1.0f);

	//plane
	if(type == 1){
		fragTexCoords.x = pos.x;
		fragTexCoords.y = pos.y;
	}
	//cylinder
	else if(type == 2){
		float theta=atan(pos.z,pos.x) + PI;
		float h=pos.y;
		fragTexCoords=vec2(theta/2*PI, h/max_z);
	}
	//sphere
	else if(type == 3){
		vec3 n=pos-vec3(mid);
		n=normalize(n);
		fragTexCoords.x=0.5+atan(n.x,n.z)/(2*PI);
		fragTexCoords.y=0.5-asin(n.y)/(PI);
	}
	else{
		// Pass the vertex texture coordinates property as it is. Its interpolated value
		// will be avilable for us in the fragment shader
		fragTexCoords = texCoords;
	}
}