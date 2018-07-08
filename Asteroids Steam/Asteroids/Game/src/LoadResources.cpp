#include "LoadResources.h"

EntitySound SoundList;

sf::Texture TextureTank1,TextureTank2,TextureTank3,
	TextureBullet1,TextureBullet2,TextureBullet3,
	TextureWeapon1,TextureWeapon2,TextureWeapon3,
	TexturePlayerTank,TexturePlayerWeapon,
	TextureEmpty;
sf::Texture explos[24];

SkinnedMesh BackgroundMap1;
SkinnedMesh BackgroundMap2;
SkinnedMesh BackgroundMap3;
SkinnedMesh BackgroundMap4;
SkinnedMesh MeshPlayer;

SkinnedMesh Asteroid1_1;
SkinnedMesh Asteroid1_2;
SkinnedMesh Asteroid1_3;
SkinnedMesh Asteroid2_1;
SkinnedMesh Asteroid2_2;
SkinnedMesh Asteroid2_3;
SkinnedMesh Asteroid3_1;
SkinnedMesh Asteroid3_2;
SkinnedMesh Asteroid3_3;
SkinnedMesh Fuel;
SkinnedMesh Box1;
SkinnedMesh Box2;

bool initMesh() {
	if (!MeshPlayer.LoadMesh("Model/Player.fbx")) {
		printf("Mesh load failed\n");
		return false;
	}
	if (!BackgroundMap1.LoadMesh("Model/Map1.fbx")) {
		printf("Mesh load failed\n");
		return false;
	}
	if (!BackgroundMap2.LoadMesh("Model/Map2.fbx")) {
		printf("Mesh load failed\n");
		return false;
	}
	if (!BackgroundMap3.LoadMesh("Model/Map3.fbx")) {
		printf("Mesh load failed\n");
		return false;
	}
	if (!BackgroundMap4.LoadMesh("Model/Map4.fbx")) {
		printf("Mesh load failed\n");
		return false;
	}
	Asteroid1_1.LoadMesh("Model/Asteroid_1-1.fbx");
	Asteroid1_2.LoadMesh("Model/Asteroid_1-2.fbx");
	Asteroid1_3.LoadMesh("Model/Asteroid_1-3.fbx");
	Asteroid2_1.LoadMesh("Model/Asteroid_2-1.fbx");
	Asteroid2_2.LoadMesh("Model/Asteroid_2-2.fbx");
	Asteroid2_3.LoadMesh("Model/Asteroid_2-3.fbx");
	Asteroid3_1.LoadMesh("Model/Asteroid_3-1.fbx");
	Asteroid3_2.LoadMesh("Model/Asteroid_3-2.fbx");
	Asteroid3_3.LoadMesh("Model/Asteroid_3-3.fbx");
	Fuel.LoadMesh("Model/Fuel.fbx");
	Box1.LoadMesh("Model/Box1.fbx");
	Box2.LoadMesh("Model/Box2.fbx");

	explos[0].loadFromFile("images/explosion/expl_01_0000.png");
	explos[1].loadFromFile("images/explosion/expl_01_0001.png");
	explos[2].loadFromFile("images/explosion/expl_01_0002.png");
	explos[3].loadFromFile("images/explosion/expl_01_0003.png");
	explos[4].loadFromFile("images/explosion/expl_01_0004.png");
	explos[5].loadFromFile("images/explosion/expl_01_0005.png");
	explos[6].loadFromFile("images/explosion/expl_01_0006.png");
	explos[7].loadFromFile("images/explosion/expl_01_0007.png");
	explos[8].loadFromFile("images/explosion/expl_01_0008.png");
	explos[9].loadFromFile("images/explosion/expl_01_0009.png");
	explos[10].loadFromFile("images/explosion/expl_01_0010.png");
	explos[11].loadFromFile("images/explosion/expl_01_0011.png");
	explos[12].loadFromFile("images/explosion/expl_01_0012.png");
	explos[13].loadFromFile("images/explosion/expl_01_0013.png");
	explos[14].loadFromFile("images/explosion/expl_01_0014.png");
	explos[15].loadFromFile("images/explosion/expl_01_0015.png");
	explos[16].loadFromFile("images/explosion/expl_01_0016.png");
	explos[17].loadFromFile("images/explosion/expl_01_0017.png");
	explos[18].loadFromFile("images/explosion/expl_01_0018.png");
	explos[19].loadFromFile("images/explosion/expl_01_0019.png");
	explos[20].loadFromFile("images/explosion/expl_01_0020.png");
	explos[21].loadFromFile("images/explosion/expl_01_0021.png");
	explos[22].loadFromFile("images/explosion/expl_01_0022.png");
	explos[23].loadFromFile("images/explosion/expl_01_0023.png");

	return true;
}

void initTexture()
{
	TextureTank1.loadFromFile("images/tank1_body.png");
	TextureTank2.loadFromFile("images/tank2_body.png");
	TextureTank3.loadFromFile("images/tank3_body.png");

	TextureBullet1.loadFromFile("images/bullet.png");
	TextureBullet2.loadFromFile("images/bullet2.png");
	TextureBullet3.loadFromFile("images/bullet3.png");

	TextureWeapon1.loadFromFile("images/tank1_gun.png");
	TextureWeapon2.loadFromFile("images/tank2_gun.png");
	TextureWeapon3.loadFromFile("images/tank3_gun.png");

	TexturePlayerTank.loadFromFile("images/t1.png");
	TexturePlayerWeapon.loadFromFile("images/t3.png");
}

EntitySound::EntitySound()
{
	alien.openFromFile("Sound/Alien.wav");
	fuelOff.openFromFile("Sound/FuelOff.wav");
	lose.openFromFile("Sound/Lose.wav");
	pickBox.openFromFile("Sound/PickBox.wav");
	pickFuel.openFromFile("Sound/PickFuel.wav");
	select.openFromFile("Sound/Select.wav");
	shipMove.openFromFile("Sound/ShipMove.wav");
	unlock.openFromFile("Sound/Unlock.wav");
	win.openFromFile("Sound/Win.wav");
	musicGame.openFromFile("Sound/Clouds.wav");
	musicMenu.openFromFile("Sound/Music_menu.ogg");
	hover.openFromFile("Sound/buttonHover.flac");
	click.openFromFile("Sound/buttonClick.wav");
	explosion.openFromFile("Sound/explosion.ogg");

	alien.setLoop(true);
	shipMove.setLoop(true);
	musicGame.setLoop(true);
	musicMenu.setLoop(true);

	alien.setVolume(20);
	musicMenu.setVolume(50);
}
