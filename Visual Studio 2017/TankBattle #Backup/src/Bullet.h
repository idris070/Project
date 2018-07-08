#pragma once
#ifndef BULLET_H
#define BULLET_H
#include "Entity.h"
#include "Explosion.h"

class Bullet :public Entity//����� ����
{
public:
	int damage;
	float rotation,c,d,timeS;
	Clock Time;

	//Bullet(Image &image, float X, float Y, int W, int H, float Rotation, string Used);//�� ��� ��, ������ ����� � ����� ��������� ������ (int dir)
	Bullet(BulletTank bullet, float X, float Y, float Rotation, string Used);//�� ��� ��, ������ ����� � ����� ��������� ������ (int dir)
	FloatRect getRect();
	void update(float time);
};
#endif /* BULLET_H */