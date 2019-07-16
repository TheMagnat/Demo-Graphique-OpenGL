
/*

Projet programmation Graphique Guillaume Magniadas

Note :  Il y à certaine partie du code qui aurait pu être mieux écrite pour mieux tirer profit des avantage du C++,
        En commencant ce projet je me suis trop vite lancé, je pense que j'aurais pu ecrire plus de classe pour améliorer la lisibilité du code,
        et le rendre plus maléable.

*/




#include "head.hpp"

//Le Chrono
#include <chrono>

//Mes fichiers
#include "SlowCube.hpp"
#include "Particles.hpp"
#include "Mobiles.hpp"

//Les fonction 3D
#include "3Dfunction.hpp"


//Temps
std::chrono::time_point<std::chrono::system_clock> last, now, startAnim, startProg;

//Taille de fenetre
static unsigned int width_f, height_f;

//Shader
static GLuint _pId = 0;
static GLuint _pidSky = 0;
static GLuint _pidSpace = 0;
static GLuint _pidSpaceMonte = 0;
static GLuint _pidStars = 0;
static GLuint _blurV = 0;
static GLuint _blurH = 0;
static GLuint _postProd = 0;
static GLuint _particuleShad = 0;
static GLuint _creditShad = 0;
static GLuint _voronoiShad = 0;
static GLuint _pidSphere = 0;
static GLuint _pidBlack = 0;
static GLuint _godLight = 0;
static GLuint _godLight2 = 0;


//static Shader testShad;

//Frame buffer
unsigned int fbo1 = 0, fbo2 = 0;
	//Texture de framebuffer color
unsigned int texColBuff = 0, texColBuff2 = 0;

	//Depth et stencil buffer tempon buffer
unsigned int rbo = 0;



//Couleurs
GLfloat cyan[] = {50.f/255.f, 1, 1, 1};

static std::array<float, 3> colors {1.0f, 0.0f, 1.0f};

//Objet GL4D
static GLuint sphere_ = 0;

//vertices
static float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

//Fond square
static float spaceVertices[] = {
    //Position,       Texture
    -1.f, -1.f, 0.f, 0.f, 0.f,
    -1.f, 1.f, 0.f, 0.f, 1.f,
    1.f, 1.f, 0.f, 1.f, 1.f,
    1.f, 1.f, 0.f, 1.f, 1.f,
    1.f, -1.f, 0.f, 1.f, 0.f,
    -1.f, -1.f, 0.f, 0.f, 0.f
};

//Variable Global OpenGL

//Save VAO VBO
static unsigned int VBO, VAO, skyVBO, skyVAO, spaceVBO, spaceVAO;

//data de forme
static std::array<float, 216> maForme;
//save des formes
static std::vector<unsigned int> allFormes;
static std::vector<unsigned int> allFormesVBO;

//Variable Global Texture

static unsigned int cubemapTexture;
static std::vector<std::string> faces{
    "texture/right.tga",
    "texture/left.tga",
    "texture/top.tga",
    "texture/bottom.tga",
    "texture/front.tga",
    "texture/back.tga"
};

static unsigned int textureSpace;
static std::string fileSpace = "texture/bottom.tga";

//Texture des mobiles 1D
static unsigned int textureMobiles;


//Variable Global Music

#define ECHANTILLONS 1024
static Sint16 _hauteurs[ECHANTILLONS];

/*!\brief pointeur vers la musique chargée par SDL_Mixer */
static Mix_Music * _mmusic = NULL;
/*!\brief données entrées/sorties pour la lib fftw */
static fftw_complex * _in4fftw = NULL, * _out4fftw = NULL;
/*!\brief donnée à précalculée utile à la lib fftw */
static fftw_plan _plan4fftw = NULL;


//Variable Global Fonts
static unsigned int _textTexId = 0;

//Fonctions

static Point p1(50, 50, 0.25, 0.5, 0.4);
static Point p2(50, 500, 0.25, 0.5, 0.4);
static Point p3(500, 500, 0.25, 0.5, 0.4);
static Point p4(500, 50, 0.25, 0.5, 0.4);

/**
* Fonction exécuté à la fin du programme.
**/
static void quitte(void) {

    //Je ne suis pas sur de bien delete tout ce qu'il faut par defaut de temps..
    glDeleteVertexArrays(1, &skyVAO);
    glDeleteBuffers(1, &skyVBO);

    glDeleteVertexArrays(1, &spaceVAO);
    glDeleteBuffers(1, &spaceVBO);

    glDeleteFramebuffers(1, &fbo1);
    glDeleteFramebuffers(1, &fbo2);

    glDeleteRenderbuffers(1, &rbo);

    //On libère les texture des fonts
    if(_textTexId) {
        glDeleteTextures(1, &_textTexId);
        _textTexId = 0;
    }
    if(textureMobiles){
        glDeleteTextures(1, &textureMobiles);
        textureMobiles = 0;
    }
    if(textureSpace){
        glDeleteTextures(1, &textureSpace);
        textureSpace = 0;
    }
    if(cubemapTexture){
        glDeleteTextures(1, &cubemapTexture);
        cubemapTexture = 0;
    }

    for(size_t i(0); i < allFormes.size(); ++i){
        glDeleteVertexArrays(1, &allFormes[i]);
    }

    for(size_t i(0); i < allFormesVBO.size(); ++i){
        glDeleteBuffers(1, &allFormesVBO[i]);
    }

    gl4duClean(GL4DU_ALL);
}


static size_t pourcentActual = 0;
static std::array<bool, 4> pourcentAuto;
static std::array<float, 4> pourcent;

/**
*   Fonction servant à dessin, permetant de faire grandire le pourcentage affiché de chaque coté de la forme 2D jusqu'a tous les avoir à 100%.
**/
static void growPourcent(float step){

    for(size_t i(0); i < 4; ++i){


        if(pourcent[i] != 1.f){
            pourcent[i] += step;
            if(pourcent[i] >= 1.0f){
                pourcent[i] = 1.0f;
                pourcentActual = i+1;
            }
            break;
        }

    }

}

/**
*   Fonction qui génère un quadrilatère 2D de facon aléatoire.
**/
static void newPoint(Point& a1, Point& a2, Point& a3, Point& a4){
    /*
    a1 = Point(rand()%(width_f/2) + width_f/4, rand()%(height_f/2) + height_f/4, 1, 1, 1);
    a2 = Point(rand()%(width_f/2) + width_f/4, rand()%(height_f/2) + height_f/4, 1, 1, 1);
    a3 = Point(rand()%(width_f/2) + width_f/4, rand()%(height_f/2) + height_f/4, 1, 1, 1);
    a4 = Point(rand()%(width_f/2) + width_f/4, rand()%(height_f/2) + height_f/4, 1, 1, 1);
    */
    a1 = Point(rand()%(width_f/4) + width_f/4, rand()%(height_f/4) + height_f/4, 1, 1, 1);
    a2 = Point(rand()%(width_f/4) + width_f/4, rand()%(height_f/4) + height_f/2, 1, 1, 1);
    a3 = Point(rand()%(width_f/4) + width_f/2, rand()%(height_f/4) + height_f/2, 1, 1, 1);
    a4 = Point(rand()%(width_f/4) + width_f/2, rand()%(height_f/4) + height_f/4, 1, 1, 1);

}

//Les étoiles
static std::vector<glm::vec3> posStar;
static std::vector<glm::vec3> posStarTwo;
static std::vector<glm::vec3> posStarTree;

/**
*   Fonction qui remplis les position de nb étoiles dans fillPosStar,
*   le tout compris entre minWidth et maxWidth pour l'axe X, et pareil pour les 2 dernier axes.
**/
static void generateStars(std::vector<glm::vec3>& fillPosStar, int nb, int minWidth, int maxWidth, int minHeight, int maxHeight, int minDepth, int maxDepth){

	for(size_t i(0); i < nb; ++i){
		fillPosStar.emplace_back(rand()%(maxWidth - minWidth) + minWidth, rand()%(maxHeight - minHeight) + minHeight, rand()%(maxDepth - minDepth) + minDepth);
	}

}

//Variable de calcul
static float accele = 0;
static float accele2 = 0;
static float hight = 0;
static float angle = 0;


//static std::array<float, 3> clearColor;
static glm::vec3 clearColor;

static glm::vec3 ambient = glm::vec3(0.1f);
static glm::vec3 specular = glm::vec3(0.1f);

static float transpa = 0.0f;

static Particles mesParticules;

static Mobiles mesMobiles;

/**
*   Boucle graphique faisant office de crédit.
*   Affiche les crédit sur un quadrilatère texturé par ces dernier qui bouge dans un framebuffer,
*   puis traite la texture obtenue dans un shader affichant une certaine version de voronoi.
*   La demo se termine une fois les crédit partie et quelque secondes après.
**/
static void dessinVoronoi(void){

    //Calculs de temps
    now = std::chrono::system_clock::now();

    std::chrono::duration<double, std::ratio<1,1>> delta = now - last;

    last = now;

    float elapsedTime(delta.count());

    float timeFromStart(std::chrono::duration<double, std::ratio<1,1>>(std::chrono::system_clock::now() - startAnim).count());
    //

    mesMobiles.move(elapsedTime);

    //On change de frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, fbo1);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Décommenter pour un effet sympathique
    /*glDepthMask(GL_FALSE);
    glUseProgram(_pidSpace);

    glUniform1f(glGetUniformLocation(_pidSpace, "temps"), timeFromStart);
    glUniform1f(glGetUniformLocation(_pidSpace, "angle2"), 0);
    glUniform1f(glGetUniformLocation(_pidSpace, "hight"), 0);

    clearColor[0] = 1.0f;
    glUniform3fv(glGetUniformLocation(_pidSpace, "clearColor"), 1, &clearColor[0]);
    glUniform1f(glGetUniformLocation(_pidSpace, "finAll"), 0.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureSpace);

    glBindVertexArray(spaceVAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDepthMask(GL_TRUE);*/
    //


    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(_creditShad);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width_f / (float)height_f, 0.1f, 50.0f);
    glUniformMatrix4fv(glGetUniformLocation(_creditShad, "projection"), 1, GL_FALSE, &projection[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textTexId);

    glUniform1i(glGetUniformLocation(_creditShad, "tex"), 0);

    gl4duBindMatrix("modelViewMatrix");

    gl4duLoadIdentityf();

    static bool end = false;

    float scaleCoef = abs(tan(timeFromStart/10.0f))*2;

    if(timeFromStart > 5.0f && scaleCoef < 0.01f){
        if(!end){
            end = true;
            timeFromStart = 0.0f;
            startAnim = now;
        }
    }

    if(!end){
        gl4duScalef(scaleCoef*2, scaleCoef, 1);
    }
    else{
        gl4duScalef(0, 0, 0);
    }
    //gl4duTranslatef(0.033, 0.10, -5.0);
    gl4duTranslatef(0.f, 0.f, -5.5f);
    gl4duRotatef(abs(tan(timeFromStart/50.f)*360), 1, 1, 1);
    gl4duSendMatrices();

    glBindVertexArray(spaceVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);




    //On dessine maintenant sur l'écran
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    //Shaders pour le voronoi
    glUseProgram(_voronoiShad);

    std::vector<float> texture1Data;
    size_t mobilesSize(mesMobiles.fillTexture(texture1Data));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texColBuff);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, textureMobiles);

    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, mobilesSize, 0, GL_RGBA, GL_FLOAT, texture1Data.data());

    glUniform1i(glGetUniformLocation(_voronoiShad, "screenTex"), 0);
    glUniform1i(glGetUniformLocation(_voronoiShad, "mobileTex"), 1);
    glUniform1f(glGetUniformLocation(_voronoiShad, "step"), (1.0f / mobilesSize));

    glUniform1f(glGetUniformLocation(_voronoiShad, "time"), timeFromStart);

    glBindVertexArray(spaceVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    if(end && timeFromStart > 3.0f){
        float timeFromStartProg(std::chrono::duration<double, std::ratio<1,1>>(std::chrono::system_clock::now() - startProg).count());
        std::cout << "Temps total : " << timeFromStartProg << std::endl;
        exit(0);
    }

}

static float mode = -1;

static bool finAll = false;
static size_t finVal;
/**
*   Boucle graphique finale qui reprend toute les formes déssiné au cours de dessin3D (les version 3D de dessin)
*   et les affiche à la chaine sous la forme d'une spirale infini. 3 paques de sphere sont aussi déssiné.
*   La camera de cette boucle avance à l'infini et a chaque fois qu'un paques de sphère passe dérière, ses spheres sont régénéré derriere les deux autres.
*   Une fois que l'angle de vue atteint une valeur fixé, commence à effacer certaint element de la scene jisqi'a obtenir un ecran noire.
*   Une fois l'ecran noire obtenue, après quelque secondes lance la boucle dessinVoronoi.
**/
static void dessinAll(void){

    //Calculs de temps

    now = std::chrono::system_clock::now();

    std::chrono::duration<double, std::ratio<1,1>> delta = now - last;

    last = now;

    float elapsedTime(delta.count());

    float timeFromStart(std::chrono::duration<double, std::ratio<1,1>>(std::chrono::system_clock::now() - startAnim).count());

    Sint16 tempo = _hauteurs[2];

    if(!finAll){

        //Constante en fonction des basses
        if(tempo > 40){
            //Couleur du clear
            clearColor[0] = 1.0f;
            clearColor[1] = 0.0f;
            clearColor[2] = 1.0f;

            //Couleur de la lumière ambient
            ambient[0] = 1.0f;
            ambient[1] = 0.0f;
            ambient[2] = 1.0f;

            specular[0] = 0.35f;
            specular[1] = 0.0f;
            specular[2] = 0.5f;

            //Transparence de la piece centrale
            transpa = 0.75f;
        }

        for(size_t i(0); i < 3; ++i){
            clearColor[i] += 0.015f;
            if(clearColor[i] > 1.0f) clearColor[i] = 1.0f;

            ambient[i] -= 0.010f;
            if(ambient[i] < 0.1f) ambient[i] = 0.1f;

            specular[i] += 0.015f;
            if(specular[i] > 1.0f) specular[i] = 1.0f;

        }

        transpa -= 0.025f;
        if(transpa < 0) transpa = 0.0f;

    }
    else{
        for(size_t i(0); i < 3; ++i){
            clearColor[i] -= 0.015f;
            if(clearColor[i] < 0.0f) clearColor[i] = 0.0f;

            ambient[i] -= 0.010f;
            if(ambient[i] < 0.1f) ambient[i] = 0.1f;

            specular[i] += 0.015f;
            if(specular[i] > 1.0f) specular[i] = 1.0f;

        }

    }


    //Scale de la forme
    tempo = _hauteurs[2];
    if(tempo > 0 && hight < (0.5*sqrt(tempo)*(0.5*0.5)*16)){
        accele = sqrt(tempo);
    }
    accele -= elapsedTime * 6.f;

    hight = 0.5*accele*(0.5*0.5)+hight;
    if(hight <= 0.5) hight = 0;
    if(hight == 0) accele = 0;




    //Rotation de la forme
    tempo = _hauteurs[32];
    if(tempo > 0 && accele2 < (0.5*tempo/3*(0.5*0.5)*16)){
        accele2 = tempo/3;
    }
    accele2 -= elapsedTime * 0.75f;

    if(accele2 <= 0) accele2 = 0;

    angle += 0.5*accele2*(0.5*0.5);

    glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //On s'occupe du fond
    glDepthMask(GL_FALSE);
    glUseProgram(_pidSpace);

    glUniform1f(glGetUniformLocation(_pidSpace, "temps"), timeFromStart);
    glUniform1f(glGetUniformLocation(_pidSpace, "angle2"), angle);
    glUniform1f(glGetUniformLocation(_pidSpace, "hight"), hight);

    glUniform3fv(glGetUniformLocation(_pidSpace, "clearColor"), 1, &clearColor[0]);
    glUniform1f(glGetUniformLocation(_pidSpace, "finAll"), finAll == true ? 1.0f : 0.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureSpace);

    glBindVertexArray(spaceVAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDepthMask(GL_TRUE);


    //Dessin des pieces
    glUseProgram(_pId);

    glUniform1f(glGetUniformLocation(_pId, "time"), timeFromStart);
    glUniform1f(glGetUniformLocation(_pId, "mode"), -1.0f);


    //Lumière
    glUniform3fv(glGetUniformLocation(_pId, "light.ambient"), 1, &ambient[0]);

    glUniform3f(glGetUniformLocation(_pId, "light.specular"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(_pId, "light.diffuse"), 1.f, 1.f, 1.f);

    //Attenuation
    glUniform1f(glGetUniformLocation(_pId, "light.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(_pId, "light.linear"), 0.00045f);
    glUniform1f(glGetUniformLocation(_pId, "light.quadratic"), 0.000075f);


    glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, cyan);


    //Camera    
    static float tempoY = -15.f;
    tempoY -= elapsedTime * 60.f;
    glm::vec3 viewPos(0.f, tempoY, 0.f);


    glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0.0, -1.0, 0.0) + viewPos, glm::vec3(0.0, 0.0, -1.0));
    glUniformMatrix4fv(glGetUniformLocation(_pId, "view"), 1, GL_FALSE, &view[0][0]);

    glUniform3fv(glGetUniformLocation(_pId, "viewPos"), 1, &viewPos[0]);

    glUniform3fv(glGetUniformLocation(_pId, "light.position"), 1, &viewPos[0]);

    //Projection
    static float viewAngle = 0.f;
    viewAngle += elapsedTime * 3.f;

    if(viewAngle >= 175.f){
        viewAngle = 175.f;
        if(finAll == false){
            finAll = true;
            finVal = static_cast<size_t>(((-1*(tempoY+15))+1000)/(allFormes.size() * 2)) - 25;
        }
    }

    glm::mat4 projection = glm::perspective(glm::radians(viewAngle), (float)width_f / (float)height_f, 0.1f, 5000.0f);
    glUniformMatrix4fv(glGetUniformLocation(_pId, "projection"), 1, GL_FALSE, &projection[0][0]);

    //Uniform
    glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, cyan);
    glUniform1f(glGetUniformLocation(_pId, "transpa"), 1.f);

    //Model
    gl4duBindMatrix("modelViewMatrix");
    gl4duLoadIdentityf();

    size_t taille(allFormes.size());


    size_t prof1Ite(taille*2);

    size_t maxIte;

    if(finAll){
        maxIte = finVal;
    }
    else{
        maxIte = static_cast<size_t>(((-1*(tempoY+15))+1000)/prof1Ite);
    }

    for(size_t i(static_cast<size_t>(((-1*(tempoY+15))/prof1Ite))), it(i*taille); i < maxIte; ++i){
        for(; it < taille * (i+1); ++it){
            gl4duPushMatrix();

            gl4duTranslatef(0, -2.f * it, 0.f);

            gl4duRotatef(360 * ((((it) - taille * i) + 0.f)/((float)taille)), 0.f, 1.f, 0.f);
            gl4duTranslatef(0, 0, (-5.f - hight));

            gl4duRotatef(-angle, -1, 1, 0);
            gl4duRotatef(timeFromStart*75.0f, -1, 0, 0);

            glBindVertexArray(allFormes[(it - taille * i)]);

            gl4duScalef(2.f, 2.f, 2.f);

            gl4duSendMatrices();

            glUniform1f(glGetUniformLocation(_pId, "transpa"), transpa);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, 36);


            gl4duScalef(hight/30.f + 1.f, hight/30.f + 1.f, hight/30.f + 1.f);

            gl4duSendMatrices();

            glUniform1f(glGetUniformLocation(_pId, "transpa"), 1.f);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            gl4duPopMatrix();
        }
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Etoiles

    glUseProgram(_pidStars);

    //Lumière
    glUniform3fv(glGetUniformLocation(_pId, "light.ambient"), 1, &ambient[0]);

    glUniform3f(glGetUniformLocation(_pId, "light.specular"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(_pId, "light.diffuse"), 1.f, 1.f, 1.f);

    //Attenuation
    glUniform1f(glGetUniformLocation(_pId, "light.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(_pId, "light.linear"), 0.022f);
    glUniform1f(glGetUniformLocation(_pId, "light.quadratic"), 0.0019f);



    static uint8_t lowest = 0;
    static int nextStep = -100;

    if(!finAll){

        if(tempoY <= nextStep){
        	if(lowest == 0){
        		posStar.clear();
        		generateStars(posStar, 500, -50, 50, nextStep - 200, nextStep - 300, -50, 50);
        		lowest = 1;
        	}
        	else if(lowest == 1){
        		posStarTwo.clear();
        		generateStars(posStarTwo, 500, -50, 50, nextStep - 200, nextStep - 300, -50, 50);
        		lowest = 2;
        	}
        	else{
        		posStarTree.clear();
        		generateStars(posStarTree, 500, -50, 50, nextStep - 200, nextStep - 300, -50, 50);
        		lowest = 0;
        	}

        	nextStep -= 100;

        }
    }

    glUniformMatrix4fv(glGetUniformLocation(_pId, "view"), 1, GL_FALSE, &view[0][0]);

    glUniform3fv(glGetUniformLocation(_pId, "viewPos"), 1, &viewPos[0]);

    glUniform3fv(glGetUniformLocation(_pId, "light.position"), 1, &viewPos[0]);

    glUniformMatrix4fv(glGetUniformLocation(_pId, "projection"), 1, GL_FALSE, &projection[0][0]);

    //Uniform
    glUniform4f(glGetUniformLocation(_pId, "couleur"), 1, 1, 1, 1);
    glUniform1f(glGetUniformLocation(_pId, "transpa"), 1.f);

    //Model
    gl4duBindMatrix("modelViewMatrix");

    for(size_t i(0), taille(posStar.size()); i < taille; ++i){
    	gl4duLoadIdentityf();

    	gl4duTranslatef(posStar[i].x, posStar[i].y, posStar[i].z);
    	gl4duScalef(0.15f, 0.15f, 0.15f);

    	gl4duSendMatrices();

    	gl4dgDraw(sphere_);

    	gl4duLoadIdentityf();

    	gl4duTranslatef(posStarTwo[i].x, posStarTwo[i].y, posStarTwo[i].z);
    	gl4duScalef(0.15f, 0.15f, 0.15f);

    	gl4duSendMatrices();

    	gl4dgDraw(sphere_);

    	gl4duLoadIdentityf();

    	gl4duTranslatef(posStarTree[i].x+10, posStarTree[i].y+10, posStarTree[i].z+10);
    	gl4duScalef(0.15f, 0.15f, 0.15f);

    	gl4duSendMatrices();

    	gl4dgDraw(sphere_);
    }

    static float transpa2 = 1.0f;

   	transpa2 -= elapsedTime * 1.0f;
   	if(transpa2 < 0.0f) transpa2 = 0.0f;



    //On dissine un ecran noir pour les transitions
    glUseProgram(_pidBlack);

    glBindVertexArray(spaceVAO);

    glUniform4f(glGetUniformLocation(_pidBlack, "color"), 0, 0, 0, transpa2);

    glDrawArrays(GL_TRIANGLES, 0, 6);



    if(timeFromStart > 66.0f){
        startAnim = std::chrono::system_clock::now();
        gl4duwDisplayFunc(dessinVoronoi);
    }

}

/**
*   Ancienne boucle graphique plus utilisé, elle était prévu à la base a la place de dessinAllStars,
*   Mais ressemble trop à dessinAll avec des choses en moins, reste intéréssante.
**/
static void dessinMonte(void){

    //Calculs de temps

    now = std::chrono::system_clock::now();

    std::chrono::duration<double, std::ratio<1,1>> delta = now - last;

    last = now;

    float elapsedTime(delta.count());

    float timeFromStart(std::chrono::duration<double, std::ratio<1,1>>(std::chrono::system_clock::now() - startAnim).count());

    Sint16 tempo = _hauteurs[2];

    //Constante en fonction des basses
    if(tempo > 40){
        //Couleur du clear
        clearColor[0] = 1.0f;
        clearColor[1] = 0.0f;
        clearColor[2] = 1.0f;

        //Couleur de la lumière ambient
        ambient[0] = 1.0f;
        ambient[1] = 0.0f;
        ambient[2] = 1.0f;

        //Transparence de la piece centrale
        transpa = 0.75f;
    }

    for(size_t i(0); i < 3; ++i){
        clearColor[i] += 0.015f;
        if(clearColor[i] > 1.0f) clearColor[i] = 1.0f;

        ambient[i] -= 0.010f;
        if(ambient[i] < 0.1f) ambient[i] = 0.1f;

        specular[i] += 0.015f;
        if(specular[i] > 1.0f) specular[i] = 1.0f;
    }

    transpa -= 0.025f;
    if(transpa < 0) transpa = 0.0f;


        //Scale de la forme
    tempo = _hauteurs[2];
    if(tempo > 0 && hight < (0.5*sqrt(tempo)*(0.5*0.5)*16)){
        accele = sqrt(tempo);
    }
    accele -= elapsedTime * 6.f;

    hight = 0.5*accele*(0.5*0.5)+hight;
    if(hight <= 0.5) hight = 0;
    if(hight == 0) accele = 0;




    //Rotation de la forme
    tempo = _hauteurs[32];
    if(tempo > 0 && accele2 < (0.5*tempo/3*(0.5*0.5)*16)){
        accele2 = tempo/3;
    }
    accele2 -= elapsedTime * 1.5f;

    if(accele2 <= 0) accele2 = 0;

    angle += 0.5*accele2*(0.5*0.5);

    glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //On s'occupe du fond
    glDepthMask(GL_FALSE);
    glUseProgram(_pidSpaceMonte);


    glUniform1f(glGetUniformLocation(_pidSpaceMonte, "temps"), 40.f - timeFromStart);
    glUniform1f(glGetUniformLocation(_pidSpaceMonte, "angle2"), angle);
    glUniform1f(glGetUniformLocation(_pidSpaceMonte, "hight"), hight);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureSpace);

    glBindVertexArray(spaceVAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDepthMask(GL_TRUE);


    //Dessin des pieces
    glUseProgram(_pidSpaceMonte);

    //Lumière

    glUniform3fv(glGetUniformLocation(_pId, "light.ambient"), 1, &ambient[0]);

    glUniform3f(glGetUniformLocation(_pId, "light.specular"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(_pId, "light.diffuse"), 1.f, 1.f, 1.f);

    //Attenuation
    glUniform1f(glGetUniformLocation(_pId, "light.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(_pId, "light.linear"), 0.0045f);
    glUniform1f(glGetUniformLocation(_pId, "light.quadratic"), 0.00075f);


    glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, cyan);




    //CALCUL POUR LES DEPLACEMENT





    //FIN CALCULS

    
    static float tempoY = -15.f;
    tempoY -= elapsedTime * 60.f;
    glm::vec3 viewPos(0.f, tempoY, 0.f);
    glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0.0, -1.0, 0.0) + viewPos, glm::vec3(0.0, 0.0, -1.0));
    glUniformMatrix4fv(glGetUniformLocation(_pId, "view"), 1, GL_FALSE, &view[0][0]);

    glUniform3fv(glGetUniformLocation(_pId, "viewPos"), 1, &viewPos[0]);

    glUniform3fv(glGetUniformLocation(_pId, "light.position"), 1, &viewPos[0]);

    //Projection
    static float viewAngle = 0.f;
    viewAngle += elapsedTime * 3.f;
    if(viewAngle >= 175.f) viewAngle = 175.f;
    glm::mat4 projection = glm::perspective(glm::radians(viewAngle), (float)width_f / (float)height_f, 0.1f, 5000.0f);
    glUniformMatrix4fv(glGetUniformLocation(_pId, "projection"), 1, GL_FALSE, &projection[0][0]);

    //Uniform
    glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, cyan);
    glUniform1f(glGetUniformLocation(_pId, "transpa"), 1.f);

    //Model
    gl4duBindMatrix("modelViewMatrix");
    gl4duLoadIdentityf();


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if(timeFromStart >= 40.f){
        startAnim = std::chrono::system_clock::now();
        gl4duwDisplayFunc(dessin);
    }

}

/**
*   Boucle graphique qui dessine 3 paques de plusieurs sphere dispersé de manière aléatoire,
*   La camera de cette boucle recule perpetuellement, et a chaque fois qu'un des paque est assez éloigné, regenère ses sphere et le replace en avant.
*   Des particules sont aussi généré au cours de cette boucle.
*   Le tout est ensuité flouté et mixé avec un avant après pour donner un effet de brillance.
*   Une fois un temps fixé passé, commence à ne plus rien generer, puis passe à la boucle dessin.
**/
static void dessinAllStars(void){

    //Calculs de temps

    now = std::chrono::system_clock::now();

    std::chrono::duration<double, std::ratio<1,1>> delta = now - last;

    last = now;

    float elapsedTime(delta.count());

    float timeFromStart(std::chrono::duration<double, std::ratio<1,1>>(std::chrono::system_clock::now() - startAnim).count());

    Sint16 tempo = _hauteurs[2];

    //Constante en fonction des basses
    if(tempo > 40){

        //Couleur de la lumière ambient
        ambient[0] = 1.0f;
        ambient[1] = 0.0f;
        ambient[2] = 1.0f;

        //Transparence de la piece centrale
        transpa = 0.75f;
    }

    for(size_t i(0); i < 3; ++i){

        ambient[i] -= 0.010f;
        if(ambient[i] < 0.1f) ambient[i] = 0.1f;

        specular[i] += 0.015f;
        if(specular[i] > 1.0f) specular[i] = 1.0f;

    }

    transpa -= 0.025f;
    if(transpa < 0) transpa = 0.0f;


    //On change de framebuffer avant tout
    glBindFramebuffer(GL_FRAMEBUFFER, fbo1);

    glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //Dessin des pieces
    glUseProgram(_pidStars);

    //Lumière
    glUniform3fv(glGetUniformLocation(_pidStars, "light.ambient"), 1, &ambient[0]);

    glUniform3f(glGetUniformLocation(_pidStars, "light.specular"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(_pidStars, "light.diffuse"), 1.f, 1.f, 1.f);

    //Attenuation
    glUniform1f(glGetUniformLocation(_pidStars, "light.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(_pidStars, "light.linear"), 0.022f);
    glUniform1f(glGetUniformLocation(_pidStars, "light.quadratic"), 0.0019f);


    static uint8_t lowest = 0;
    static int nextStep = 100;
    static float tempoZ = 0.f;

    float tempoX = cos(timeFromStart)*10.f;
    float tempoY = sin(timeFromStart)*10.f;
    tempoZ += elapsedTime * 30.f;
    glm::vec3 viewPos(tempoX, tempoY, tempoZ);


    if(timeFromStart >= 12.5f){

    }
    else{
        if(tempoZ >= nextStep){
        	if(lowest == 0){
        		posStar.clear();
        		generateStars(posStar, 500, -50, 50, -50, 50, nextStep, nextStep + 100);
        		lowest = 1;
        	}
        	else if(lowest == 1){
        		posStarTwo.clear();
        		generateStars(posStarTwo, 500, -50, 50, -50, 50, nextStep, nextStep + 100);
        		lowest = 2;
        	}
        	else{
        		posStarTree.clear();
        		generateStars(posStarTree, 500, -50, 50, -50, 50, nextStep, nextStep + 100);
        		lowest = 0;
        	}

        	nextStep += 100;

        }
    }

    if(timeFromStart < 15.0f){
        //Ajout de particules
        int nbNewParticles = (int)(elapsedTime*150.0);

        //Pour eviter les paté de particules en cas de petit freeze
        if(nbNewParticles > (int)(0.016f*150.0)) nbNewParticles = (int)(0.016f*150.0);


        for(size_t i(0); i < nbNewParticles; ++i){

            //Dispertion des particules inspiré des cours de http://www.opengl-tutorial.org sur les particules
            float spread = 1.5f;
            glm::vec3 maindir = glm::vec3(0.0f, 0.0f, 2.5f);
            glm::vec3 randomdir = glm::vec3(
                (rand()%2000 - 1000.0f)/250.0f,
                (rand()%2000 - 1000.0f)/250.0f,
                (rand()%2000 - 1000.0f)/1000.0f
            );
            
            glm::vec3 pos(glm::vec3(rand()%40 - 20, rand()%40 - 20, tempoZ - 5.f));

            glm::vec4 color((rand()%256)/256.f, (rand()%256)/256.f, (rand()%256)/256.f, (rand()%256)/256.f);

            mesParticules.add(pos, maindir + randomdir*spread, color, (rand()%100)/100.f * 0.3f + 0.05f, 0, 0, 5.0f);

        }
    }

    glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0.0, 0.0, -40.0f + tempoZ), glm::vec3(0.0, 1.f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(_pidStars, "view"), 1, GL_FALSE, &view[0][0]);

    glUniform3fv(glGetUniformLocation(_pidStars, "viewPos"), 1, &viewPos[0]);

    glUniform3fv(glGetUniformLocation(_pidStars, "light.position"), 1, &viewPos[0]);

    //Projection
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width_f / (float)height_f, 0.1f, 5000.0f);
    glUniformMatrix4fv(glGetUniformLocation(_pidStars, "projection"), 1, GL_FALSE, &projection[0][0]);

    //Uniform
    glUniform4f(glGetUniformLocation(_pidStars, "couleur"), 1, 1, 1, 1);
    glUniform1f(glGetUniformLocation(_pidStars, "transpa"), 1.f);

    //Model
    gl4duBindMatrix("modelViewMatrix");

    for(size_t i(0), taille(posStar.size()); i < taille; ++i){
    	gl4duLoadIdentityf();

    	gl4duTranslatef(posStar[i].x, posStar[i].y, posStar[i].z);
    	gl4duScalef(0.25f, 0.25f, 0.25f);

    	gl4duSendMatrices();

    	gl4dgDraw(sphere_);

    	gl4duLoadIdentityf();

    	gl4duTranslatef(posStarTwo[i].x, posStarTwo[i].y, posStarTwo[i].z);
    	gl4duScalef(0.25f, 0.25f, 0.25f);

    	gl4duSendMatrices();

    	gl4dgDraw(sphere_);

    	gl4duLoadIdentityf();

    	gl4duTranslatef(posStarTree[i].x+10, posStarTree[i].y+10, posStarTree[i].z+10);
    	gl4duScalef(0.25f, 0.25f, 0.25f);

    	gl4duSendMatrices();

    	gl4dgDraw(sphere_);
    }


    // ----Particules

    mesParticules.update(elapsedTime, viewPos);

    mesParticules.draw(view, projection);

    //Une fois tout déssiné, on produit le resultat final
    glDisable(GL_DEPTH_TEST);

    //On utilise finalement le flou de gl4d, parfait pour notre utilisation
    gl4dfBlur(texColBuff, texColBuff2, 15, 1, 0, GL_FALSE);

    //On bind une fois la forme, ca suffi.
    glBindVertexArray(spaceVAO);

    //Flou verticale
    /*
    glBindFramebuffer(GL_FRAMEBUFFER, fbo2);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT);

	//Shaders pour le flou verti
	glUseProgram(_blurV);

	glUniform1f(glGetUniformLocation(_blurV, "sigma"), 3);
	glUniform1f(glGetUniformLocation(_blurV, "blurSize"), 1.f/height_f);

	glBindTexture(GL_TEXTURE_2D, texColBuff);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Flou horizontale
	glUseProgram(_blurH);

	glUniform1f(glGetUniformLocation(_blurH, "sigma"), 3);
	glUniform1f(glGetUniformLocation(_blurH, "blurSize"), 1.f/width_f);

	glBindTexture(GL_TEXTURE_2D, texColBuff2);
	glDrawArrays(GL_TRIANGLES, 0, 6);*/

	//	----- Ecran -----

	//Et on déssine sur l'ecran
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	//Shaders pour le quad
	glUseProgram(_postProd);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texColBuff);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texColBuff2);

	glUniform1i(glGetUniformLocation(_postProd, "screenTexture"), 0);
	glUniform1i(glGetUniformLocation(_postProd, "blurTexture"), 1);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glEnable(GL_DEPTH_TEST);




    if(timeFromStart >= 20.f){
        startAnim = std::chrono::system_clock::now();
        gl4duwDisplayFunc(dessin);
    }

}

static float camePos2 = 0.f;

/**
*   Boucle de graphique de transition entre dessin3D et dessinAll.
*   Dessine une sphere texturé avec un fond spacial et agrandi la sphere jusqu'a atteindre l'intérieur de celle ci.
*   Passe à dessinAll quand celui ci est atteint. 
**/
static void dessinTransition(void){

	static float transpa2 = 1.0f;
	static bool intro = true;

    //Calculs de temps

    now = std::chrono::system_clock::now();

    std::chrono::duration<float, std::ratio<1,1>> delta = now - last;

    last = now;

    float elapsedTime(delta.count());

    float timeFromStart(std::chrono::duration<float, std::ratio<1,1>>(now - startAnim).count());


    //On change de framebuffer avant tout
    glBindFramebuffer(GL_FRAMEBUFFER, fbo1);

    //
    Sint16 tempo = _hauteurs[2];

    if(tempo > 40){

        //Couleur de la lumière ambient
        ambient[0] = colors[0];
        ambient[1] = colors[1];
        ambient[2] = colors[2];

    }

    float toAdd(elapsedTime * 0.90f);

    for(size_t i(0); i < 3; ++i){
        ambient[i] -= toAdd;
        if(ambient[i] < 0.1f) ambient[i] = 0.1f;
    }

    //Scale de la forme
    if(tempo > 0 && hight < (0.5*sqrt(tempo)*(0.5*0.5)*16)){
        accele = sqrt(tempo);
    }
    accele -= elapsedTime * 30.f;

    hight = 0.5*accele*(0.5*0.5)+hight;
    if(hight <= 0.5) hight = 0;
    if(hight == 0) accele = 0;




    //Rotation de la forme
    tempo = _hauteurs[32];
    if(tempo > 0 && accele2 < (0.5*tempo/3*(0.5*0.5)*16)){
        accele2 = tempo/3;
    }
    accele2 -= elapsedTime * 1.0f;

    if(accele2 <= 0) accele2 = 0;

    angle += 0.5*accele2;



    glUseProgram(_pidSphere);

    glUniform3fv(glGetUniformLocation(_pidSphere, "light.ambient"), 1, &ambient[0]);

    glUniform3f(glGetUniformLocation(_pidSphere, "light.specular"), 0.5f, 0.5f, 0.5f);
    glUniform3f(glGetUniformLocation(_pidSphere, "light.diffuse"), 1.f, 1.f, 1.f);

    //Attenuation
    glUniform1f(glGetUniformLocation(_pidSphere, "light.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(_pidSphere, "light.linear"), 0.022f);
    glUniform1f(glGetUniformLocation(_pidSphere, "light.quadratic"), 0.0019f);


    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //Camera
    camePos2 += elapsedTime * 30.f;

    float radius = 10.0f;
    float camX = sin((camePos2)/50.f) * radius;
    float camZ = cos((camePos2)/50.f) * radius;

    glm::vec3 viewPos(camX, 0.0, camZ);

    glUniform3fv(glGetUniformLocation(_pidSphere, "light.position"), 1, &viewPos[0]);


    glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(_pidSphere, "view"), 1, GL_FALSE, &view[0][0]);

    glUniform3fv(glGetUniformLocation(_pidSphere, "viewPos"), 1, &viewPos[0]);

    glUniform3fv(glGetUniformLocation(_pidSphere, "light.position"), 1, &viewPos[0]);

    //Projection
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width_f / (float)height_f, 0.1f, 5000.0f);
    glUniformMatrix4fv(glGetUniformLocation(_pidSphere, "projection"), 1, GL_FALSE, &projection[0][0]);

    //Uniform
    glUniform4f(glGetUniformLocation(_pidSphere, "couleur"), cyan[0], cyan[1], cyan[2], 1);
    glUniform1f(glGetUniformLocation(_pidSphere, "transpa"), 1.f);

    //Model
    gl4duBindMatrix("modelViewMatrix");
    gl4duLoadIdentityf();

    //float scaleCoef(abs(cos(timeFromStart/5.f))*3.0f);
    //float scaleCoef(timeFromStart*2 - 1.0f);
    //float scaleCoef(abs(tan(timeFromStart/4.0f))/2.0f - 0.1f);

    static std::vector<glm::vec3> allVelocity;
    static std::vector<glm::vec3> allPos;

    static int endScale = 0;
    static float endTime = 0;

    float actualTime(timeFromStart - endTime);

    float scaleCoef(abs(cos(timeFromStart/5.0f)));

    //std::cout << "Scale coef : " << scaleCoef << std::endl;

    if(scaleCoef < 0.01f && !endScale){
    	endScale = true;
    	endTime = timeFromStart;
    }

    if(endScale == 1){
    	scaleCoef = 0.0f;
    	if(timeFromStart - endTime > 1.0f){
    		endScale = 2;
    		endTime = timeFromStart;

    		for(size_t i(0), taille(allFormes.size()*6); i < taille; ++i){

    			float vX((rand()%200 - 100) / 100.0f), vY((rand()%300 - 150) / 100.0f), vZ((rand()%200 - 100) / 100.0f);

    			allVelocity.emplace_back(vX, vY, vZ);

    		}

    	}
    }
	else if(endScale == 2){
		scaleCoef = actualTime*10;
	}


    gl4duRotatef(angle, 1, -1, 0);
    gl4duScalef(scaleCoef, scaleCoef, scaleCoef);

    gl4duSendMatrices();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureSpace);

    if(endScale == 0){
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	    gl4dgDraw(sphere_);
	}


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for(size_t i(0), taille(allFormes.size()); i < taille*6; ++i){

    	size_t j(i%taille);

    	gl4duLoadIdentityf();

    	if(!endScale){
    		gl4duScalef(scaleCoef, scaleCoef, scaleCoef);
    	}
    	else if(endScale == 1){
    		gl4duScalef(0, 0, 0);
    	}
    	else if(endScale == 2){
    		if(scaleCoef < 1){
    			gl4duScalef(scaleCoef, scaleCoef, scaleCoef);
    		}

    		glm::vec3& velocity(allVelocity[i]);

    		gl4duTranslatef(velocity.x*5 * actualTime, velocity.y*2 * actualTime, velocity.z*5 * actualTime);

    	}



    	gl4duSendMatrices();

    	glBindVertexArray(allFormes[j]);
    	glDrawArrays(GL_TRIANGLES, 0, 36);

    }

    if(timeFromStart >= 18.5f) intro = false;


    if(intro){
    	transpa2 -= elapsedTime * 0.5f;
    	if(transpa2 < 0.0f) transpa2 = 0.0f;
    }
    else{
    	transpa2 += elapsedTime * 1.0f;
    	if(transpa2 > 1.0f) transpa2 = 1.0f;
    }




    //On dissine un ecran noir pour les transitions
    glUseProgram(_pidBlack);

    glBindVertexArray(spaceVAO);

    glUniform4f(glGetUniformLocation(_pidBlack, "color"), 0, 0, 0, transpa2);

    glDrawArrays(GL_TRIANGLES, 0, 6);


        //Une fois tout déssiné, on produit le resultat final
    glDisable(GL_DEPTH_TEST);

    //On utilise finalement le flou de gl4d, parfait pour notre utilisation
    //gl4dfBlur(texColBuff, texColBuff2, 15, 1, 0, GL_FALSE);

    //On bind une fois la forme, ca suffi.
    glBindVertexArray(spaceVAO);

	//	----- Ecran -----

	//Et on déssine sur l'ecran
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	//Shaders pour le quad
	glUseProgram(_godLight2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texColBuff);

	glUniform1i(glGetUniformLocation(_godLight2, "screenTexture"), 0);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glEnable(GL_DEPTH_TEST);


    if(timeFromStart >= 20.0f){

        generateStars(posStar, 500, -50, 50,        0, -100, -50, 50);
        generateStars(posStarTwo, 500, -50, 50,  -100, -200, -50, 50);
        generateStars(posStarTree, 500, -50, 50, -200, -300, -50, 50);

        startAnim = std::chrono::system_clock::now();
        gl4duwDisplayFunc(dessinAll);
    }

}

//Static de dessin3D
static float camePos = 0.f;


std::chrono::time_point<std::chrono::system_clock> lastClear;

static int modePlus = 0.0f;

/**
*   Fonction qui dessine la forme precedemment déssiné dans dessin mais en 3D,
*   avec un fond texturé en 3D et des effets sur la forme en fonction des fréquence de la musique.
*   Si un temps total d'animation est atteint, passe a dessinTransition, sinon revien à dessin et genere une nouvelle forme 2D.   
**/
static void dessin3D(void){

    //Calculs de temps

    now = std::chrono::system_clock::now();

    std::chrono::duration<float, std::ratio<1,1>> delta = now - last;

    last = now;

    float elapsedTime(delta.count());

    float timeFromStart(std::chrono::duration<float, std::ratio<1,1>>(now - startAnim).count());

    float timeFromStartProg(std::chrono::duration<double, std::ratio<1,1>>(std::chrono::system_clock::now() - startProg).count());

    glUseProgram(_pId);

    glUniform1f(glGetUniformLocation(_pId, "time"), timeFromStartProg);
    glUniform1f(glGetUniformLocation(_pId, "mode"), modePlus%7);

    //Clear color
    Sint16 tempo = _hauteurs[2];

    //Constante en fonction des basses
    if(tempo > 40){
        //Couleur du clear
        clearColor[0] = colors[0];
        clearColor[1] = colors[1];
        clearColor[2] = colors[2];

        //Couleur de la lumière ambient
        ambient[0] = colors[0];
        ambient[1] = colors[1];
        ambient[2] = colors[2];

        //Transparence de la piece centrale
        transpa = 0.75f;
    }

    float toAdd(elapsedTime * 0.9f);
    for(size_t i(0); i < 3; ++i){
        clearColor[i] += toAdd;
        if(clearColor[i] > 1.0f) clearColor[i] = 1.0f;

        ambient[i] -= toAdd;
        if(ambient[i] < 0.1f) ambient[i] = 0.1f;

    }

    transpa -= elapsedTime * 0.75f;
    if(transpa < 0) transpa = 0.0f;

    //Lumière
    //glUniform3f(glGetUniformLocation(_pId, "light.direction"), -1.f, 0.f, -1.f);

    glUniform3fv(glGetUniformLocation(_pId, "light.ambient"), 1, &ambient[0]);

    glUniform3f(glGetUniformLocation(_pId, "light.specular"), 0.5f, 0.5f, 0.5f);
    glUniform3f(glGetUniformLocation(_pId, "light.diffuse"), 1.f, 1.f, 1.f);

    //Attenuation
    glUniform1f(glGetUniformLocation(_pId, "light.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(_pId, "light.linear"), 0.09f);
    glUniform1f(glGetUniformLocation(_pId, "light.quadratic"), 0.032f);

    //Position de la lumiere constante
    glUniform3f(glGetUniformLocation(_pId, "light.position"), 0, 0, 1);

    //Clear
    glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Uutilisation des shaders
    glUseProgram(_pId);


    //CALCUL POUR LES DEPLACEMENT

    //Scale de la forme
    tempo = _hauteurs[2];
    if(tempo > 0 && hight < (0.5*sqrt(tempo)*(0.5*0.5)*16)){
        accele = sqrt(tempo);
    }
    accele -= elapsedTime * 30.f;

    hight = 0.5*accele*(0.5*0.5)+hight;
    if(hight <= 0.5) hight = 0;
    if(hight == 0) accele = 0;




    //Rotation de la forme
    tempo = _hauteurs[32];
    if(tempo > 0 && accele2 < (0.5*tempo/3*(0.5*0.5)*16)){
        accele2 = tempo/3;
    }
    accele2 -= elapsedTime * 0.75f;

    if(accele2 <= 0) accele2 = 0;

    angle += 0.5*accele2*(0.5*0.5);



    //FIN CALCULS


    //Camera
    camePos += elapsedTime * 30.f;

    float radius = 10.0f;
    float camX = sin((angle + camePos)/50.f) * radius;
    float camZ = cos((angle + camePos)/50.f) * radius;
    glm::mat4 view(1.0f);

    glm::vec3 viewPos(camX, 0.0, camZ);

    view = glm::lookAt(viewPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    glUniformMatrix4fv(glGetUniformLocation(_pId, "view"), 1, GL_FALSE, &view[0][0]);
    
    glUniform3fv(glGetUniformLocation(_pId, "viewPos"), 1, &viewPos[0]);

    //SKY
    glDepthMask(GL_FALSE);
    glUseProgram(_pidSky);

    glm::mat4 projection = glm::perspective(glm::radians(hight*2.5f + 45.f), (float)width_f / (float)height_f, 0.1f, 50.0f);
    glUniformMatrix4fv(glGetUniformLocation(_pidSky, "projection"), 1, GL_FALSE, &projection[0][0]);

    glm::mat4 viewNoTranslate = glm::mat4(glm::mat3(view));
    glUniformMatrix4fv(glGetUniformLocation(_pidSky, "view"), 1, GL_FALSE, &viewNoTranslate[0][0]);

    glBindVertexArray(skyVAO);

    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);

    //FIN SKY


    glUseProgram(_pId);

    //On active le VAO de notre forme
    glBindVertexArray(VAO);

    //Matrice de model
    gl4duBindMatrix("modelViewMatrix");
    gl4duLoadIdentityf();

    gl4duRotatef(-angle, 1, 1, 0);

    gl4duPushMatrix();
    gl4duScalef(hight/15.f + 1.f, hight/15.f + 1.f, hight/15.f + 1.f);
    gl4duSendMatrices();
    gl4duPopMatrix();

    //gl4duRotatef(180 * sin(noStop/100.f), 0, 1, 0);

    glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, cyan);
    glUniform1f(glGetUniformLocation(_pId, "transpa"), 1.f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glUniform1f(glGetUniformLocation(_pId, "transpa"), transpa);

    //A changer ?
    //gl4duScalef(1.0f/(hight/30.f + 0.25f), 1.0f/(hight/30.f + 0.25f), 1.0f/(hight/30.f + 0.25f));
    gl4duScalef((sqrt(hight/30.0f) + 1.0f), (sqrt(hight/30.0f) + 1.0f), (sqrt(hight/30.0f) + 1.0f));
    gl4duSendMatrices();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    if(timeFromStart >= 3.f){

        transpa = 0.f;

        newPoint(p1, p2, p3, p4);

        pourcentActual = 0;
        pourcent = std::array<float, 4>();
        pourcentAuto = std::array<bool, 4>();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        startAnim = std::chrono::system_clock::now();
        gl4duwDisplayFunc(dessin);

        if(timeFromStartProg >= 82.f){

        	clearColor[0] = 0.0f;
        	clearColor[1] = 0.0f;
        	clearColor[2] = 0.0f;

            startAnim = std::chrono::system_clock::now();
            gl4duwDisplayFunc(dessinTransition);
        }

    }


}

float lastStrongest = 1.0f;
/** 
*   Boucle graphique qui dessine une forme 2D de facon procedurale
*   et passe à la fonction dessin3D une fois le dessin complet
**/
static void dessin(void) {

    //Calculs de temps

    now = std::chrono::system_clock::now();

    std::chrono::duration<float, std::ratio<1,1>> delta = now - last;

    last = now;

    float elapsedTime(delta.count());

    //Calcul
    Sint16 tempo = _hauteurs[2];
    if(tempo > 0 && hight < (0.5*sqrt(tempo)*(0.5*0.5)*16)){
        accele = sqrt(tempo);
    }
    accele -= 0.5;

    hight = 0.5*accele*(0.5*0.5)+hight;
    if(hight <= 0.5) hight = 0;
    if(hight == 0) accele = 0;


    for(size_t i(0); i < 3; ++i){
        clearColor[i] -= 0.05f;
        if(clearColor[i] < 0) clearColor[i] = 0.0f;
    }

    //Clear
    glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl4dpClearScreenWith(RGB(255*clearColor[0], 255*clearColor[1], 255*clearColor[2]));

    tempo = _hauteurs[2];

    lastStrongest = hight/1000.0f + elapsedTime * 1.8f;
    if(tempo > 40){
        if(pourcent[pourcentActual] == 0.f){
            pourcentAuto[pourcentActual] = true;
        }
    }

    SlowCube myCube(p1, p2, p3, p4);

    gl4dpSetColor(RGB(50, 255, 255));

    growPourcent(lastStrongest);

    for(size_t i(0); i < 4; ++i){
        std::vector<Point> const& tempo(myCube.getSide(i));
        for(size_t j(0), maxSize(tempo.size()*pourcent[i]); j < maxSize; ++j){
            gl4dpPutPixel(tempo[j].x, tempo[j].y);
        }
    }

    if(pourcent[3] == 1.f){

        //On genere le cube en fonction des nouveaux points...
        generateCube(p1, p2, p3, p4, maForme);

        //On genere Array et buffer
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        //Et on bind le buffer sur ce nouvel array
        bindNewBuffer(maForme, VAO, VBO);

        allFormes.push_back(VAO);
        //On stoque le VBO seulement pour pouvoir le supprimer plus tard
        allFormesVBO.push_back(VBO);


        //On remet tout à 0
        accele = 0;
        accele2 = 0;
        hight = 0;
        angle = 0;

        //On remet à 0 le timer de debut d'animation a chaque changement d'animation
        startAnim = std::chrono::system_clock::now();

        //On remet les static de dessin3D a 0
        camePos = 0.f;

        mode = rand()%7;

        ++modePlus;

        gl4duwDisplayFunc(dessin3D);
    }

    gl4dpUpdateScreen(NULL);
}

///Fonction d'initialisation
static void init(){
    //Partie musicale
    _in4fftw   = (fftw_complex*) fftw_malloc(ECHANTILLONS *  sizeof *_in4fftw);
    memset(_in4fftw, 0, ECHANTILLONS *  sizeof *_in4fftw);
    assert(_in4fftw);
    _out4fftw  = (fftw_complex*) fftw_malloc(ECHANTILLONS * sizeof *_out4fftw);
    assert(_out4fftw);
    _plan4fftw = fftw_plan_dft_1d(ECHANTILLONS, _in4fftw, _out4fftw, FFTW_FORWARD, FFTW_ESTIMATE);
    assert(_plan4fftw);

    //Partie GL
    glEnable(GL_DEPTH_TEST);

    //Transparence
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.f, 0.f, 0.f, 0.0f);

    //Charge texture cube
    cubemapTexture = loadCubemap(faces);

    //Charge texture space
    textureSpace = loadTexture(fileSpace);


    //On genere les frameBuffer
    glGenFramebuffers(1, &fbo1);
    glGenFramebuffers(1, &fbo2);


    //Pour le premier frame buffer
    //On bind
    glBindFramebuffer(GL_FRAMEBUFFER, fbo1);

    //On genere la texture
    texColBuff = genTexture();

    //On bind le (texture) tempon de couleur sur le framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColBuff, 0);

    rbo = genDepthStencil();

    //On bind le render buffer pour le depth et stencil
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    //On verifie si c'est good
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "Erreur framebuffer 1 incomplet" << std::endl;



	//Pour le deuxieme frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fbo2);

	texColBuff2 = genTexture();

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColBuff2, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "Erreur framebuffer 2 incomplet" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);




    //Sky
    //On genere Array et buffer
    glGenVertexArrays(1, &skyVAO);
    glGenBuffers(1, &skyVBO);

    glBindVertexArray(skyVAO);

    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    //attribue de position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    //FIN SKY

    //VAO ET VBO SPACE

    //On genere Array et buffer
    glGenVertexArrays(1, &spaceVAO);
    glGenBuffers(1, &spaceVBO);

    glBindVertexArray(spaceVAO);

    glBindBuffer(GL_ARRAY_BUFFER, spaceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(spaceVertices), spaceVertices, GL_STATIC_DRAW);

    //Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Vecteur Texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    //Fin Space




    //Charge les shader
    _pId  = gl4duCreateProgram("<vs>shaders/vertex.vs", "<fs>shaders/fragment.fs", NULL);
    _pidSky  = gl4duCreateProgram("<vs>shaders/skybox.vs", "<fs>shaders/skybox.fs", NULL);
    _pidSpace  = gl4duCreateProgram("<vs>shaders/space.vs", "<fs>shaders/space.fs", NULL);
    _pidSpaceMonte  = gl4duCreateProgram("<vs>shaders/spaceMonte.vs", "<fs>shaders/spaceMonte.fs", NULL);
    _pidStars  = gl4duCreateProgram("<vs>shaders/stars.vs", "<fs>shaders/stars.fs", NULL);
    _blurV  = gl4duCreateProgram("<vs>shaders/prod/after.vs", "<fs>shaders/prod/blursVert.fs", NULL);
    _blurH  = gl4duCreateProgram("<vs>shaders/prod/after.vs", "<fs>shaders/prod/blursHori.fs", NULL);
    _postProd  = gl4duCreateProgram("<vs>shaders/prod/after.vs", "<fs>shaders/prod/after.fs", NULL);
    _particuleShad  = gl4duCreateProgram("<vs>shaders/particule/particle.vs", "<fs>shaders/particule/particle.fs", NULL);
    _creditShad = gl4duCreateProgram("<vs>shaders/credit/credits.vs", "<fs>shaders/credit/credits.fs", NULL);
    _voronoiShad = gl4duCreateProgram("<vs>shaders/voronoi.vs", "<fs>shaders/voronoi.fs", NULL);
    _pidSphere = gl4duCreateProgram("<vs>shaders/spaceSphere.vs", "<fs>shaders/spaceSphere.fs", NULL);
    _pidBlack = gl4duCreateProgram("<vs>shaders/transi/black.vs", "<fs>shaders/transi/black.fs", NULL);
    _godLight = gl4duCreateProgram("<vs>shaders/prod/god.vs", "<fs>shaders/prod/god.fs", NULL);
    _godLight2 = gl4duCreateProgram("<vs>shaders/prod/god2.vs", "<fs>shaders/prod/god2.fs", NULL);



    mesParticules.init("texture/atlas.png", _particuleShad, 10000);

    //Mobiles
    textureMobiles = gen1DTexture();
    mesMobiles.init(30, width_f, height_f);

    //Generation matrice gl4d
    gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");

    //Objet GL4D
    sphere_ = gl4dgGenSpheref(30, 30);

    glViewport(0, 0, width_f, height_f);


    glUseProgram(_pId);
    glm::mat4 projection = glm::ortho(-1.0f, 1.f, -1.0f, 1.f, -50.f, 50.f);
    glUniformMatrix4fv(glGetUniformLocation(_pId, "projection"), 1, GL_FALSE, &projection[0][0]);

    glUseProgram(_pidSky);
    glUniformMatrix4fv(glGetUniformLocation(_pidSky, "projection"), 1, GL_FALSE, &projection[0][0]);


    //InitiAudio
    initAudio("audio/Arsonist - Discovery — No Copyright Music.ogg");

    //Init textes
    initText(&_textTexId,
        "Développé par\n"
        "Guillaume Magniadas\n\n\n"
        "Musique :\n"
        "Arsonist - Discovery (No Copyright)\n\n\n"
        "Merci d'avoir regardé !");



    //On démarre le chrono de départ
    startProg = std::chrono::system_clock::now();

    last = std::chrono::system_clock::now();

    lastClear = std::chrono::system_clock::now();

}

///Fonction d'initialisation de l'audio reprises des samples GL4D
static void initAudio(const char * filename) {
    //Car apple = chiant
    #if defined(__APPLE__)
        int mult = 1;
    #else
        int mult = 2;
    #endif

    int mixFlags = MIX_INIT_MP3, res;
    res = Mix_Init(mixFlags);

    if( (res & mixFlags) != mixFlags ) {
        fprintf(stderr, "Mix_Init: Erreur lors de l'initialisation de la bibliotheque SDL_Mixer\n");
        fprintf(stderr, "Mix_Init: %s\n", Mix_GetError());
        //exit(3); commenté car ne réagit correctement sur toutes les architectures
    }

    if(Mix_OpenAudio(44100, AUDIO_S16LSB, 1, mult * ECHANTILLONS) < 0) exit(4);

    if(!(_mmusic = Mix_LoadMUS(filename))) {
        fprintf(stderr, "Erreur lors du Mix_LoadMUS: %s\n", Mix_GetError());
        exit(5);
    }

    Mix_SetPostMix(mixCallback, NULL);

    if(!Mix_PlayingMusic()) Mix_PlayMusic(_mmusic, 1);
}

///Fonction de traitement de l'audio reprise des samples GL4D
static void mixCallback(void *udata, Uint8 *stream, int len) {

    if(_plan4fftw) {

        int i, j, l = MIN(len >> 1, ECHANTILLONS);
        Sint16 *d = (Sint16 *)stream;

        for(i = 0; i < l; i++)
            _in4fftw[i][0] = d[i] / ((1 << 15) - 1.0);

        fftw_execute(_plan4fftw);

        for(i = 0; i < l >> 2; i++) {
            _hauteurs[4 * i] = (int)(sqrt(_out4fftw[i][0] * _out4fftw[i][0] + _out4fftw[i][1] * _out4fftw[i][1]) * exp(2.0 * i / (double)(l / 4.0)));
            
            for(j = 1; j < 4; j++)
                _hauteurs[4 * i + j] = MIN(_hauteurs[4 * i], 255);

        }
    }

}


//Fonction generation de texte via fonts reprise des samples GL4D
static void initText(GLuint * ptId, const char * text){

    static int firstTime = 1;

    SDL_Color c = {255, 255, 0, 255};
    SDL_Surface * d, * s;
    TTF_Font * font = NULL;

    if(firstTime) {
        /* initialisation de la bibliothèque SDL2 ttf */
        if(TTF_Init() == -1) {
            fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
            exit(2);
        }
        firstTime = 0;
    }

    if(*ptId == 0) {

        /* initialisation de la texture côté OpenGL */
        glGenTextures(1, ptId);
        glBindTexture(GL_TEXTURE_2D, *ptId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    }

    /* chargement de la font */
    if( !(font = TTF_OpenFont("font/Obesum-Caps-FFP.ttf", 128)) ) {
        fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    /* création d'une surface SDL avec le texte */
    d = TTF_RenderUTF8_Blended_Wrapped(font, text, c, 2048);
    if(d == NULL) {
        TTF_CloseFont(font);
        fprintf(stderr, "Erreur lors du TTF_RenderText\n");
        return;
    }

    /* copie de la surface SDL vers une seconde aux spécifications qui correspondent au format OpenGL */
    s = SDL_CreateRGBSurface(0, d->w, d->h, 32, R_MASK, G_MASK, B_MASK, A_MASK);
    assert(s);
    SDL_BlitSurface(d, NULL, s, NULL);
    SDL_FreeSurface(d);
    /* transfert vers la texture OpenGL */
    glBindTexture(GL_TEXTURE_2D, *ptId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, s->pixels);
    fprintf(stderr, "Dimensions de la texture : %d %d\n", s->w, s->h);
    SDL_FreeSurface(s);
    TTF_CloseFont(font);
    glBindTexture(GL_TEXTURE_2D, 0);

}


int main(int argc, char ** argv) {

	int seed(time(NULL));
	std::cout << "Seed : " << seed << std::endl;
    srand(seed);
    //Des tailles qui vont bien
    //1024 768
    //1200 924
    //1280 1024
    //1280 800
    //1366 768
    if(!gl4duwCreateWindow(0, NULL, /* args du programme */
        "Demo - Guillaume Magniadas", /* titre */
        0, 0, 1366, 768, /* x,y, largeur, hauteur */
        GL4DW_FULLSCREEN | GL4DW_SHOWN) /* état visible */) {
        return 1;
    }

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    width_f = vp[2];
    height_f = vp[3];

    //std::cout << "WIDTH : " << width_f << " HEIGHT : " << height_f << std::endl;

    //Limiter a 60fps
    SDL_GL_SetSwapInterval(1);

    gl4dpInitScreenWithDimensions(width_f, height_f);

    init();

    atexit(quitte);

    newPoint(p1, p2, p3, p4);

    generateCube(p1, p2, p3, p4, maForme);
    bindNewBuffer(maForme, VAO, VBO);

    startAnim = std::chrono::system_clock::now();
    

    generateStars(posStar, 500, -50, 50, -50, 50, -200, -100);
    generateStars(posStarTwo, 500, -50, 50, -50, 50, -100, 0);
    generateStars(posStarTree, 500, -50, 50, -50, 50, 0, 100);

    /*
    Pour commencer avec une autre animation il suffi de commenter
    celle avec dessin et decommenter celle que vous voulez
    */

    gl4duwDisplayFunc(dessinAllStars);
    //gl4duwDisplayFunc(dessinTransition);
    //gl4duwDisplayFunc(dessinVoronoi);
    //gl4duwDisplayFunc(dessin);

    gl4duwMainLoop();

    /*
    Pour lancer dessinAll ou dessinTransition, decommentez ces lignes en commentant celles au dessus
    */

    /*generateStars(posStar, 500, -50, 50,        0, -100, -50, 50);
    generateStars(posStarTwo, 500, -50, 50,  -100, -200, -50, 50);
    generateStars(posStarTree, 500, -50, 50, -200, -300, -50, 50);
    
    for(int i = 0; i < 15; ++i){
        newPoint(p1, p2, p3, p4);
        //On genere le cube en fonction des nouveaux points...
        generateCube(p1, p2, p3, p4, maForme);

        //On genere Array et buffer
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        //Et on bind le buffer sur ce nouvel array
        bindNewBuffer(maForme, VAO, VBO);

        allFormes.push_back(VAO);
    }
    
    //Choisir une des deux

    gl4duwDisplayFunc(dessinTransition);
    //gl4duwDisplayFunc(dessinAll);

    gl4duwMainLoop();*/


    return 0;


}
