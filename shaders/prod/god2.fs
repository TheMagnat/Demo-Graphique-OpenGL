#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

vec4 ray(){

    vec4 tempoFragColor = vec4(0, 0, 0, 1);

    int nbDecalage = 150;



    //Vec2(0.5) represente le centre de la lumière
    vec2 delta = vec2(TexCoords - vec2(0.5));
    vec2 textCoodsSave = TexCoords;
    delta *= 1.0 /  float(nbDecalage);
    
    
    for(int i=0; i < nbDecalage; ++i){

            textCoodsSave -= delta;
            vec4 pixelDecale = texture2D(screenTexture, textCoodsSave);
            
            pixelDecale *= 0.005;
            
            tempoFragColor += pixelDecale;
            
    }
    

    return tempoFragColor;
}

void main(){

    vec4 color = texture(screenTexture, TexCoords);

    vec4 raycolor = ray();

    FragColor = 0.70*color + 1.15*raycolor;

}