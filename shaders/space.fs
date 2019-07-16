
/*
Fragment shader a moitié repris des shaders des smaple GL4D.
Donne l'impression d'avancé dans l'espace
*/

#version 330

uniform sampler2D tex;
uniform float hight;
uniform float angle2;
uniform float temps;
uniform float finAll;

uniform vec3 clearColor;

in  vec2 vsoTexCoord;

out vec4 fragColor;

void main(void) {
  vec2 vecteur = vsoTexCoord - vec2(0.5);
  float distance = length(vecteur);
  float tempoDistance;
  float angle = atan(vecteur.y, vecteur.x);

  //Effet sympa mais je préfère sans
  //angle -=  0.06 * hight / (1.0 + distance);

  tempoDistance = distance;

  tempoDistance -= temps * 0.06;

  tempoDistance = mod(tempoDistance, 1.0/2.0);

  tempoDistance -= hight * 0.06;

  distance = tempoDistance;

  vec2 tc = vec2(0.5) + vec2(distance * cos(angle), distance * sin(angle));
  vec4 color = texture(tex, tc);



  if(length(clearColor) == 0){
    fragColor = vec4(0, 0, 0, 1);
  }
  else{
    if(color.r > 0.2){
    	 fragColor = vec4(0, 0, 0, 0);
    }
    else{
    	fragColor = color;
    }
  }
}
