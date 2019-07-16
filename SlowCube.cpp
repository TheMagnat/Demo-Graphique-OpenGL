#include "SlowCube.hpp"
#include <algorithm>

SlowCube::SlowCube(Point const& a, Point const& b, Point const& c, Point const& d){
	plotLine(a, b, 0);
	plotLine(b, c, 1);
	plotLine(c, d, 2);
	plotLine(d, a, 3);
}

SlowCube::~SlowCube() {}

void SlowCube::plotLineHigh(Point const& a, Point const& b, int side){

    int dx = b.x - a.x;
    int dy = b.y - a.y;
    int xi = 1;
    
    if(dx < 0){
        xi = -1;
        dx = -dx;
    }

    int D = 2*dx - dy;
    int x = a.x;

    for(int y = a.y; y < b.y; ++y){
        forme_[side].emplace_back(x, y, 1, 1, 1);
        if (D > 0){
           x = x + xi;
           D = D - 2*dy;
        }
        D = D + 2*dx;
    }
}

void SlowCube::plotLineLow(Point const& a, Point const& b, int side){

    int dx = b.x - a.x;
    int dy = b.y - a.y;
    int yi = 1;

    if(dy < 0){
        yi = -1;
        dy = -dy;
    }

    int D = 2*dy - dx;
    int y = a.y;

    for(int x = a.x; x < b.x; ++x){
        forme_[side].emplace_back(x, y, 1, 1, 1);
        if (D > 0){
           y = y + yi;
           D = D - 2*dx;
        }
        D = D + 2*dy;
    }
}

void SlowCube::plotLine(Point a, Point b, int side){
    if(abs(b.y - a.y) < abs(b.x - a.x)){
        if(a.x > b.x){
        	plotLineLow(b, a, side);
        	reverse(forme_[side].begin(), forme_[side].end());
        }
        else{
          	plotLineLow(a, b, side);
        }
    }
    else{
        if(a.y > b.y){
        	plotLineHigh(b, a, side);
        	reverse(forme_[side].begin(), forme_[side].end());
        }
        else{
        	plotLineHigh(a, b, side);
        }
    }
}

std::vector<Point> const& SlowCube::getSide(size_t side) const{
		return forme_[side];
}