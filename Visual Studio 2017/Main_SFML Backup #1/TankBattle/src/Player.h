#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include "Entity.h"

extern Vector2f playerPos;
extern float PassedTime;

class Player: public Entity {
public:
	Clock resetTrack;
	int CurrentFrame;
	Vector2i weaponGun;
	BulletTank bullet;

	Player(Tank &tank, WeaponTank &weapon, float X, float Y);
	void rotation(float time, Vector2f pos);
	void update(float time);
};

#endif /* PLAYER_H */