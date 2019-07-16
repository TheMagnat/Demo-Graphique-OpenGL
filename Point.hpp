#pragma once
/**
*	Structure très simple pour stoquer des information basique sur un point
**/
struct Point{

    Point(int xx, int yy, float rr, float gg, float bb);

    int x, y;
    float r, g, b;
};