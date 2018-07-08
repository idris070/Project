#pragma once
#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "Entity.h"
class Explosion //класс пули
{
public:
	sf::String name;
	sf::Sprite sprite;
	bool life;
	int num;
	float rotation, x, y, last_x, last_y;
	sf::Clock Time;
	float timeS;

	Explosion(float X, float Y, float lastX, float lastY, float Rotation);
	void update(float time);
};

extern std::list<Explosion*> SFMLDraw;
extern std::list<Explosion*>::iterator SFMLit;
#endif /* EXPLOSION_H */