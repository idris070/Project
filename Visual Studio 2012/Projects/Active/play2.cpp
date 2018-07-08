#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include <list>
#include <vector>

using namespace sf;
using namespace std;

// ����� ������ // 

Vector2f playerPos;
Image BulletImage2;

class Entity2 {
public:
	//std::vector<Object> obj;
	float dx, dy, x, y, speed,moveTimer,trackTimer;
	int w,h,health;
	bool life, isMove, onGround;
	bool player;
	Texture texture,texture2;
	Sprite sprite,sprite2;
	String name;
	Entity2(Image &image, float X, float Y, int W, int H){
		x = X; y = Y; w = W; h = H; //name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}

	FloatRect getRect(){
		return FloatRect(x, y, w, h);
	}

	virtual void update(float time) = 0;//��� ������� �������������� ��� �-���

};

std::list<Entity2*>  entities;
std::list<Entity2*>  entenemy;
std::list<Entity2*>::iterator it;
std::list<Entity2*>::iterator it2;//������ ��������.��� �������������� ����� ��������� ������

class Bullet :public Entity2//����� ����
{
public:
	float vx,vy, distance;
	float rotation,c,d;
	Clock Time;
	float timeS;
	Bullet(Image &image, float X, float Y, int W, int H, float Rotation) :Entity2(image,X,Y,W,H){//�� ��� ��, ������ ����� � ����� ��������� ������ (int dir)
		//obj = lvl.GetObjects("solid");//�������������� .�������� ������ ������� ��� �������������� ���� � ������
		x = X;
		y = Y;
		rotation = Rotation;
		speed = 3;
		w = h = 16;
		life = true;
		sprite.setOrigin(w/2, h/2);
		c = sin(rotation * 3.14 / 180);
		d = cos(rotation * 3.14 / 180);
	}


	void update(float time)
	{
		timeS = Time.getElapsedTime().asSeconds();
		x = x + c * time*speed;
		y = y - d * time*speed;
		if (x <= 0) x = 1;// �������� ���� � ����� �����, ����� ��� ���������� ������ ��� �������� �� �������� �� ������ ����� � �� ���� ������
		if (y <= 0) y = 1;

		if(timeS >= 5)
		life = false;

		sprite.setPosition(x, y);//�������� ������� ����
	}
};


class Player: public Entity2 {
public:
	Player(Image &image, Image &image2, float X, float Y, int W, int H) :Entity2(image,X,Y,W,H){
		player = true;
		x = X; y = Y; w = W; h = H; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		texture2.loadFromImage(image2);
		//texture.setSmooth(true);
		sprite.setTexture(texture);
		sprite2.setTexture(texture2);
		sprite.setOrigin(w/2, h/2);
		sprite.setPosition(x, y);
		sprite2.setOrigin(56/2, 56/2);
		sprite2.setPosition(sprite.getPosition().x,sprite.getPosition().y);
	}

	void rotation(float time, Vector2f pos){
		float dX = pos.x - x; // ������ ����������� ������, ������� ���������� ������ � ������
		float dY = pos.y - y; // ������ ����������� ������, ������� ���������� ������ � ������
		float rotP = sprite2.getRotation();
		float rotation = (atan2(dY,dX)) * 180 / 3.14159265 + 90; // �������� ���� � �������� � ��������� ��� � �������
		if(rotation<0)rotation +=360;
		if(rotP+180>360){
			if(rotation>rotP || rotation < rotP-180) sprite2.rotate(0.5*time);
			else sprite2.rotate(-0.5*time);
		}else{
			if(rotP<rotation && rotation<rotP+180) sprite2.rotate(0.5*time);
			else sprite2.rotate(-0.5*time);
		}
	}

	void update(float time)
	{
		playerPos.x = x;
		playerPos.y = y;
	}
};

class Enemy: public Entity2 {
public:
	Clock resetMove,resetTrack;
	int randX, randY;
	Enemy(Image &image, Image &image2, float X, float Y, int W, int H) :Entity2(image,X,Y,W,H){
		x = X; y = Y; w = W; h = H; moveTimer = 0;
		name = "Enemy";
		randX, randY = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		texture2.loadFromImage(image2);
		sprite.setTexture(texture);
		sprite2.setTexture(texture2);
		sprite.setOrigin(w/2, h/2);
		sprite.setPosition(x, y);
		sprite2.setOrigin(56/2, 56/2);
		sprite2.setPosition(sprite.getPosition().x,sprite.getPosition().y);
	}

	void rotation(float time, Vector2f pos){
		float dX = pos.x - x; // ������ ����������� ������, ������� ���������� ������ � ������
		float dY = pos.y - y; // ������ ����������� ������, ������� ���������� ������ � ������
		float rotP = sprite2.getRotation();
		float rotation = (atan2(dY,dX)) * 180 / 3.14159265 + 90; // �������� ���� � �������� � ��������� ��� � �������
		if(rotation<0)rotation +=360;
		if(rotP+180>360){
			if(rotation>rotP || rotation < rotP-180) sprite2.rotate(0.5*time);
			else sprite2.rotate(-0.5*time);
		}else{
			if(rotP<rotation && rotation<rotP+180) sprite2.rotate(0.5*time);
			else sprite2.rotate(-0.5*time);
		}
	}

	void update(float time)
	{
		moveTimer = resetMove.getElapsedTime().asSeconds();
		trackTimer = resetTrack.getElapsedTime().asMilliseconds();
		if(moveTimer > 3) {
			float bulletSin = sin(sprite2.getRotation() * 3.14 / 180);
			float bulletCos = cos(sprite2.getRotation() * 3.14 / 180);
			entities.push_back(new Bullet(BulletImage2, x+(bulletSin*56/2), y-(bulletCos*56/2), 16, 16, sprite2.getRotation()));

			randX = 1 + rand() % 1300 - x;
			randY = 1 + rand() % 1000 - y;
			resetMove.restart();
		}
		if(trackTimer>150) resetTrack.restart();
		float dX = randX; // ������ ����������� ������, ������� ���������� ������ � ������
		float dY = randY; // ������ ����������� ������, ������� ���������� ������ � ������
		float rotP = sprite.getRotation();
		float rot = (atan2(dY,dX)) * 180 / 3.14159265 + 90; // �������� ���� � �������� � ��������� ��� � �������
		if(rot<0)rot +=360;
		if(rotP+180>360){
			if(rot>rotP || rot < rotP-180) sprite.rotate(0.5*time), sprite2.rotate(0.5*time);
			else sprite.rotate(-0.5*time), sprite2.rotate(-0.5*time);
		}else{
			if(rotP<rot && rot<rotP+180) sprite.rotate(0.5*time),sprite2.rotate(0.5*time);
			else sprite.rotate(-0.5*time), sprite2.rotate(-0.5*time);
		}

		float c = sin(sprite.getRotation() * 3.14 / 180);
		float d = cos(sprite.getRotation() * 3.14 / 180);
		dx = c;
		dy = d;
		x = x + c * time;
		for (it2 = entenemy.begin(); it2 != entenemy.end();)
				{
					//Entity2 *enemy2 = *it2;
					if(*it2 != this)
					{
						Entity2 *enemy2 = *it2;
						if (getRect().intersects(enemy2->getRect()))
						{
							if (dx>0)//���� ���� ���� ������
									{
										x = x - c * time;
										//x = enemy2->x - w; //����������� ��� �� ������ ����� (�������)
										dx = 0;//�������������
									}
									if (dx < 0)//���� ���� ���� �����
									{
										x = x - c * time;
										//x = enemy2->x + enemy2->w; //���������� - ����������� ������
										dx = 0;//�������������
									}
						}
					}
					it2++;
				}
		y = y - d * time;
		for (it2 = entenemy.begin(); it2 != entenemy.end();)
		{
			//Entity2 *enemy2 = *it2;
			if(*it2 != this)
			{
				Entity2 *enemy2 = *it2;
				if (getRect().intersects(enemy2->getRect()))
				{
					if (dy>0)//���� ���� ���� ������
					{
						y = y + d * time;
						//y = enemy2->y + h; //����������� ��� �� ������ ����� (�������)
						dy = 0;//�������������
					}
					if (dy < 0)//���� ���� ���� �����
					{
						y = y + d * time;
						//y = enemy2->y - enemy2->h; //���������� - ����������� ������
						dy = 0;//�������������
					}
				}
			}

			it2++;
		}

		sprite.setPosition(x, y);
		sprite2.setPosition(x, y);

		//rotation(time, playerPos);
	}
	void reset(){
		resetTrack.restart();
	}
};

class Track :public Entity2//����� ����
{
public:
	float rotation;
	Clock Time;
	float timeS;
	Track(Image &image, float X, float Y, int W, int H, float Rotation) :Entity2(image,X,Y,W,H){//�� ��� ��, ������ ����� � ����� ��������� ������ (int dir)
		x = X;
		y = Y;
		rotation = Rotation;
		life = true;
		sprite.setRotation(rotation);
		sprite.setPosition(x, y);//�������� �������
	}
	void update(float time)
	{
		timeS = Time.getElapsedTime().asMilliseconds();
		float alpha = 255 - timeS/1000*100;
		sprite.setColor(Color(255,255,255,alpha));
		if(timeS >= 2500)
		life = false;
	}
};

// ----- //

int play32(RenderWindow & window)
{
	Image image, image2;
	image.loadFromFile("images/tank1b_body.png");
	image2.loadFromFile("images/tank1b_dualgun.png");

	BulletImage2.loadFromFile("images/bullet.png");//��������� �������� � ������ �����������
	BulletImage2.createMaskFromColor(Color(0, 0, 0));//����� ��� ���� �� ������� �����

	Image TrackImage;
	TrackImage.loadFromFile("images/tank1_track.png");

	Player p(image, image2, 400, 300, 34, 48);
	Player p2(image, image2, 333, 222, 34, 48);

	entities.clear();
	entenemy.clear();

	entities.push_back(new Player(image, image2, 222, 222, 34, 48)); 
	for(int i=0; i<35; i++)
	{
		 int xPos = 1 + rand() % 1000;
		 int yPos = 1 + rand() % 800;
		 entities.push_back(new Enemy(image, image2, xPos, yPos, 34, 48));
		 //it3 = entities.begin();
		 entenemy.push_back(entities.back());
		 //entenemy.push_back(*it3);
	}
	int countIt3 = 0;
	//for (it3 = entities.begin(); it3 != entities.end();)//������� ��� ���������� �� ������ �� �����
	//	{
	//		Entity2 *b = *it3;//��� ��������, ����� �� ������ (*it)->
	//		if(b->name=="Enemy"){
	//			entenemy.push_back(*it3);
	//		}
	//		it3++;
	//	}
	//for (it = entenemy.begin(); it != entenemy.end();)
	//{
	//	//cout << (*it)->x << endl;
	//	it++;
	//}

	Clock clock;
	Clock checker;

    while (window.isOpen())
    {
		float time = clock.getElapsedTime().asMicroseconds();
		float trackTimer = checker.getElapsedTime().asMilliseconds();
		clock.restart();
		time = time / 5000;

		Vector2i pixelPos = Mouse::getPosition(window); // �������� ���������� �������
		Vector2f pos = window.mapPixelToCoords(pixelPos); 
		p.rotation(time, pos);
		p2.rotation(time, pos);

		float c = sin(p.sprite.getRotation() * 3.14 / 180);
		float d = cos(p.sprite.getRotation() * 3.14 / 180);

		playerPos.x = p.x;
		playerPos.y = p.y;

		if(Keyboard::isKeyPressed(Keyboard::W)){
			p.x = p.x + c * time;
			p.dx = c;
			if (p.getRect().intersects(p2.getRect()))
			{
				if (p.dx>0)//���� ���� ���� ������
						{
							p.x = p2.x - p.w; //����������� ��� �� ������ ����� (�������)
							p.dx = 0;//�������������
						}
						if (p.dx < 0)//���� ���� ���� �����
						{
							p.x = p2.x + p2.w; //���������� - ����������� ������
							p.dx = 0;//�������������
						}
			}
			p.y = p.y - d * time;
			p.dy = d;
			if (p.getRect().intersects(p2.getRect()))
			{
				if (p.dy>0)//���� ���� ���� ������
						{
							p.y = p2.y + p.h; //����������� ��� �� ������ ����� (�������)
							p.dy = 0;//�������������
						}
						if (p.dy < 0)//���� ���� ���� �����
						{
							p.y = p2.y - p2.h; //���������� - ����������� ������
							p.dy = 0;//�������������
						}
			}
			p.sprite.setPosition(p.x, p.y);
			p.sprite2.setPosition(p.x, p.y);
			if(trackTimer>150){
				entities.push_back(new Track(TrackImage, p.x, p.y, 34, 28, p.sprite.getRotation()));
				checker.restart();
			}
		}
		if(Keyboard::isKeyPressed(Keyboard::S)){
			p.x = p.x - c * time;
			p.dx = -c;
			if (p.getRect().intersects(p2.getRect()))
			{
				if (p.dx>0)//���� ���� ���� ������
						{
							p.x = p2.x - p.w; //����������� ��� �� ������ ����� (�������)
							p.dx = 0;//�������������
						}
						if (p.dx < 0)//���� ���� ���� �����
						{
							p.x = p2.x + p2.w; //���������� - ����������� ������
							p.dx = 0;//�������������
						}
			}
			p.y = p.y + d * time;
			p.dy = -d;
			if (p.getRect().intersects(p2.getRect()))
			{
				if (p.dy>0)//���� ���� ���� ������
						{
							p.y = p2.y + p.h; //����������� ��� �� ������ ����� (�������)
							p.dy = 0;//�������������
						}
						if (p.dy < 0)//���� ���� ���� �����
						{
							p.y = p2.y - p2.h; //���������� - ����������� ������
							p.dy = 0;//�������������
						}
			}
			p.sprite.setPosition(p.x, p.y);
			p.sprite2.setPosition(p.x, p.y);
			if(trackTimer>150){
				entities.push_back(new Track(TrackImage, p.x, p.y, 34, 28, p.sprite.getRotation()));
				checker.restart();
			}
		}
		if(Keyboard::isKeyPressed(Keyboard::A)){
			p.sprite.rotate(-0.5*time);
			p.sprite2.rotate(-0.5*time);
		}
		if(Keyboard::isKeyPressed(Keyboard::D)){
			p.sprite.rotate(0.5*time);
			p.sprite2.rotate(0.5*time);
		}

        Event event;
        while (window.pollEvent(event))
        {
			if (event.type == Event::Closed)
				window.close();
			if (Keyboard::isKeyPressed(Keyboard::Escape)) return 0;

			if (event.type == Event::MouseButtonPressed)//���� ������ ������� ����
			if (event.key.code == Mouse::Left) //� ������ �����, �� �������� 
			{ //� ��� �� ������� ���������� ���� � ���� �����
				Vector2i pixelPos = Mouse::getPosition(window);//�������� ����� �������
				Vector2f pos = window.mapPixelToCoords(pixelPos);//��������� �� � ������� (������ �� ����� ����)
				float bulletSin = sin(p.sprite2.getRotation() * 3.14 / 180);
				float bulletCos = cos(p.sprite2.getRotation() * 3.14 / 180);
				entities.push_back(new Bullet(BulletImage2, p.x+(bulletSin*56/2), p.y-(bulletCos*56/2), 16, 16, p.sprite2.getRotation())); //� �������� � ����������� ����. ��������� ����
				entenemy.push_back(entities.back());
				//shoot.play();//������ ���� ����
			}
		}

		for (it = entities.begin(); it != entities.end();)//������� ��� ���������� �� ������ �� �����
		{
			//entenemy.clear();
			//for (it3 = entities.begin(); it3 != entities.end();)//������� ��� ���������� �� ������ �� �����
			//{
			//	Entity2 *b = *it3;//��� ��������, ����� �� ������ (*it)->
			//	if(b->name=="Enemy"){
			//		entenemy.push_back(*it3);
			//	}
			//	it3++;
			//}
			

			Entity2 *b = *it;//��� ��������, ����� �� ������ (*it)->
			if(b->name=="Enemy") 
			if(b->trackTimer>150){
			//entities.push_back(new Track(TrackImage, b->x, b->y, 34, 28, b->sprite.getRotation()));
			}
			b->update(time);//�������� �-��� update ��� ���� �������� (�� ���� ��� ���, ��� ���)
			if (b->life == false)	{ entenemy.remove(*it); it = entities.erase(it); delete b; }// ���� ���� ������ �����, �� ������� ���
			else it++;//� ���� �������� (����������) � ���� �������. ��� ������ �� ����� ��������� ������
		}

        window.clear(Color::White);
		for (it = entities.begin(); it != entities.end(); it++){
			window.draw((*it)->sprite);
			window.draw((*it)->sprite2);
		}
		window.draw(p.sprite);
		window.draw(p.sprite2);
		window.draw(p2.sprite);
		window.draw(p2.sprite2);
        window.display();
    }
	return 0;
}