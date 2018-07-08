#include <SFML/Graphics.hpp>
//#include "map.h"
#include "view.h"
#include <iostream>
#include <sstream>
#include "mission.h"
#include "iostream"
#include "level.h"
#include <vector>
#include <list>

#include "TinyXML/tinyxml.h"

using namespace sf;
////////////////////////////////////Общий класс-родитель//////////////////////////
class Entity {
public:
	virtual void update(float time) = 0;
	std::vector<Object> obj;//вектор объектов карты
	float dx, dy, x, y, speed,moveTimer;
	int w,h,health;
	bool life, isMove, onGround;
	float speedX;
	float speedY;
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image &image, String Name, float X, float Y, int W, int H){
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speedX = 0;
		speedY = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}

	FloatRect getRect(){//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
	}
	void randomMapGenerate(){

	int randomElementX = 0;
	int randomElementY = 0;
	srand(time(0));
	int countStone =1;

	//while (countStone > 0) {
		//randomElementX = 1 + rand() % (obj[i].rect.left - 1);
		//randomElementY = 1 + rand() % (HEIGHT_MAP - 1);
		if(1 == '1') {
			//TileMap[randomElementY][randomElementX] = 's';
			std::cout << "coordinate of Stone X:" << 1 << "\n" << "coordinate of Stone Y:" << 2 << "\n\n";
			countStone--;
		}
	//}
}
};
////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay } state;
	int playerScore;

	Player(Image &image, String Name, Level &lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H ){
		   playerScore = 0; state = stay; obj = lev.GetAllObjects();//инициализируем.получаем все объекты для взаимодействия персонажа с картой
		   if (name == "Player1"){
			   sprite.setTextureRect(IntRect(0, 0, w, h));
		   }
	   }

	   void control(){
		   if (Keyboard::isKeyPressed){
			   if (Keyboard::isKeyPressed(Keyboard::Left)) {
				   state = left; speed = 0.1;
			   }
			   if (Keyboard::isKeyPressed(Keyboard::Right)) {
				   state = right; speed = 0.1;
			   }

			   if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround)) {
				   state = jump; dy = -0.6; onGround = false;
			   }

			   if (Keyboard::isKeyPressed(Keyboard::Down)) {
				   state = down;
			   }
		   }
	   } 

	   

	   void checkCollisionWithMap(float Dx, float Dy, float time)
	   {
		  		x += time*speedX;
				for (int i = 0; i<obj.size(); i++)//проходимся по объектам
				if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
				{
					if (obj[i].name == "solid")
					{
						if(speedX > 0)
						{
							x = obj[i].rect.left - w;
							speedX =  -speedX/4;
							break;
						}
						else if(speedX < 0)
						{
							x = obj[i].rect.left + obj[i].rect.width;
							speedX =  -speedX/4;
							break;
						}
					}
				}
				y += time*speedY;
				for (int i = 0; i<obj.size(); i++)//проходимся по объектам
				if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
				{
					if (obj[i].name == "solid")
					{
						if(speedY > 0)
						{
							y = obj[i].rect.top - h;
							speedY =  -speedY/4;
							break;
						}
						else if(speedY < 0)
						{
							y = obj[i].rect.top + obj[i].rect.height;
							speedY =  -speedY/4;
							break;
						}
					}
				}

		   for (int i = 0; i<obj.size(); i++)//проходимся по объектам
		   if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
		   {
			   if (obj[i].name == "solid")//если встретили препятствие
			   {
				   if (Dy>0)	{ y = obj[i].rect.top - h;  dy = 0; onGround = true; }
				   if (Dy<0)	{ y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				   if (Dx>0)	{ x = obj[i].rect.left - w; }
				   if (Dx<0)	{ x = obj[i].rect.left + obj[i].rect.width; }
			   }
		   }
	   }
	    
	   void update(float time)
	   {
		   control();
		   sprite.rotate(1);
		   switch (state)
		   {
		   case right:dx = speed; break;
		   case left:dx = -speed; break;
		   case up: break;
		   case down: dx = 0; break;
		   case stay: break;
		   }
		   x += dx*time;
		   checkCollisionWithMap(dx, 0, time);
		   y += dy*time;
		   checkCollisionWithMap(0, dy, time);
		   sprite.setPosition(x + w / 2, y + h / 2);
		   if (health <= 0){ life = false; }
		   if (!isMove){ speed = 0; }
		   setPlayerCoordinateForView(x, y);
		   if (life) { setPlayerCoordinateForView(x, y); }
		   //dy = dy + 0.0015*time;
		   randomMapGenerate();
	   } 
};



class Enemy :public Entity{
public:
	Enemy(Image &image, String Name,Level &lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H){
		obj = lvl.GetObjects("solid");//инициализируем.получаем нужные объекты для взаимодействия врага с картой
		if (name == "EasyEnemy"){
			sprite.setTextureRect(IntRect(0, 0, w, h));
			dx = 0.1;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i<obj.size(); i++)//проходимся по объектам
		if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
		{
			//if (obj[i].name == "solid"){//если встретили препятствие (объект с именем solid)
				if (Dy>0)	{ y = obj[i].rect.top - h;  dy = 0; onGround = true; }
				if (Dy<0)	{ y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				if (Dx>0)	{ x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
				if (Dx<0)	{ x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
			//}
		}
	}

	void update(float time)
	{
		if (name == "EasyEnemy"){
			//moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//меняет направление примерно каждые 3 сек
			checkCollisionWithMap(dx, 0);
			x += dx*time;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0){ life = false; }
		}
	}
};

int main()
{
	RenderWindow window(VideoMode(1300, 820), "Lesson 22. kychka-pc.ru");
	view.reset(FloatRect(0, 0, 1300, 820));

	Level lvl;//создали экземпляр класса уровень
	lvl.LoadFromFile("map.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.
	
	std::list<Entity*>  entities;//создаю список, сюда буду кидать объекты.например врагов.
	std::list<Entity*>::iterator it;//итератор чтобы проходить по эл-там списка

	std::vector<Object> e = lvl.GetObjects("EasyEnemy");//все объекты врага на tmx карте хранятся в этом векторе


	Image heroImage;
	heroImage.loadFromFile("images/red_p2.png");

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/shamaich.png");
	easyEnemyImage.createMaskFromColor(Color(255, 0, 0));

	Object player=lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него

	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 80, 80);//передаем координаты прямоугольника player из карты в координаты нашего игрока

	for (int i = 0; i < e.size(); i++)//проходимся по элементам этого вектора(а именно по врагам)
	entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 200, 97));//и закидываем в список всех наших врагов с карты
	
	Clock clock;

	bool isMove = false;
	float dX = 0;
	float dY = 0;
	float lenght = 0;
	int tempX = 0; // временая координата Х. снимаем ее после нажатия прав клавиши
	int tempY = 0; // коoрд Y
	float distance = 0; // расстояние от объекта до точки
	int gameTime = 0;
	int createObjectForMapTimer = 0;



	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 800;

		Vector2i pixelPos = Mouse::getPosition(window); // забираем координаты курсора
		Vector2f pos = window.mapPixelToCoords(pixelPos); // переводим их в игровые ( уходим от коорд окна )
		
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();	

			if(event.type == Event::MouseButtonPressed) // если нажата клавиша мыши
				if(event.key.code == Mouse::Left) // а именно левая
					if(p.sprite.getGlobalBounds().contains(pos.x, pos.y)) // и при этом координата курсора попадает в спрайт
					{
						std::cout << "isClicked\n";
						dX = pos.x - p.sprite.getPosition().x; // делаем разность между позицией курсора и спрайта
						dY = pos.y - p.sprite.getPosition().y;
						isMove = true; // можем двигать 
					}
					if(event.type == Event::MouseButtonReleased) // если отпустили клавишу
						if(event.key.code == Mouse::Left) // а именно левую
							isMove = false; // то не можем двигать спрайт
							p.sprite.setColor(Color::White);
		}
		if (isMove) {// если можем двигать
			p.sprite.setColor(Color(0,50,0)); // красим спрайт в зеленый
			dX = pos.x - p.sprite.getPosition().x; // делаем разность между позицией курсора и спрайта
			dY = pos.y - p.sprite.getPosition().y;
			lenght =  sqrt(pow(dX, 2)+pow(dY, 2));
			if(dX>0){
				if(p.speedX < 5.25){
				p.speedX += pow((dX/lenght),2)*0.0015;
				}
			}else{
				if(p.speedX > -5.25){
				p.speedX -= pow((dX/lenght),2)*0.0015;
				}
			}
			if(dY>0){
				if(p.speedY < 5.25){
				p.speedY += pow((dY/lenght),2)*0.0015;
				}
			}else{
				if(p.speedY > -5.25){
				p.speedY -= pow((dY/lenght),2)*0.0015;
				}
			}
		//p.sprite.setPosition(p.x, p.y);
		}

		p.update(time);
		viewmap(time);
		for (it = entities.begin(); it != entities.end(); it++) { (*it)->update(time);}//для всех элементов списка(пока это только враги,но могут быть и пули к примеру) активируем ф-цию update
		window.setView(view);
		window.clear(Color(77,83,140));
		lvl.Draw(window);//рисуем новую карту
		
		for (it = entities.begin(); it != entities.end(); it++){
			window.draw((*it)->sprite); //рисуем entities объекты (сейчас это только враги)
		}
		window.draw(p.sprite);
		window.display();
	}
	return 0;
}