#version 330

uniform mat4 modelViewMatrix;
uniform mat4 view;
uniform mat4 projection;
layout (location = 0) in vec3 vsiPosition;
layout (location = 1) in vec3 vsiNormal;
 
out vec3 FragPos;
out vec3 vsoNormal;

void main(void) {
  vsoNormal = (transpose(inverse(modelViewMatrix))  * vec4(vsiNormal, 0.0)).xyz;

  gl_Position = projection * view * modelViewMatrix * vec4(vsiPosition, 1.0);

  //Pour avoir les coordonnée du fragment dans l'espace monde
  FragPos = vec3(modelViewMatrix * vec4(vsiPosition, 1.0));
}
