#pragma once
#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"

extern int** Ground;
extern list <Vector2i> search(Vector2i s, Vector2i finP);
extern int intersection(Vector2i a1, Vector2i a2, Vector2i b);
extern Vector2f playerPos;

class Enemy: public Entity {
public:
	Clock resetMove,resetTrack;
	int randX, randY,targetX,targetY;
	float randB, moveTimer2, speed2, WayTimer,distControl;
	bool WayMove;
	Vector2i weaponGun;
	list <Vector2i> Act, Act2;
	list <Vector2i>::iterator itAct,itAct2;
	BulletTank bullet;

	Enemy(Tank &tank, WeaponTank &weapon, float X, float Y);
	void TrackWay();
	void rotation(float time, Vector2f pos);
	float degreesPoints(vec2 a, vec2 b);
	void update(float time);
	void reset();
};
#endif /* ENEMY_H */