#pragma once
#ifndef WALL_H
#define WALL_H
#include "Entity.h"

class Wall: public Entity {
public:
	Clock resetMove,resetTrack;
	int randX, randY;
	float randB;

	Wall(Image &image, float X, float Y, int W, int H);
	void update(float time);
};
#endif /* WALL_H */