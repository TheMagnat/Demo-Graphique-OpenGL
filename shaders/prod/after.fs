#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D blurTexture;

void main(){

    vec4 beforeColor = texture(screenTexture, TexCoords);
    vec4 afterColor = 4 * texture(blurTexture, TexCoords);

    FragColor = beforeColor + afterColor;

}