
#version 330

uniform sampler2D screenTex;
uniform sampler1D mobileTex;

//Nombre de mobiles normalisé
uniform float step;


uniform float time;

in vec2 texCoord;

out vec4 color;

vec4 voronoif(void) {
	float i, d, secondMin = 500;
	float finalStep = step / 2.0;
	vec3 po = texture(mobileTex, finalStep + step).xyz;
	float actualMin = length(texCoord - po.xy);

	for(i = step / 2.0 + 2.0 * step; i < 1.0; i += 2.0 * step) {

		po = texture(mobileTex, i + step).xyz;

		if((d = length(texCoord - po.xy)) < actualMin){
			secondMin = actualMin;
			actualMin = d;
			finalStep = i;

		}
		else if(d < secondMin){
			secondMin = d;

		}

	}
	if(secondMin - actualMin < 0.003){
		return vec4(0, 0, 0, 1);
	}
	else{
		return texture(mobileTex, finalStep) + vec4(1) * (actualMin/secondMin)/3;  	
	}
}


void main(void){
	vec4 tempoColor = texture(screenTex, texCoord);
	//color = vec4(0, 0, 0, 1);
	
	if(length(tempoColor.rgb) > 0){
		color = voronoif();

	}
	else{
		color = vec4(0, 0, 0, 1);

	}

}