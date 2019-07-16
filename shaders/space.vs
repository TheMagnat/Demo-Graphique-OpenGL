
//Vertex Shader simple pour un quad

#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 vsoTexCoord; 

void main(void) {
  vsoTexCoord = texCoord;
  gl_Position = vec4(position, 1.0);
}
