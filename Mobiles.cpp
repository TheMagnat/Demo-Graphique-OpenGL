#include "Mobiles.hpp"

#include <algorithm>
#include <cmath>
#include <experimental/array>

Mobiles::Mobile::Mobile(double xx, double yy, double vitesseXX, double vitesseYY, std::array<double, 3> colorr)
: x(xx), y(yy), vitesseX(vitesseXX), vitesseY(vitesseYY), color(colorr) {}

Mobiles::Mobiles() {}

Mobiles::Mobiles(size_t nb, size_t width, size_t height) : width_(width), height_(height) {

	//srand(time(NULL));

	for(size_t i(0); i < nb; ++i) {

		data_.emplace_back(	width_ / 4 + (rand() / (RAND_MAX + 1.0)) * width_ / 2,			//X
							height_ / 4 + (rand() / (RAND_MAX + 1.0)) * height_ / 2,		//Y
							100 + (2.0 * (rand() / (RAND_MAX + 1.0)) - 1.0) * 500,			//Vitesse en X
							100 + (rand() / (RAND_MAX + 1.0)) * 200,						//Vitesse en Y
							std::experimental::make_array((rand() / (RAND_MAX + 1.0)), (rand() / (RAND_MAX + 1.0)), (rand() / (RAND_MAX + 1.0)))
							); //Couleurs

	}

}

void Mobiles::init(size_t nb, size_t width, size_t height){

	//Initialisation
	width_ = width;
	height_ = height;

	for(size_t i(0); i < nb; ++i) {

		data_.emplace_back(	(rand() / (RAND_MAX + 1.0)) * width_,			//X
							(rand() / (RAND_MAX + 1.0)) * height_,		//Y
							100 + (2.0 * (rand() / (RAND_MAX + 1.0)) - 1.0) * 500,			//Vitesse en X
							100 + (rand() / (RAND_MAX + 1.0)) * 200,						//Vitesse en Y
							std::experimental::make_array((rand() / (RAND_MAX + 1.0)), (rand() / (RAND_MAX + 1.0)), (rand() / (RAND_MAX + 1.0)))
							); //Couleurs

	}

}

///Fonction reprise des samples GL4D
size_t Mobiles::fillTexture(std::vector<float>& texture){

	size_t taille(data_.size());
	for(size_t i(0); i < taille; i++) {

		texture.push_back(data_[i].color[0]);
		texture.push_back(data_[i].color[1]);
		texture.push_back(data_[i].color[2]);
		texture.push_back(1.0f);
		texture.push_back(data_[i].x / width_);
		texture.push_back(data_[i].y / height_);
		texture.push_back(0.0f);
		texture.push_back(0.0f);

	}

	return taille*2;

}

///Fonction reprise des samples GL4D
void Mobiles::frottements(int i, float kx, float ky) {

	float intensite = 0.01f;

	double x = fabs(data_[i].vitesseX), y = fabs(data_[i].vitesseY);
	data_[i].vitesseX = std::max(x - kx * x * x * intensite * intensite - 2.0 * kx, 0.0) * ((data_[i].vitesseX) < 0 ? -1 : 1);
	data_[i].vitesseY = std::max(y - ky * y * y * intensite * intensite - 2.0 * ky, 0.0) * ((data_[i].vitesseY) < 0 ? -1 : 1);
}

///Fonction reprise des samples GL4D
void Mobiles::move(float elapsedTime) {
	const double G = -300;

	double d;
	int collision_x, collision_bas;

	for(size_t i(0); i < data_.size(); ++i) {

	collision_x = collision_bas = 0;
	data_[i].x += data_[i].vitesseX * elapsedTime;
	data_[i].y += data_[i].vitesseY * elapsedTime;

	if( (d = data_[i].x) <= 0 ) {

	  if(data_[i].vitesseX < 0) data_[i].vitesseX = -data_[i].vitesseX;

	  data_[i].vitesseX -= d;
	  frottements(i, 1.0, 0.2);
	  collision_x = 1;

	}
	if( (d = data_[i].x - (width_ - 1)) >= 0 ) {

	  if(data_[i].vitesseX > 0) data_[i].vitesseX = -data_[i].vitesseX;

	  data_[i].vitesseX -= d;
	  frottements(i, 1.0, 0.2);
	  collision_x = 1;

	}
	if( (d = data_[i].y) <= 0 ) {

	  if(data_[i].vitesseY < 0) data_[i].vitesseY = -data_[i].vitesseY;
	  
	  data_[i].vitesseY -= d;
	  frottements(i, 0.2, 1.0);
	  collision_bas = 1;

	}
	if(!collision_bas)
		data_[i].vitesseY += G * elapsedTime;
	}
}
