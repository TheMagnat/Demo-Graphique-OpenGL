#pragma once

#include "head.hpp"

#include <vector>
#include <array>

#include "stb_image.h"

void generateCube(Point const& a1, Point const& a2, Point const& a3, Point const& a4, std::array<float, 216>& maForme);
void bindNewBuffer(std::array<float, 216> const& maForme, unsigned int VAO, unsigned int VBO);
unsigned int loadCubemap(std::vector<std::string> const& faces);
unsigned int loadTexture(std::string const& file);
unsigned int loadTextureA(std::string const& file);
unsigned int genTexture();
unsigned int gen1DTexture();
unsigned int genDepthStencil();