#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include "Entity.h"

extern sf::Vector2f playerPos;
extern float PassedTime;

class Player: public Entity {
public:
	int Score;
	float Fuel, volShip;
	bool soundFuel;
	int countBox, countFuel;
	void rotation(float time, sf::Vector2f pos);
	void update(float time);
	// Новое на 3D
	Player(Unit & unit, float X, float Y);
	~Player();
};

#endif /* PLAYER_H */