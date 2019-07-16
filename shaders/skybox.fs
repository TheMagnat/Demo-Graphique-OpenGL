//fragment shaders simple pour une texture cubique

#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main(){
	vec4 color = texture(skybox, TexCoords);
	if(color.r > 0.1){
		FragColor = vec4(0, 0, 0, 0);
	}
	else{
		FragColor = vec4(color.rgb, 1);
	}
}