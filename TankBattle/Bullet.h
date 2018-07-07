class Bullet :public Entity//класс пули
{
public:
	float rotation,c,d;
	Clock Time;
	float timeS;
	Image img;
	Bullet(Image &image, float X, float Y, int W, int H, float Rotation, string Used) :Entity(image,X,Y,W,H){//всё так же, только взяли в конце состояние игрока (int dir)
		//obj = lvl.GetObjects("solid");//инициализируем .получаем нужные объекты для взаимодействия пули с картой
		name = "Bullet";
		control = Used;
		img = image;
		rotation = Rotation;
		speed = 0.6;
		size = 16;
		c = sin(rotation * 3.14 / 180);
		d = cos(rotation * 3.14 / 180);
		sprite.setRotation(Rotation);
	}

	FloatRect getRect(){
		return FloatRect(x-sizeWidth/2, y-sizeHeight/2, sizeWidth, sizeHeight);
	}

	void update(float time)
	{
		w = 12;
		h = 38;
		timeS = Time.getElapsedTime().asSeconds();
		x += c * time*speed;
		y -= d * time*speed;
		if(timeS >= 5)	life = false;
		if(timeS > 0.2)
		if(1){
			for (it2 = entenemy.begin(); it2 != entenemy.end();)
			{
				Entity *enemy2 = *it2;
				//if (getRect().intersects(enemy2->getRect()))
				if(checkCollision(enemy2))
				//if (sprite.getGlobalBounds().intersects(enemy2->sprite.getGlobalBounds()))
				{
					enemy2->health -=5;
					if(enemy2->health <= 0)
					{
						enemy2->life = false;
					}
					life = false;
					entities.push_back(new Explosion(BulletImage, x, y, 64, 64, rotation));
				}
				it2++;
			}
		}
		sprite.setPosition(x, y);//задается позицию пуле
	}
};