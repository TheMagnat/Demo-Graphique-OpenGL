
//Vertex shader principale de la plus part des formes 3D de la demo.

#version 330

uniform mat4 modelViewMatrix;
uniform mat4 view;
uniform mat4 projection;
layout (location = 0) in vec3 vsiPosition;
layout (location = 1) in vec3 vsiNormal;
 
out vec3 posVertex;
out vec3 FragPos;
out vec3 vsoNormal;

out vec2 ddPos;

void main(void) {
  vsoNormal = (transpose(inverse(modelViewMatrix))  * vec4(vsiNormal, 0.0)).xyz;

  gl_Position = projection * view * modelViewMatrix * vec4(vsiPosition, 1.0);
  //gl_Position = projection * vec4(vsiPosition, 1.0);

  //Pour avoir les coordonnée du fragment dans l'espace monde
  FragPos = vec3(modelViewMatrix * vec4(vsiPosition, 1.0));

  posVertex = vsiPosition;

  if(vsiNormal.x != 0){
  	ddPos = vec2(posVertex.z, posVertex.y);
  }
  else if(vsiNormal.y != 0){
  	ddPos = vec2(posVertex.x, posVertex.z);
  }
  else if(vsiNormal.z != 0){
  	ddPos = vec2(posVertex.x, posVertex.y);
  }
}
