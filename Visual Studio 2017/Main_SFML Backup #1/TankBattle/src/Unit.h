#pragma once
#ifndef UNIT_H
#define UNIT_H
#include <SFML/Graphics.hpp>

using namespace sf;

struct Basis
{
	int width;
	int height;
	Texture *texture;
	String name;
	Basis();
};
struct Tank : Basis
{
	int health;
	float speed;
	Tank();
	Tank(String Name);
};
struct BulletTank : Basis
{
	int damage;
	float speed;
	int widthBullet;
	int heightBullet;
	BulletTank();
	BulletTank(String Name);
};

struct WeaponTank : Basis
{
	BulletTank bullet;
	Vector2i GunOrigin;
	WeaponTank();
	WeaponTank(String Name);
};


//extern Tank tank1,tank2,tank3;
//extern WeaponTank weapon1,weapon2,weapon3;
//extern BulletTank bullet1,bullet2,bullet3;
void initUnit();

#endif /* UNIT_H */