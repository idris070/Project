#pragma once
#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include "HandyMathLib\hm.h"
#include <vector>
#include <list>
#include "Unit.h"

using namespace sf;
using namespace std;

// Класс Entity //

enum KeyboardMove{ Up,Down,None };
extern Texture explos[24];

class Entity {
public:
	float x, y, speed,moveTimer,trackTimer,size,reload,secondTimer;
	int w,h,health,sizeWidth,sizeHeight,side,num;
	bool life;
	Vector2f posMouse;
	Sprite sprite,sprite2;
	String name, control, name_type, name_type2;
	KeyboardMove move;
	bool Left, Right, LeftClick;

	Entity();
	Entity(Image &image, float X, float Y, int W, int H);
	Entity(Texture &Texture, float X, float Y, int W, int H);
	Entity(Basis &basis, float X, float Y);
	FloatRect getRect();
	vec2 rotate(vec2 point, float angle);
	int checkCollision(Entity* b);
	virtual void update(float time) = 0;
};

extern list<Entity*> entities;
extern list<Entity*> entenemy;
extern list<Entity*>::iterator it;
extern list<Entity*>::iterator it2;

// Класс Entity //

#endif /* ENTITY_H */