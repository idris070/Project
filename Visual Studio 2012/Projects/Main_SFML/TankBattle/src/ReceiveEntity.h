#pragma once
#ifndef RECEIVEENTITY_H
#define RECEIVEENTITY_H
#include "Entity.h"

class ReceiveEntity {
public:
	float x,y,rotation;
	int health;
	String name, control;
	Sprite sprite,sprite2;
	Texture *texture, *texture2;

	ReceiveEntity(void);
	ReceiveEntity(float X, float Y, String Name, float Rot);
	ReceiveEntity(float X, float Y, String NameType1, String NameType2, float Rot1, float Rot2);
	ReceiveEntity(float X, float Y, int NUM);
	~ReceiveEntity(void);
	void update(float time);
};

extern list<ReceiveEntity*> entities2;
extern list<ReceiveEntity*> entities3;
extern list<ReceiveEntity*>::iterator it3;

#endif /* RECEIVEENTITY_H */