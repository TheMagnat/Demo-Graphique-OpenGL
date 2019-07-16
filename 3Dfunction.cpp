#include "3Dfunction.hpp"
#include <cmath>

//Ce fichier contient des fonction utiles pour les scene 3D et openGL en général

/**
* Calcule la longueur entre 2 points a et b dans un espace 2D.
**/
float longueur(Point const& a, Point const& b){
    float test(sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2)));
    return test;
}

/**
* Calcule la longueur entre 2 points a et b dans un espace 2D.
**/
float longueur(std::pair<float, float> a, std::pair<float, float> b){
    float test(sqrt(pow(b.first - a.first, 2) + pow(b.second - a.second, 2)));
    std::cout << "Test : " << test << std::endl;
    return test;
}

/**
* Calcule la longueur entre 2 points a et b dans un espace 2D.
**/
float longueur(float ax, float ay, float bx, float by){
    float test(sqrt(pow(bx - ax, 2) + pow(by - ay, 2)));
    return test;
}

/**
*   Fonction qui génère les vertices d'une forme 3D se basant sur une forme 2D à 4 coté.
*   Note : Il est conseillé de rentrer les points dans cette ordre ci : angle bas gauche, angle haut gauche, angle haut droite, angle bas droit.
**/
void generateCube(Point const& a1, Point const& a2, Point const& a3, Point const& a4, std::array<float, 216>& maForme){

    //float l1(longueur(a1, a2)), l2(longueur(a2, a3)), l3(longueur(a3, a4)), l4(longueur(a4, a1));
    
    //float profondeur((l1 + l2 + l3 + l4)/4.f);



    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    unsigned int width = vp[2];
    unsigned int height = vp[3];

    //profondeur = abs(((100.0f * (profondeur - (height/2.f)))/(height/2.f))/100.0f);

    //profondeur = 0.25f;

    //First = x, Second = y;
    std::vector<std::pair<float, float>> angles;

    angles.emplace_back(((100.0f * (a1.x - (width/2.f)))/(width/2.f))/100.0f, ((100.0f * (a1.y - (height/2.f)))/(height/2.f))/100.0f);
    angles.emplace_back(((100.0f * (a2.x - (width/2.f)))/(width/2.f))/100.0f, ((100.0f * (a2.y - (height/2.f)))/(height/2.f))/100.0f);
    angles.emplace_back(((100.0f * (a3.x - (width/2.f)))/(width/2.f))/100.0f, ((100.0f * (a3.y - (height/2.f)))/(height/2.f))/100.0f);
    angles.emplace_back(((100.0f * (a4.x - (width/2.f)))/(width/2.f))/100.0f, ((100.0f * (a4.y - (height/2.f)))/(height/2.f))/100.0f);

    size_t actualFace(0);

    //float l1(longueur(angles[0], angles[1])), l2(longueur(angles[1], angles[2])), l3(longueur(angles[2], angles[3])), l4(longueur(angles[3], angles[0]));
    float l2(longueur(angles[1], angles[2])), l4(longueur(angles[3], angles[0]));

    //float profondeur((l1 + l2 + l3 + l4)/4.f);
    float profondeur((l2 + l4)/4.f);

    std::cout << "profondeur obtenue : " << profondeur << std::endl;

    //Face 1 / 6 Puis Face 2 / 6

    float z = -1.f;

    for(size_t j(0); j < 2; ++j){

        //Angles 0, 1, 2;
        for(size_t i(0); i < 3; ++i){

            maForme[actualFace*6 + 0] = angles[i].first;
            maForme[actualFace*6 + 1] = angles[i].second;
            maForme[actualFace*6 + 2] = profondeur * z;
            maForme[actualFace*6 + 3] = 0.0f;
            maForme[actualFace*6 + 4] = 0.0f;
            maForme[actualFace*6 + 5] = z;

            ++actualFace;

        }

            //Angles 2, 3;
        for(size_t i(2); i < 4; ++i){

            maForme[actualFace*6 + 0] = angles[i].first;
            maForme[actualFace*6 + 1] = angles[i].second;
            maForme[actualFace*6 + 2] = profondeur * z;
            maForme[actualFace*6 + 3] = 0.0f;
            maForme[actualFace*6 + 4] = 0.0f;
            maForme[actualFace*6 + 5] = z;

            ++actualFace;

        }

            //Angle 0
        maForme[actualFace*6 + 0] = angles[0].first;
        maForme[actualFace*6 + 1] = angles[0].second;
        maForme[actualFace*6 + 2] = profondeur * z;
        maForme[actualFace*6 + 3] = 0.0f;
        maForme[actualFace*6 + 4] = 0.0f;
        maForme[actualFace*6 + 5] = z;

        ++actualFace;
    
        z *= -1.0f;
    }

    //Face 3 / 6 (Face de gauche)

    //1
    //Angles 0;
    maForme[actualFace*6 + 0] = angles[0].first;
    maForme[actualFace*6 + 1] = angles[0].second;
    maForme[actualFace*6 + 2] = -profondeur;

    maForme[actualFace*6 + 3] = -1.0f;
    maForme[actualFace*6 + 4] = 0.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //2
    //Angle 1;
    maForme[actualFace*6 + 0] = angles[1].first;
    maForme[actualFace*6 + 1] = angles[1].second;
    maForme[actualFace*6 + 2] = -profondeur;

    maForme[actualFace*6 + 3] = -1.0f;
    maForme[actualFace*6 + 4] = 0.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //3
    //Angle 1 (Seul z change ici);
    maForme[actualFace*6 + 0] = angles[1].first;
    maForme[actualFace*6 + 1] = angles[1].second;
    maForme[actualFace*6 + 2] = profondeur;

    maForme[actualFace*6 + 3] = -1.0f;
    maForme[actualFace*6 + 4] = 0.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //4
    //Angle 1;
    maForme[actualFace*6 + 0] = angles[1].first;
    maForme[actualFace*6 + 1] = angles[1].second;
    maForme[actualFace*6 + 2] = profondeur;

    maForme[actualFace*6 + 3] = -1.0f;
    maForme[actualFace*6 + 4] = 0.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //5
    //Angles 0 (z change);
    maForme[actualFace*6 + 0] = angles[0].first;
    maForme[actualFace*6 + 1] = angles[0].second;
    maForme[actualFace*6 + 2] = profondeur;

    maForme[actualFace*6 + 3] = -1.0f;
    maForme[actualFace*6 + 4] = 0.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //6
    //Angles 0;
    maForme[actualFace*6 + 0] = angles[0].first;
    maForme[actualFace*6 + 1] = angles[0].second;
    maForme[actualFace*6 + 2] = -profondeur;

    maForme[actualFace*6 + 3] = -1.0f;
    maForme[actualFace*6 + 4] = 0.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //Face 4 / 6 (Face de droite)

    //1
    //Angles 0;
    maForme[actualFace*6 + 0] = angles[3].first;
    maForme[actualFace*6 + 1] = angles[3].second;
    maForme[actualFace*6 + 2] = -profondeur;

    maForme[actualFace*6 + 3] = 1.0f;
    maForme[actualFace*6 + 4] = 0.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //2
    //Angle 1;
    maForme[actualFace*6 + 0] = angles[2].first;
    maForme[actualFace*6 + 1] = angles[2].second;
    maForme[actualFace*6 + 2] = -profondeur;

    maForme[actualFace*6 + 3] = 1.0f;
    maForme[actualFace*6 + 4] = 0.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //3
    //Angle 1 (Seul z change ici);
    maForme[actualFace*6 + 0] = angles[2].first;
    maForme[actualFace*6 + 1] = angles[2].second;
    maForme[actualFace*6 + 2] = profondeur;

    maForme[actualFace*6 + 3] = 1.0f;
    maForme[actualFace*6 + 4] = 0.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //4
    //Angle 1;
    maForme[actualFace*6 + 0] = angles[2].first;
    maForme[actualFace*6 + 1] = angles[2].second;
    maForme[actualFace*6 + 2] = profondeur;

    maForme[actualFace*6 + 3] = 1.0f;
    maForme[actualFace*6 + 4] = 0.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //5
    //Angles 0 (z change);
    maForme[actualFace*6 + 0] = angles[3].first;
    maForme[actualFace*6 + 1] = angles[3].second;
    maForme[actualFace*6 + 2] = profondeur;

    maForme[actualFace*6 + 3] = 1.0f;
    maForme[actualFace*6 + 4] = 0.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //6
    //Angles 0;
    maForme[actualFace*6 + 0] = angles[3].first;
    maForme[actualFace*6 + 1] = angles[3].second;
    maForme[actualFace*6 + 2] = -profondeur;

    maForme[actualFace*6 + 3] = 1.0f;
    maForme[actualFace*6 + 4] = 0.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;



    //
    //
    //Face 5 / 6 (Dessous)

    //1
    //Angles 0;
    maForme[actualFace*6 + 0] = angles[0].first;
    maForme[actualFace*6 + 1] = angles[0].second;
    maForme[actualFace*6 + 2] = -profondeur;

    maForme[actualFace*6 + 3] = 0.0f;
    maForme[actualFace*6 + 4] = -1.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //2
    //Angle 1;
    maForme[actualFace*6 + 0] = angles[0].first;
    maForme[actualFace*6 + 1] = angles[0].second;
    maForme[actualFace*6 + 2] = profondeur;

    maForme[actualFace*6 + 3] = 0.0f;
    maForme[actualFace*6 + 4] = -1.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //3
    //Angle 1 (Seul z change ici);
    maForme[actualFace*6 + 0] = angles[3].first;
    maForme[actualFace*6 + 1] = angles[3].second;
    maForme[actualFace*6 + 2] = profondeur;

    maForme[actualFace*6 + 3] = 0.0f;
    maForme[actualFace*6 + 4] = -1.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //4
    //Angle 1;
    maForme[actualFace*6 + 0] = angles[3].first;
    maForme[actualFace*6 + 1] = angles[3].second;
    maForme[actualFace*6 + 2] = profondeur;

    maForme[actualFace*6 + 3] = 0.0f;
    maForme[actualFace*6 + 4] = -1.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //5
    //Angles 0 (z change);
    maForme[actualFace*6 + 0] = angles[3].first;
    maForme[actualFace*6 + 1] = angles[3].second;
    maForme[actualFace*6 + 2] = -profondeur;

    maForme[actualFace*6 + 3] = 0.0f;
    maForme[actualFace*6 + 4] = -1.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //6
    //Angles 0;
    maForme[actualFace*6 + 0] = angles[0].first;
    maForme[actualFace*6 + 1] = angles[0].second;
    maForme[actualFace*6 + 2] = -profondeur;

    maForme[actualFace*6 + 3] = 0.0f;
    maForme[actualFace*6 + 4] = -1.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;


    //
    //
    //Face 6 / 6 (Dessus)

    //1
    //Angles 0;
    maForme[actualFace*6 + 0] = angles[1].first;
    maForme[actualFace*6 + 1] = angles[1].second;
    maForme[actualFace*6 + 2] = profondeur;

    maForme[actualFace*6 + 3] = 0.0f;
    maForme[actualFace*6 + 4] = -1.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //2
    //Angle 1;
    maForme[actualFace*6 + 0] = angles[1].first;
    maForme[actualFace*6 + 1] = angles[1].second;
    maForme[actualFace*6 + 2] = -profondeur;

    maForme[actualFace*6 + 3] = 0.0f;
    maForme[actualFace*6 + 4] = -1.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //3
    //Angle 1 (Seul z change ici);
    maForme[actualFace*6 + 0] = angles[2].first;
    maForme[actualFace*6 + 1] = angles[2].second;
    maForme[actualFace*6 + 2] = -profondeur;

    maForme[actualFace*6 + 3] = 0.0f;
    maForme[actualFace*6 + 4] = -1.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //4
    //Angle 1;
    maForme[actualFace*6 + 0] = angles[2].first;
    maForme[actualFace*6 + 1] = angles[2].second;
    maForme[actualFace*6 + 2] = -profondeur;

    maForme[actualFace*6 + 3] = 0.0f;
    maForme[actualFace*6 + 4] = -1.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //5
    //Angles 0 (z change);
    maForme[actualFace*6 + 0] = angles[2].first;
    maForme[actualFace*6 + 1] = angles[2].second;
    maForme[actualFace*6 + 2] = profondeur;

    maForme[actualFace*6 + 3] = 0.0f;
    maForme[actualFace*6 + 4] = -1.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    ++actualFace;

    //6
    //Angles 0;
    maForme[actualFace*6 + 0] = angles[1].first;
    maForme[actualFace*6 + 1] = angles[1].second;
    maForme[actualFace*6 + 2] = profondeur;

    maForme[actualFace*6 + 3] = 0.0f;
    maForme[actualFace*6 + 4] = -1.0f;
    maForme[actualFace*6 + 5] = 0.0f;

    /*for(int test = 0; test < 36; ++test){
        printf("Vertex %d : %f, %f, %f, %f, %f, %f\n", test, maForme[test*6 + 0], maForme[test*6 + 1], maForme[test*6 + 2], maForme[test*6 + 3], maForme[test*6 + 4], maForme[test*6 + 5]);
    }*/

}

/**
*   Fonction pour remplir le VAO du nouvel objet avec les vertices de maForme
**/
void bindNewBuffer(std::array<float, 216> const& maForme, unsigned int VAO, unsigned int VBO){

	glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, maForme.size() * sizeof(float), maForme.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Vecteur normal des sommets
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

}

//Fonction reprise du cours openGL developpez.com légèrement modifié

/**
*   Fonction pour charger une cube map.
*   Retourne l'identifiant openGL de celle-ci.
**/
unsigned int loadCubemap(std::vector<std::string> const& faces){
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (size_t i = 0; i < faces.size(); i++){

        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if(data){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else{
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
        }

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

/**
*   Fonction pour charger une texture 2D simple.
*   Retourne l'identifiant openGL de celle-ci.
*   Note : version sans canal alpha.
**/
unsigned int loadTexture(std::string const& file){
    unsigned int textureID;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture " << file << std::endl;
    }
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

/**
*   Fonction pour charger une texture 2D simple.
*   Retourne l'identifiant openGL de celle-ci.
*   Note : version avec canal alpha.
**/
unsigned int loadTextureA(std::string const& file){
    unsigned int textureID;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture " << file << std::endl;
    }
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

/**
*   Génère et reseve la mémoire pour une texture 2D.
*   Pratique pour les frambuffer.
**/
unsigned int genTexture(){

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    unsigned int width = vp[2];
    unsigned int height = vp[3];

    // génère la texture
    unsigned int texColorBufferId;

    glGenTextures(1, &texColorBufferId);

    glBindTexture(GL_TEXTURE_2D, texColorBufferId);
    
    //Réserve la mémoire
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texColorBufferId;

}

/**
*   Génère et reserve la mémoire pour un texture 1D.
**/
unsigned int gen1DTexture(){

    unsigned int id;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_1D, id);
    
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, 1, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindTexture(GL_TEXTURE_1D, 0);

    return id;
}


/**
*   Génère un buffer de profondeur et de pochoir utilisable par des VBO.
**/
unsigned int genDepthStencil(){

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    unsigned int width = vp[2];
    unsigned int height = vp[3];

    unsigned int rbo;

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    return rbo;
}