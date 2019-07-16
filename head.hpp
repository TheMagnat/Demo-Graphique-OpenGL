#pragma once

#include <GL4D/gl4du.h>
#include <GL4D/gl4df.h>
#include <GL4D/gl4dp.h>
#include <GL4D/gl4duw_SDL2.h>

#include <vector>
#include <string>
#include <array>
#include <iostream>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

//Son
#include <SDL_mixer.h>
#include <fftw3.h>

//Fonts
#include <SDL_ttf.h>

//Mes includes
#include "Point.hpp"

//Define
#define WIDTH 1200
#define HEIGHT 650


//Variable Importante

//Fichier main
static void growPourcent();

static void dessin3D(void);
static void dessin(void);
static void init();
static void initAudio(const char * filename);
static void mixCallback(void *udata, Uint8 *stream, int len);
static void initText(GLuint * ptId, const char * text);