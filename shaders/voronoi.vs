
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture;

out vec2 texCoord;

void main(void){
	texCoord = texture;
	gl_Position = vec4(position, 1.0);
}