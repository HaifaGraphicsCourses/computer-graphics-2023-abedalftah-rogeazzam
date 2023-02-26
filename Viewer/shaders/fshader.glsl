#version 330 core

struct Material
{
	samplerCube environmentMap;
	sampler2D textureMap;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	// You can add more fields here...
	// Such as:
	//		1. diffuse/specular relections constants
	//		2. specular alpha constant
	//		3. Anything else you find relevant
};
struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 pos;
};


// We set this field's properties from the C++ code
uniform Material material;
uniform Light light;
uniform vec3 camera_pos;
uniform int map = 10;
uniform int transitionsNum;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
// The final color of the fragment (pixel)
out vec4 frag_color;

float scale_factor = 1.0/transitionsNum;

void main()
{
	// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));

	// Normal shading
	if(map == 1){
		vec3 normalOfTexture = normalize(textureColor.rgb * 2.0 - 1.0);
		
		vec3 iA = light.ambient * textureColor;

		vec3 light_temp = normalize(light.pos - fragPos);

		float degree = max(dot(light_temp, normalOfTexture), 0.0);

		vec3 iD = material.diffuse * degree * light.diffuse;

		vec3 r = 2 * (normalOfTexture * degree) - light_temp;
		vec3 v = normalize(camera_pos - fragPos);

		float rv = max(pow(dot(r, v), 1), 0.0);

		vec3 iS = material.specular * rv * light.specular;
	
		frag_color = vec4(iA + iD + iS, 1.0);

	}
	// Environment mapping
	else if(map == 2){
		
		vec3 iA = light.ambient * material.ambient;

		vec3 light_temp = normalize(light.pos - fragPos);

		float degree = max(dot(light_temp, fragNormal), 0.0);

		vec3 iD = material.diffuse * degree * light.diffuse;

		vec3 r = 2 * (fragNormal * degree) - light_temp;
		vec3 v = normalize(camera_pos - fragPos);

		float rv = max(pow(dot(r, v), 1), 0.0);

		vec3 iS = material.specular * rv * light.specular;
	
		vec3 I = normalize(fragPos - camera_pos);
		vec3 R = reflect(I, normalize(fragNormal));

		frag_color = (vec4(texture(material.environmentMap, R).rgb, 1.0) * 0.4) + vec4((iA + iD + iS), 1) * 0.6;

	}
	// Toon mapping
	else if(map == 3){
		vec3 iA = light.ambient * material.ambient;

		vec3 light_temp = normalize(light.pos - fragPos);

		float degree = max(dot(light_temp, fragNormal), 0.0);
		float factor = degree;

		// Quantizing the intensity.
		if(factor > 0){
			factor = floor(factor * transitionsNum) * scale_factor;
		}

		vec3 iD = material.diffuse * factor * light.diffuse;

		vec3 r = 2 * (fragNormal * degree) - light_temp;
		vec3 v = normalize(camera_pos - fragPos);

		float rv = max(pow(dot(r, v), 1), 0.0);

		vec3 iS = material.specular * rv * light.specular;
	
		frag_color = vec4(factor * (iA + iD + iS), 1.0);

	}
	// Normal texture image
	else if(map == 4){
		frag_color = vec4(textureColor, 1);
	}
	else if(map == 5){
		frag_color = texture(material.environmentMap, fragPos);
	}
	// Constant color
	else{
		frag_color = vec4(0.8, 0.5, 0.3, 1);
	}
}
