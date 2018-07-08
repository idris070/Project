#pragma once
#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"

class Enemy: public Entity {
public:
	Enemy(Unit & unit, float X, float Y, float Speed);
	void update(float time);
private:
	float SpeedX, SpeedY;
};
#endif /* ENEMY_H */