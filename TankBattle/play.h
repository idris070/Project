#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include "level.h"
#include "HandyMathLib\hm.h"
#include <vector>
#include <list>
#include "Objects.h"
#include "Unit.h"

void changeLevel(Level &lvl,int numberLevel) {
	if (numberLevel == 1) { lvl.LoadFromFile("map1.tmx"); }
}

sf::View view;

int width = 51;
int height = 25;
void setArray(int*** arr)
{
	*arr = new int*[width];
	for (int i = 0; i < width; i++)
    (*arr)[i] = new int[height];
	for(int i = 0; i<width; i++)
	for(int y = 0; y<height; y++)
	(*arr)[i][y] = 0;
}

int play(RenderWindow & window)
{
	tt.height = 10;

	setArray(&Ground);

	entities.clear();
	entenemy.clear();

	view.reset(FloatRect(0, 0, 1600, 900));

	Level lvl;
	lvl.LoadFromFile("map1.tmx");
	std::vector<Object> obj;
	obj = lvl.GetAllObjects();
	
	Image image, image2,bricks;
	image.loadFromFile("images/tank1b_body.png");
	image2.loadFromFile("images/tank1b_dualgun.png");
	bricks.loadFromFile("images/bricks.png");

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

	BulletImage.loadFromFile("images/bullet.png");//загрузили картинку в объект изображения
	BulletImage.createMaskFromColor(Color(0, 0, 0));//маска для пули по черному цвету

	Image TrackImage;
	TrackImage.loadFromFile("images/tank1_track.png");

	Vector2f SpawnPlayer;
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
			{
				for (int y = top; y < height; y++)
				{
					Ground[x][y] = 1;
				}
			}

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
	for (int x = 0; x < 25; x++)
		{
			for (int y = 0; y < 51; y++)
			{
				cout << Ground[y][x] << " ";
			}
			cout << endl;
		}

	entities.push_back(new Player (image, image2, SpawnPlayer.x, SpawnPlayer.y, 34, 48));
	it = entities.begin();
	entenemy.push_back(entities.back());
	//entities.push_back(new Wall (bricks, 16, 16, 32, 1200));
	//entenemy.push_back(entities.back());
	Entity *p = *it;

	Clock clock;
	Clock checker;
	Clock second;

	float spawnTank = 5;
	float location = 0;
    while (window.isOpen())
    {
		float time = clock.getElapsedTime().asMicroseconds();
		float trackTimer = checker.getElapsedTime().asMilliseconds();
		PassedTime = second.getElapsedTime().asSeconds();
		p->secondTimer = second.getElapsedTime().asSeconds();
		clock.restart();
		time = time / 800;

		Vector2i pixelPos = Mouse::getPosition(window); // забираем координаты курсора
		Vector2f pos = window.mapPixelToCoords(pixelPos); 
		p->posMouse.x = pos.x;
		p->posMouse.y = pos.y;

		playerPos.x = p->x;
		playerPos.y = p->y;

		view.setCenter(p->x, p->y); 

		if(PassedTime>spawnTank){
			spawnTank += 15;
			for(int i=0; i<1; i++)
			{
					int xPos = spawn01.left + rand() % spawn01.width;
					int yPos = spawn01.top + rand() % spawn01.height;
					entities.push_back(new Enemy(image, image2, xPos, yPos, 34, 48));
					entenemy.push_back(entities.back());
			}
		}

		if(Keyboard::isKeyPressed(Keyboard::W)) p->move = Up;
		else if(Keyboard::isKeyPressed(Keyboard::S)) p->move = Down;
		else p->move = None;
		
		if(Keyboard::isKeyPressed(Keyboard::A)){
			p->sprite.rotate((-p->speed-0.01)*time*0.5);
			p->sprite2.rotate((-p->speed-0.01)*time*0.5);
		}
		if(Keyboard::isKeyPressed(Keyboard::D)){
			p->sprite.rotate((p->speed+0.01)*time*0.5);
			p->sprite2.rotate((p->speed+0.01)*time*0.5);
		}

        Event event;
        while (window.pollEvent(event))
        {
			if (event.type == Event::Closed)
				window.close();
			if (Keyboard::isKeyPressed(Keyboard::Escape)) { 
				view.reset(FloatRect(0, 0, 1600, 900));
				window.setView(view); return 0; }
		}

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if(p->reload<p->secondTimer){
				p->reload = p->secondTimer + 0.15;
				float bulletSin = sin(p->sprite2.getRotation() * 3.14 / 180);
				float bulletCos = cos(p->sprite2.getRotation() * 3.14 / 180);
				entities.push_back(new Bullet(BulletImage, p->x+(bulletSin*56/2), p->y-(bulletCos*56/2), 64, 64, p->sprite2.getRotation(), "player")); //и передаем в конструктор пули. создается пуля
			}
		}

		for (it = entities.begin(); it != entities.end();)
		{
			Entity *b = *it;//для удобства, чтобы не писать (*it)->
			if(b->name=="Enemy" || b->name=="Player")
			if(b->trackTimer>150){
				entities.push_front(new Track(TrackImage, b->x, b->y, 34, 28, b->sprite.getRotation()));
			}
			b->secondTimer = second.getElapsedTime().asSeconds();
			b->update(time);
			if (b->life == false)	{ entenemy.remove(*it); it = entities.erase(it); delete b; }// если этот объект мертв, то удаляем его
			else it++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
		}
		window.setView(view);
        window.clear(Color::White);
		lvl.Draw(window);
		for (it = entities.begin(); it != entities.end(); it++){
			window.draw((*it)->sprite);
			window.draw((*it)->sprite2);
		}
        window.display();
    }
	return 0;
}

list <Vector2i> search(Vector2i s, Vector2i finP)
{
	struct history{ int x, y, step; history(int a,int b,int c){ x = a;	y = b;	step = c; } };
	std::list <Vector2i> Sort;
	std::list <history> wave;
	std::list <history>::iterator itWave;
	Vector2i fin(finP.x, finP.y);
	int** finishRay, **step;
	setArray(&finishRay); setArray(&step);
	int waveCol = 0, x = fin.x, y = fin.y;
	s.x /=32; s.y /= 32;
	wave.push_back(history(s.x,s.y,1)); 
	step[s.x][s.y] = 1;
	for (itWave = wave.begin(); itWave != wave.end();)
	{
		waveCol++;
		if(waveCol>50*25)	return Sort;
		history b = *itWave;
		if(fin==Vector2i(b.x,b.y))	goto Search2;
		if(step[b.x-1][b.y]==0 && Ground[b.x-1][b.y]==0){ wave.push_back(history(b.x-1,b.y,b.step+1));	step[b.x-1][b.y] = b.step+1; }
		if(step[b.x+1][b.y]==0 && Ground[b.x+1][b.y]==0){ wave.push_back(history(b.x+1,b.y,b.step+1));	step[b.x+1][b.y] = b.step+1; }
		if(step[b.x][b.y-1]==0 && Ground[b.x][b.y-1]==0){ wave.push_back(history(b.x,b.y-1,b.step+1));	step[b.x][b.y-1] = b.step+1; }
		if(step[b.x][b.y+1]==0 && Ground[b.x][b.y+1]==0){ wave.push_back(history(b.x,b.y+1,b.step+1));	step[b.x][b.y+1] = b.step+1; }
		itWave++;
	}
	return Sort;
Search2:
	finishRay[x][y] = step[x][y];
	Sort.push_back(Vector2i(x, y));
	while (finishRay[x][y]>1)
	{
		if(finishRay[x][y]==step[x-1][y]+1 && finishRay[x][y]>1){ finishRay[x-1][y]=step[x-1][y];	x -=1;	Sort.push_back(Vector2i(x, y));	}
		if(finishRay[x][y]==step[x+1][y]+1 && finishRay[x][y]>1){ finishRay[x+1][y]=step[x+1][y];	x +=1;	Sort.push_back(Vector2i(x, y));	}
		if(finishRay[x][y]==step[x][y-1]+1 && finishRay[x][y]>1){ finishRay[x][y-1]=step[x][y-1];	y -=1;	Sort.push_back(Vector2i(x, y));	}
		if(finishRay[x][y]==step[x][y+1]+1 && finishRay[x][y]>1){ finishRay[x][y+1]=step[x][y+1];	y +=1;	Sort.push_back(Vector2i(x, y)); }
	}
	return Sort;
}