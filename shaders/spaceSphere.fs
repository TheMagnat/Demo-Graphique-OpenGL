
/*
Fragment shader similaire au fragment shader principale mais avec une texture en plus.
*/

#version 330

//Toute les information sur la lumière
struct Light {
    //Position de la lumiere
    vec3 position;

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

uniform sampler2D tex;


in vec3 FragPos;
in vec3 vsoNormal;
in vec2 vsoTexCoord;


out vec4 fragColor;

void main(void) {

    //Calculs de lumière

    //vec4 tempoCouleur = texture(tex, vsoTexCoord);

    vec4 tempoCouleur = couleur;

    vec3 ambient = light.ambient * tempoCouleur.rgb;

    vec3 normal = normalize(vsoNormal);

    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * tempoCouleur.rgb;

    //Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * tempoCouleur.rgb;


    //Attenuation de la lumière en fonction de la distance
    float distance    = length(light.position.xyz - FragPos.xyz);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;

    fragColor = vec4(result, transpa);

}
