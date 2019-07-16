#pragma once

#include <vector>
#include <array>
#include "Point.hpp"
/**
*	Classe qui génère tout les points des 4 segments d'un quadrilatère en coordonnée écran
**/
class SlowCube {
public:

	SlowCube(Point const& a, Point const& b, Point const& c, Point const& d);
	~SlowCube();


	void plotLineHigh(Point const& a, Point const& b, int side);

	void plotLineLow(Point const& a, Point const& b, int side);

	void plotLine(Point a, Point b, int side);

	std::vector<Point> const& getSide(size_t side) const;

private:

	std::array<std::vector<Point>, 4> forme_;

};