
//Fragment shader principale de la plus part des formes 3D de la demo
/*
Contient pleins de petits effet à selectionnecer avec mode.
(dont quelques fractales 2D)
*/

/*
    Les calculs de lumière viennent des cours de developpez.net sur leur cours d'openGL,
    vue qu'il sagit du site ou j'ai obtenue une grande partie de mes conaissance en openGL
*/

#version 330

//Toute les information sur la lumière
struct Light {
    //Position de la lumiere
    vec3 position;

    //Direction si utilisation de lumiere éloigné
    //vec3 direction;

    //Intensité de la lumière sur chaque type de reflet
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //Attenuation
    float constant;
    float linear;
    float quadratic;
};
uniform Light light;

//Transparence
uniform float transpa;
uniform vec4 couleur;

//Position de la camera
uniform vec3 viewPos;

uniform float time;

uniform float mode;


in vec3 posVertex;
in vec3 FragPos;
in vec3 vsoNormal;

in vec2 ddPos;


out vec4 fragColor;

void transpaTime(vec3 colorOutput){

    vec3 tempoPos = posVertex.xyz;
    float dist = length(tempoPos);
    
    float coef = abs(cos(time*1.5f));


    if(dist < coef && transpa < 1.0f){
        fragColor = vec4(0, 0, 0, 0);
    }
    else{
        //Formule sympa pour avoir des jolie couleurs
        fragColor = vec4(mix(couleur.rgb * dist, colorOutput, 0.5) , transpa);
    }

}

void circle(vec3 colorOutput){

    vec3 tempoPos = posVertex.xyz;
    float dist = length(tempoPos);
    
    float coef = abs(cos(time*1.5f));

    float tempo = mod(dist, 0.1);

    if(tempo < abs(cos(time/2.0f))*0.1 && transpa < 1.0f){
        fragColor = vec4(0, 0, 0, 0);
                //fragColor = vec4(0, colorOutput.yz, 1);

    }
    else{
        //Formule sympa pour avoir des jolie couleurs
        fragColor = vec4(mix(couleur.rgb * dist, colorOutput, 0.5) , transpa);
    }

}

void circle2(vec3 colorOutput){

    vec3 tempoPos = posVertex.xyz;
    float dist = length(tempoPos);
    
    float coef = abs(cos(time*1.5f));

    float tempo = mod(dist, abs(cos(time/2.0f))*0.1);

    if(tempo < 0.025 && transpa < 1.0f){
        fragColor = vec4(0, 0, 0, 0);
                fragColor = vec4(0, colorOutput.yz, transpa);

    }
    else{
        //Formule sympa pour avoir des jolie couleurs
        fragColor = vec4(mix(couleur.rgb * dist, colorOutput, 0.5) , transpa);
    }

}

void trian(vec3 colorOutput){

    vec2 tempoPos = ddPos.xy;
    tempoPos += 0.1 * time;
    float dist = length(tempoPos);
    
    //float coef = abs(cos(time*1.5f));

    float tempo = mod(tempoPos.x, 0.1);
    float tempo2 = mod(tempoPos.y, 0.1);

    if(tempo + tempo2 < (0.1) && transpa < 1.0f){
        //fragColor = vec4(0, 0, 0, 0);
        fragColor = vec4(0, colorOutput.yz, transpa);
    }
    else{
        //Formule sympa pour avoir des jolie couleurs
        fragColor = vec4(mix(couleur.rgb * length(posVertex.xyz), colorOutput, 0.5) , transpa);
    }

}

void quad(vec3 colorOutput){

    vec2 tempoPos = ddPos.xy;
    //tempoPos += 0.1 * time;
    float dist = length(tempoPos);
    
    float coef = abs(cos(time))*0.1;

    float tempo = mod(tempoPos.x, coef);
    float tempo2 = mod(tempoPos.y, coef);

    if(transpa < 1.0f){
        if(tempo > coef/2 || tempo2 > coef/2){
            //fragColor = vec4(0, 0, 0, 0);
            //fragColor = vec4(0, colorOutput.yz, transpa);
            fragColor = vec4(mix(couleur.rgb * length(posVertex.xyz), colorOutput, 0.5) , transpa);  
        }
        else{
            //fragColor = vec4(mix(couleur.rgb * length(posVertex.xyz), colorOutput, 0.5) , 1);  
            fragColor = vec4(0, 0, 0, 0);
        }
    }
    else{
        //Formule sympa pour avoir des jolie couleurs
        fragColor = vec4(mix(couleur.rgb * length(posVertex.xyz), colorOutput, 0.5) , transpa);
    }

}

vec2 cmpAdd(in vec2 a, in vec2 b){
    return vec2(a.x + b.x, a.y + b.y);
}


vec2 cmpMult(in vec2 a, in vec2 b) {
    return vec2(a.x * b.x - a.y * b.y, a.y * b.x + a.x * b.y);
}

float cmpNorm(in vec2 a) {
    return sqrt(a.x * a.x + a.y * a.y);
}


const vec2 c = vec2(-0.038088, 0.9754633);
const float rmax = 100;


int nbBelongsTo(vec2 z, vec2 c) {
    int i = 0;
    for(; i < 100; ++i) {
        z = cmpMult(z, z);
        z = cmpAdd(z, c);
        if (cmpNorm(z) > rmax) break;
    }
    //i = int(i + 2 - log(log(z.x*z.x + z.y*z.y)) / log(2));
    return i;
}


void julia(vec3 colorOutput){

    if(transpa == 1.0f){
        fragColor = vec4(mix(couleur.rgb * length(posVertex.xyz), colorOutput, 0.5) , transpa);
    }
    else{

        vec2 c = vec2((abs(cos(time/2.0f)))*0.3 + 0.1 , (abs(sin(time/2.0f)))*0.5754633 + 0.2);

        int u_color = nbBelongsTo(vec2(ddPos.x , ddPos.y) * 2.5, c);


        float coef = 20.0/u_color; 

        /*float rIntValue = cos(u_color);
        float gIntValue = cos(u_color*1.1);
        float bIntValue = cos(u_color*1.2);*/

        if(u_color < 20 || u_color == 100){
            fragColor = vec4(0, 0, 0, 0);
        }
        else{
            fragColor = vec4(colorOutput * coef, transpa);
        }

    }

}


void Mandelbrot(vec3 colorOutput){

    if(transpa == 1.0f){
        fragColor = vec4(mix(couleur.rgb * length(posVertex.xyz), colorOutput, 0.5) , transpa);
    }
    else{

        int u_color = nbBelongsTo(vec2(ddPos.x, ddPos.y) * 2.5, vec2(ddPos.x, ddPos.y) * 2.5 * (abs(cos(time)) + 0.2) * 2);


        float coef = 5.0/u_color; 

        coef = 1.5;

        /*float rIntValue = cos(u_color);
        float gIntValue = cos(u_color*1.1);
        float bIntValue = cos(u_color*1.2);*/

        if(u_color < 20 || u_color == 100){
            fragColor = vec4(0, 0, 0, 0);
        }
        else{
            fragColor = vec4(colorOutput * coef, transpa);
        }

    }


}



void main(void) {

    //vec3 lightPosition = vec3(0, 0, 1);

    vec3 ambient = light.ambient * couleur.rgb;

    vec3 normal = normalize(vsoNormal);

    vec3 lightDir = normalize(light.position - FragPos); /*vers le bas vers la gauche*/

    //vec3 lightDir = normalize(-light.direction); /*vers le bas vers la gauche*/

    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * couleur.rgb;

    //Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * couleur.rgb;


    //Attenuation
    float distance    = length(light.position.y - FragPos.y);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;

    //fragColor = vec4(result, transpa);

    //coef = 0;

    //float angle = abs(atan(tempoPos.x, tempoPos.z));

    //float mode2 = 5;

    if(mode == 0.0){
        transpaTime(result);
    }
    else if(mode == 4.0){
        circle(result);
        //trian(result);
    }
    else if(mode == 2.0){
        circle2(result);
    }
    else if(mode == 6.0){
        trian(result);
    }
    else if(mode == 3.0){
        quad(result);
    }
    else if(mode == 5.0){
        julia(result);
    }
    else if(mode == 1.0){
        Mandelbrot(result);
    }
    else{
        //fragColor = vec4(vec3(1,1,1)*dist, 1);
        fragColor = vec4(result, transpa);
    }
    //julia(result);
}
