#version 330

layout (location = 0) in vec3 vsiPosition;
layout (location = 1) in vec2 vsiTexCoord;

uniform int inv;

uniform mat4 modelViewMatrix;
uniform mat4 projection;

out vec2 vsoTexCoord;

void main(void) {

	gl_Position = projection * modelViewMatrix * vec4(vsiPosition, 1.0);

	//On inverse la texture car elle est inversé avec sdl_ttf
	vsoTexCoord = vec2(vsiTexCoord.s, 1.0 - vsiTexCoord.t);

}
