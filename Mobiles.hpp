#pragma once

#include <vector>
#include <array>

/**
*	Classe pour generer des mobiles et en faire une texture 1D openGL.
**/
class Mobiles {
public:

	struct Mobile{

		Mobile(double xx, double yy, double vitesseXX, double vitesseYY, std::array<double, 3> colorr);

		double x, y;
		double vitesseX, vitesseY;

		std::array<double, 3> color;
	};

	Mobiles();
	Mobiles(size_t nb, size_t width, size_t height);

	void init(size_t nb, size_t width, size_t height);

	size_t fillTexture(std::vector<float>& texture);

	void frottements(int i, float kx, float ky);

	void move(float elapsedTime);

private:
	std::vector<Mobile> data_;

	size_t width_;
	size_t height_;
};