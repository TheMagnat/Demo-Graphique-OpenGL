
//Shader non utilisé pour la boucle dessinMonte

#version 330

uniform sampler2D tex;
uniform float hight;
uniform float angle2;
uniform float temps;

in  vec2 vsoTexCoord;

out vec4 fragColor;

void main(void) {
  vec2 vecteur = vsoTexCoord - vec2(0.5);
  float distance = length(vecteur);
  float tempoDistance;
  float angle = atan(vecteur.y, vecteur.x);
  /* c'est la ligne qui suit qui change tout ! */
  //angle +=  0.1 * temps / (1.0 + distance);
  //angle -=  0.06 * hight / (1.0 + distance);
  //angle = 0.1;

  tempoDistance = distance;

  tempoDistance -= temps * 0.06;

  //tempoDistance = mod(tempoDistance, sqrt(2)/2);

  //tempoDistance -= hight * 0.06;

  distance = tempoDistance;

  vec2 tc = vec2(0.5) + vec2(distance * cos(angle), distance * sin(angle));
  vec4 color = texture(tex, tc);

  if(color.r > 0.2){
    fragColor = vec4(0, 0, 0, 0);
  }
  else{
    fragColor = color;
  }
}
