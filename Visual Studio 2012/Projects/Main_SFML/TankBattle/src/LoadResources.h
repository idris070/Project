#pragma once
#ifndef LOADRESOURCES_H
#define LOADRESOURCES_H
#include <SFML/Graphics.hpp>

using namespace sf;

extern Texture TextureTank1,TextureTank2,TextureTank3,
	TextureBullet1,TextureBullet2,TextureBullet3,
	TextureWeapon1,TextureWeapon2,TextureWeapon3,
	TexturePlayerTank,TexturePlayerWeapon,
	TextureEmpty;
extern Texture explos[24];

void initTexture();
#endif  /* LOADRESOURCES_H */