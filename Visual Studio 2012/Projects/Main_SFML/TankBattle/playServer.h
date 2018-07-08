#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include "src/level.h"
#include "src/HandyMathLib\hm.h"
#include <vector>
#include <list>
#include "src/Objects.h"

vec2 rotate(vec2 point, float angle){
	vec2 rotated_point;
	rotated_point.x = point.x * cos(angle) - point.y * sin(angle);
	rotated_point.y = point.x * sin(angle) + point.y * cos(angle);
	return rotated_point;
}
Image bricks;
Vector2f SpawnPlayer;
int play()
{
	RenderWindow window(VideoMode(1600, 900), "Tanks");
	initUnit();
	initTexture();
	setArray(&Ground);

	entities.clear();
	entenemy.clear();

	view.reset(FloatRect(0, 0, 1600, 900));

	Level lvl;
	lvl.LoadFromFile("map1.tmx");
	std::vector<Object> obj;
	obj = lvl.GetAllObjects();

	bricks.loadFromFile("images/bricks.png");

	Image TrackImage;
	TrackImage.loadFromFile("images/tank1_track.png");
	struct SpawnEnemy
	{
		int left,top,width,height;
	};
	SpawnEnemy spawn01;

	for (int i = 0; i<obj.size(); i++)
	{
		if(obj[i].name == "solid")
		{
			int left = obj[i].rect.left/32;
			int width = obj[i].rect.width/32+left;
			int top = obj[i].rect.top/32;
			int height = obj[i].rect.height/32+top;

			for (int x = left; x < width; x++)
			for (int y = top; y < height; y++)
				Ground[x][y] = 1;

			entenemy.push_back(new Wall (bricks,
				obj[i].rect.left+obj[i].rect.width/2,
				obj[i].rect.top+obj[i].rect.height/2, 
				obj[i].rect.width, 
				obj[i].rect.height));
		}
		if(obj[i].name == "SpawnPlayer")
		{
			SpawnPlayer.x = obj[i].rect.left+obj[i].rect.width/2;
			SpawnPlayer.y = obj[i].rect.top+obj[i].rect.height/2;
		}
		if(obj[i].name == "SpawnEnemy")
		{
			spawn01.left = obj[i].rect.left;
			spawn01.top = obj[i].rect.top;
			spawn01.width = obj[i].rect.width;
			spawn01.height = obj[i].rect.height;
		}
	}

	entities.push_back(new Player (Tank("tank1"), WeaponTank("weapon1"), SpawnPlayer.x, SpawnPlayer.y));
	it = entities.begin();
	entenemy.push_back(entities.back());
	Entity *p = *it;

	Clock clock;
	Clock checker;
	Clock second;

	float spawnTank = 5;
	float location = 0;
	bool wave1 = true, wave2 = true, wave3 = true;
    while (window.isOpen())
    {
		float time = clock.getElapsedTime().asMicroseconds();
		float trackTimer = checker.getElapsedTime().asMilliseconds();
		PassedTime = second.getElapsedTime().asSeconds();
		p->secondTimer = second.getElapsedTime().asSeconds();
		clock.restart();
		time = time / 800;
		if(time>60)
			time = 60;

		playerPos.x = p->x;
		playerPos.y = p->y;

		view.setCenter(p->x, p->y); 

		if(PassedTime>1 && wave1){
			spawnTank += 500;
			wave1 = false;
			int xPos, yPos;
			for(int i=0; i<10; i++)
			{
					xPos = spawn01.left + rand() % spawn01.width;
					yPos = spawn01.top + rand() % spawn01.height;
					entities.push_back(new Enemy(Tank("tank1"), WeaponTank("weapon1"), xPos, yPos));
					entenemy.push_back(entities.back());
			}
		}
		if(PassedTime>20 && wave2){
			spawnTank += 500;
			wave2 = false;
			int xPos, yPos;
			for(int i=0; i<10; i++)
			{
					xPos = spawn01.left + rand() % spawn01.width;
					yPos = spawn01.top + rand() % spawn01.height;
					entities.push_back(new Enemy(Tank("tank2"), WeaponTank("weapon2"), xPos, yPos));
					entenemy.push_back(entities.back());
			}
		}
		if(PassedTime>35 && wave3){
			spawnTank += 500;
			wave3 = false;
			int xPos, yPos;
			for(int i=0; i<10; i++)
			{
					xPos = spawn01.left + rand() % spawn01.width;
					yPos = spawn01.top + rand() % spawn01.height;
					entities.push_back(new Enemy(Tank("tank3"), WeaponTank("weapon3"), xPos, yPos));
					entenemy.push_back(entities.back());
			}
		}

        Event event;
        while (window.pollEvent(event))
        {
			Vector2i pixelPos = Mouse::getPosition(window); // забираем координаты курсора
			Vector2f pos = window.mapPixelToCoords(pixelPos); 
			p->posMouse.x = pos.x;
			p->posMouse.y = pos.y;
			if(Keyboard::isKeyPressed(Keyboard::W)) p->move = Up;
			else if(Keyboard::isKeyPressed(Keyboard::S)) p->move = Down;
			else p->move = None;
			p->Left = (Keyboard::isKeyPressed(Keyboard::A)) ? true : false;
			p->Right = (Keyboard::isKeyPressed(Keyboard::D)) ? true : false;
			p->LeftClick = (Mouse::isButtonPressed(Mouse::Left)) ? true : false;

			if (event.type == Event::Closed)
				window.close();
			if (Keyboard::isKeyPressed(Keyboard::Escape)) { 
				view.reset(FloatRect(0, 0, 1600, 900));
				window.setView(view); return 0; }
		}

		for (it = entities.begin(); it != entities.end();)
		{
			Entity *b = *it;
			if(b->name=="Enemy" || b->name=="Player")

			//window.draw(line, 2, sf::Lines);
			/*if(b->trackTimer>150){
				entities.push_front(new Track(TrackImage, b->x, b->y, 34, 28, b->sprite.getRotation()));
			}*/
			b->secondTimer = second.getElapsedTime().asSeconds();
			b->update(time);
			if (b->life == false)	{ entenemy.remove(*it); it = entities.erase(it); delete b; }
			else it++;
		}
		Packet packet;	//Для осуществления пакетной передачи дынных
		packet << static_cast<sf::Uint32>(entities.size());
		for (std::list<Entity*>::const_iterator it = entities.begin(); it != entities.end(); ++it){
			Entity* b = *it;
			packet << b;
		}
		client1.packetSend = packet;
		client2.packetSend = packet;
		
		window.setView(view);
        window.clear(Color::White);
		lvl.Draw(window);
		for (it = entities.begin(); it != entities.end(); it++){
			window.draw((*it)->sprite);
			window.draw((*it)->sprite2);
			/*Entity *b = *it;
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(b->test1.x, b->test1.y)),
				sf::Vertex(sf::Vector2f(b->test3.x, b->test3.y)),
			};
			window.draw(line, 2, sf::Lines);
			sf::Vertex line2[] =
			{
				sf::Vertex(sf::Vector2f(b->test2.x, b->test2.y)),
				sf::Vertex(sf::Vector2f(b->test4.x, b->test4.y)),
			};
			window.draw(line2, 2, sf::Lines);*/
		}
        window.display();
    }
	return 0;
}