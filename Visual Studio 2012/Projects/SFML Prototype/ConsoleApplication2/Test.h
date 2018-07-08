#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
#include "mission.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include "level.h"
#include <vector>
#include <list>

using namespace sf; // �������� ������������ ���� sf, ����� ��������� �� ������ sf::

///////////////////////����� ������ /////////////////////////
class Player { // ����� ������
private:
	//float x, y;
public:
	std::vector<Object> obj; //������ �������� �����
	float w, h, dx, dy, x, y, speed, x_r, y_r;
	int dir, playerScore, health;
	bool life, isMove, isSelect;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	Player(String F, int X, int Y, float W, float H, float x_r, float y_r){
		x, y, w, h, dx, dy, speed = 0, health = 100;
		life = true; isMove = false; isSelect = false;
		dir = 0; playerScore = 0;
		File = F;
		w = W; h = H;
		image.loadFromFile("images/" + File);
		image.createMaskFromColor(Color(255, 255, 255));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setTextureRect(IntRect(x_r, y_r, w, h));
		sprite.setOrigin(w / 2, h / 2);
	}
	void update (float time)
	{
		switch (dir)
		{
		case 0: dx = speed; dy = 0; break;
		case 1: dx = -speed; dy = 0; break;
		case 2: dx = 0; dy = speed; break;
		case 3: dx = 0; dy = -speed; break;
		}

		x += dx*time;
		y += dy*time;

		speed = 0;
		sprite.setPosition(x, y);
		interactionWithMap();
		if (health <= 0){ life = false; }

	}
	
	FloatRect getRect(){//�-��� ��������� ��������������. ��� �����,������� (���,�����).
		return FloatRect(x, y, w, h);//��� �-��� ����� ��� �������� ������������ 
	}

	float getPlayerCoordinateX(){
		return x;
	}
	float getPlayerCoordinateY(){
		return y;
	}

	void interactionWithMap()
	{
		for(int i = (y - h / 2) / 32; i < (y + h / 2) / 32; i++) // ������������
		for(int j = (x - w / 2) / 32; j < (x + w / 2) / 32; j++) // ������������
		{
			if (TileMap[i][j] == '0')
			{
				if(dy > 0)
				{
					y =  i * 32 - h + h/2;
				}
				if(dy < 0)
				{
					y =  i * 32 + 32 + h/2;
				}
				if(dx > 0)
				{
					x =  j * 32 - w + w/2;
				}
				if(dx < 0)
				{
					x =  j * 32 + 32 + w/2;
				}
			}

			if(TileMap[i][j] == 's') {
				playerScore++;
				TileMap[i][j] = ' ';
			}

			if(TileMap[i][j] == 'f') {
				health -=40;
				TileMap[i][j] = ' ';
			}

			if(TileMap[i][j] == 'h') {
				health += 20;
				TileMap[i][j] = ' ';
			}
		}
		 for (int i = 0; i<obj.size(); i++)//���������� �� ��������
		   if (getRect().intersects(obj[i].rect))//��������� ����������� ������ � ��������
		   {
			   if (obj[i].name == "solid")//���� ��������� �����������
			   {
				   if (dy>0)	{ y = obj[i].rect.top - h;  dy = 0;}
				   if (dy<0)	{ y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				   if (dx>0)	{ x = obj[i].rect.left - w; }
				   if (dx<0)	{ x = obj[i].rect.left + obj[i].rect.width; }
			   }
		   }
	}
};

int main()
{
	RenderWindow window(sf::VideoMode(1300, 700), "Lesson 1. idris070 style");
	view.reset(sf::FloatRect(0, 0, 1300, 700));

	Level lvl;//������� ��������� ������ �������
	lvl.LoadFromFile("map.tmx");//��������� � ���� �����, ������ ������ � ������� ������� �� �� ����������.
	

	Font font;
	font.loadFromFile("CyrilicOld.ttf");
	Text text("", font, 20);
	text.setFillColor(sf::Color::Black);

	Image map_image; // ������� ������ Image (�����������)
	map_image.loadFromFile("images/map.png"); // ��������� � ���� ����

	Texture map; // ������� ������ Texture (��������)
	map.loadFromImage(map_image); // �������� � ���� ������ Image (�����������)

	Sprite s_map; // ������� ������ Sprite (������)
	s_map.setTexture(map); // �������� � ���� ������ Texture (��������)
	//herosprite.setTextureRect(IntRect(0,192,96,96));
	//herosprite.setPosition(50,25); // ������ ��������� ���������� ��������� �������

	Image quest_image;
	quest_image.loadFromFile("images/missionbg.jpg");
	quest_image.createMaskFromColor(Color(0, 0, 0));
	Texture quest_texture;
	quest_texture.loadFromImage(quest_image);
	Sprite s_quest;
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(IntRect(0, 0, 340, 510));
	s_quest.setScale(0.6f, 0.6f);

	Player p("red_p.png", 450, 450, 80, 80, 0, 0);
	//Player p("mm_blue.png", 250, 250, 12, 12);
	//p.sprite.setTextureRect(IntRect(0, 0, 128, 128));
	//p.sprite.setScale(sf::Vector2f(2.33f, 2.33f));
	//p.sprite.setOrigin(sf::Vector2f(64, 64));

	bool showMissionText = true;

	float CurrentFrame = 0; // ������ ������� ����
	Clock clock; // ������� ���������� �������, �������� �� ������� (� �� ��������� �������� ����������)
	Clock gameTimeClock;

	bool isMove = false; //���������� ��� ������ ���� �� �������
	float dX = 0; // ������������� �������� �� X
	float dY = 0;
	float lenght = 0;
	float speedX = 0;
	float speedY = 0;
	int tempX = 0; // �������� ���������� �. ������� �� ����� ������� ���� �������
	int tempY = 0; // ��o�� Y
	float distance = 0; // ���������� �� ������� �� �����
	int gameTime = 0;
	int createObjectForMapTimer = 0;

	//CircleShape shape(100.f);
	//shape.setFillColor(sf::Color::Green);

	while(window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds(); // ���� ��������� ����� � �������������
		if(p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds(); else{ view.move(0.1, 0);	}
		clock.restart(); // ������������� �����
		time = time/800; // �������� ����

		Vector2i pixelPos = Mouse::getPosition(window); // �������� ���������� �������
		Vector2f pos = window.mapPixelToCoords(pixelPos); // ��������� �� � ������� ( ������ �� ����� ���� )
		//std::cout << pixelPos.x << "\n"; // ������� �� ���������� � ������� ������� � ������� ( ��� �� ����� ������ ������ ���� )
		//std::cout << pos.x << "\n"; // ������� �� � ������� ��������������� � ������� ����������
		//std::cout<<time<<"\n";
		Event event;
		while(window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			window.close();
			/*
			if(event.type == Event::MouseButtonPressed)
				if(event.key.code == Mouse::Left){
					if(p.sprite.getGlobalBounds().contains(pos.x, pos.y))
					{
						p.sprite.setColor(Color::Red);
						p.isSelect = true;
					}
				}
				
				if(p.isSelect)
					if(event.type == Event::MouseButtonPressed)
						if(event.key.code == Mouse::Right){
							p.isMove = true;
							p.isSelect = false;
							p.sprite.setColor(Color::White);
							tempX = pos.x;// - p.w/2;
							tempY = pos.y;// - p.h/2;
							float dX = pos.x - p.x; // ������ ����������� ������, ������� ���������� ������ � ������
							float dY = pos.y - p.y; // ������ ����������� ������, ������� ���������� ������ � ������
							float rotation = (atan2(dY,dX)) * 180 / 3.14159265; // �������� ���� � �������� � ��������� ��� � �������
							std::cout << rotation << "\n"; // ������� ��  ������� � ���������
							p.sprite.setRotation(rotation); // ������������ ������ �� ��� �������
						}
						*/
						
			if(event.type == Event::MouseButtonPressed) // ���� ������ ������� ����
				if(event.key.code == Mouse::Left) // � ������ �����
					if(p.sprite.getGlobalBounds().contains(pos.x, pos.y)) // � ��� ���� ���������� ������� �������� � ������
					{
						std::cout << "isClicked\n";
						dX = pos.x - p.sprite.getPosition().x; // ������ �������� ����� �������� ������� � �������
						dY = pos.y - p.sprite.getPosition().y;
						isMove = true; // ����� ������� 
					}
					if(event.type == Event::MouseButtonReleased) // ���� ��������� �������
						if(event.key.code == Mouse::Left) // � ������ �����
							isMove = false; // �� �� ����� ������� ������
							p.sprite.setColor(Color::White);
							
			if(event.type == Event::KeyPressed)
				if((event.key.code == Keyboard::Tab)) {

					switch (showMissionText)
					{
					case true: {
								std::ostringstream playerHealthString;
								playerHealthString << p.health;
								std::ostringstream task;
								task << getTextMission(getCurrentMission(p.getPlayerCoordinateX()));
								text.setString("��������: " + playerHealthString.str() + "\n" + task.str());
								text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);
								s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);
								showMissionText = false;
								break;
					}
					case false: {
						text.setString("");
						showMissionText = true;
						break;
								}
					}
				}
		}

		if(p.isMove){
			distance = sqrt((tempX - p.x)*(tempX -p.x) + (tempY - p.y)*(tempY - p.y));

			if(distance>2){
				p.x += 0.1*time*(tempX - p.x) / distance;
				p.y += 0.1*time*(tempY - p.y) / distance;
			}
			else { p.isMove = false; std::cout << "Priehal\n"; }
			}


		
		if (isMove) {// ���� ����� �������
			p.sprite.setColor(Color(0,50,0)); // ������ ������ � �������
			dX = pos.x - p.sprite.getPosition().x; // ������ �������� ����� �������� ������� � �������
			dY = pos.y - p.sprite.getPosition().y;
			lenght =  sqrt(pow(dX, 2)+pow(dY, 2));
			if(dX>0){
				if(speedX < 5.25){
				speedX += pow((dX/lenght),2)*0.0015;
				}
			}else{
				if(speedX > -5.25){
				speedX -= pow((dX/lenght),2)*0.0015;
				}
			}
			if(dY>0){
				if(speedY < 5.25){
				speedY += pow((dY/lenght),2)*0.0015;
				}
			}else{
				if(speedY > -5.25){
				speedY -= pow((dY/lenght),2)*0.0015;
				}
			}
		//p.sprite.setPosition(p.x, p.y);
		}
		p.x += time*speedX;
				for(int i = (p.y - p.h / 2) / 32; i < (p.y + p.h / 2) / 32; i++) // ������������
				for(int j = (p.x - p.w / 2) / 32; j < (p.x + p.w / 2) / 32; j++) // ������������
				{
					if (TileMap[i][j] == '0')
					{
						if(speedX > 0)
						{
							p.x =  j * 32 - p.w + p.w/2;
							speedX =  -speedX/1.2;
							break;
						}
						else if(speedX < 0)
						{
							p.x =  j * 32 + 32 + p.w/2;
							speedX =  -speedX/1.2;
							break;
						}
						//break;
					}
				}
				p.y += time*speedY;
				for(int i = (p.y - p.h / 2) / 32; i < (p.y + p.h / 2) / 32; i++) // ������������
				for(int j = (p.x - p.w / 2) / 32; j < (p.x + p.w / 2) / 32; j++) // ������������
				{
					if (TileMap[i][j] == '0')
					{
						if(speedY > 0)
						{
							p.y =  i * 32 - p.h + p.h/2;
							speedY =  -speedY/2;
							break;
						}
						else if(speedY < 0)
						{
							p.y =  i * 32 + 32 + p.h/2;
							speedY =  -speedY/2;
							break;
						}
					}
					//break;
				}
			//std::cout << pow((dX/lenght),2) + pow((dY/lenght),2);
			//std::cout <<lenght << "\n";
			//p.sprite.setPosition(pos.x - dX, pos.y - dY);//����� � ��� ��������,���� � ��� ���� � � � � ������ ������
			//p.x = pos.x-dX; //������� ������ �� X
			//p.y = pos.y-dY; // �� Y

			//x=pow(x,y) // ���������� � �������

		
		CurrentFrame += 0.021*time;
		if(CurrentFrame > 8) CurrentFrame -=8;
		p.sprite.rotate(3*speedX);
		if(speedX>0){
			speedX = speedX-0.0001;
		}else{
			speedX = speedX-0.0001;
		}
		
		speedY = speedY-0.0001;
		//p.sprite.setTextureRect(IntRect(80 * int(CurrentFrame), 0, 80, 80));
		
		if(p.life==true){
			if(Keyboard::isKeyPressed(Keyboard::Left)) {
				p.dir = 1; p.speed = 0.1; //
				CurrentFrame += 0.005*time;
				if(CurrentFrame > 3) CurrentFrame -=3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96));
				getPlayerCoordinateForView(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());
			}
		
			if(Keyboard::isKeyPressed(Keyboard::Right)) {
				p.dir = 0; p.speed = 0.1; //
				CurrentFrame += 0.005*time;
				if(CurrentFrame > 3) CurrentFrame -=3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 96, 96));
				getPlayerCoordinateForView(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());
			}

			if(Keyboard::isKeyPressed(Keyboard::Up)) {
				p.dir = 3; p.speed = 0.1; //
				CurrentFrame += 0.005*time;
				if(CurrentFrame > 3) CurrentFrame -=3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 307, 96, 96));
				getPlayerCoordinateForView(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());
			}

			if(Keyboard::isKeyPressed(Keyboard::Down)) {
				p.dir = 2; p.speed = 0.1; //
				CurrentFrame += 0.005*time;
				if(CurrentFrame > 3) CurrentFrame -=3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
				getPlayerCoordinateForView(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());
			}
		}

		sf::Vector2i localPosition = Mouse::getPosition(window);


		if(localPosition.x < 3) { view.move(-0.2*time, 0); }
		if(localPosition.x > window.getSize().x-3) { view.move(0.2*time, 0); }
		if(localPosition.y > window.getSize().y-3) { view.move(0, 0.2*time); }
		if(localPosition.y < 3) { view.move(0, -0.2*time); }

		createObjectForMapTimer += time;
		if (createObjectForMapTimer>3000){
			//::cout << p.sprite.m_po << std::endl;
			randomMapGenerate();
			createObjectForMapTimer = 0;
		}

		p.update(time);
		viewmap(time);
		changeview();
		window.setView(view);
		window.clear(Color(128, 106, 89));

		for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if(TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 32, 32));
			if(TileMap[i][j] == 's') s_map.setTextureRect(IntRect(32, 0, 32, 32));
			if(TileMap[i][j] == '0') s_map.setTextureRect(IntRect(64, 0, 32, 32));
			if(TileMap[i][j] == 'f') s_map.setTextureRect(IntRect(96, 0, 32, 32));
			if(TileMap[i][j] == 'h') s_map.setTextureRect(IntRect(128, 0, 32, 32));

			s_map.setPosition(j * 32, i * 32);

			window.draw(s_map);
		}
		if(!showMissionText) { 
			text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);
			s_quest.setPosition(view.getCenter().x +115, view.getCenter().y - 130);
			window.draw(s_quest); window.draw(text);
		}
		window.draw(p.sprite); // ������� ������ �� �����
		sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(p.x-p.w/2, p.y-p.h/2)),
				sf::Vertex(sf::Vector2f(p.x+p.w/2, p.y-p.h/2)),
				sf::Vertex(sf::Vector2f(p.x+p.w/2, p.y+p.h/2)),
				sf::Vertex(sf::Vector2f(p.x-p.w/2, p.y+p.h/2)),
				sf::Vertex(sf::Vector2f(p.x-p.w/2, p.y-p.h/2))
			};

			//window.draw(line, 5, sf::LinesStrip);
			// define a 120x50 rectangle
			sf::RectangleShape rectangle(sf::Vector2f(p.x, 10));

			// change the size to 100x100
			rectangle.setSize(sf::Vector2f(p.w*p.health/100, 10));
			rectangle.setPosition(p.x+1-p.w/2, p.y+1-p.h/2);
			rectangle.setFillColor(Color(255,0,0));
			window.draw(rectangle);

		window.display();

	}


	return 0;
}