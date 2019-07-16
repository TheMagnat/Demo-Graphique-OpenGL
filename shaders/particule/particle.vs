
/*
Vertex Shader pour les particules,
à moitié repris du cours sur les particules de http://www.opengl-tutorial.org
Il s'occupe de calculer les coordonée ecran des sommets des particules en partant de leur position centrale,
et calcule les cordonée de texture en fonction du temps pour des textures animées.
*/


#version 330 core

const int NB_IMAGE_PER_LINE = 4;
const int NB_IMAGE_PER_COL = 4;
const int IMAGE_MAX = 16;

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; // Position of the center of the particule and size of the square
layout(location = 2) in vec4 color; // Position of the center of the particule and size of the square
layout(location = 3) in float time; // Time

// Output data ; will be interpolated for each fragment.
out vec2 texPos;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)

void main(){

	float particleSize = xyzs.w; //Car la taille est stoqué dans le 4eme element du vec4 localisé en 1 dans la data.
	vec3 particleCenter_wordspace = xyzs.xyz;
	int i = 0;
	int j = 0;

	//Cette ligne permet d'assigner les bonnes cordonnée à nos vertex en partant d'une position centrale
	vec3 vertexPosition_worldspace = 
		particleCenter_wordspace									//Position centre
		+ CameraRight_worldspace * squareVertices.x * particleSize	//Ajout du X 
		+ CameraUp_worldspace * squareVertices.y * particleSize;	//Ajout du Y


	int texNb = int(IMAGE_MAX * time);

	float xStep = 1.0/NB_IMAGE_PER_LINE;
	float yStep = 1.0/NB_IMAGE_PER_COL;

	int line = int(texNb / NB_IMAGE_PER_LINE);
	int col =  int(texNb%NB_IMAGE_PER_COL);

	float posTexX = float(col)/float(NB_IMAGE_PER_COL);
	float posTexY = (1 - yStep) - float(line)/float(NB_IMAGE_PER_LINE);

	if(squareVertices.x > 0){
		texPos.x = posTexX + xStep;
	}
	else{
		texPos.x = posTexX;
	}

	if(squareVertices.y > 0){
		texPos.y = posTexY + yStep;
	}
	else{
		texPos.y = posTexY;
	}

	gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f);
	
}

