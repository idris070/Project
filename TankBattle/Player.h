class Player: public Entity {
public:
	Clock resetTrack;
	Player(Image &image, Image &image2, float X, float Y, int W, int H) :Entity(image,X,Y,W,H){
		speed = 0; health = 55555; dx = 0; dy = 0; moveTimer = 0;
		name = "Player";
		reload = 0;
		texture2.loadFromImage(image2);
		texture.setSmooth(true);
		texture2.setSmooth(true);
		sprite2.setTexture(texture2);
		sprite2.setOrigin(56/2, 56/2);
		sprite.setPosition(x, y);
		sprite2.setPosition(x, y);
	}

	void rotation(float time, Vector2f pos){
		float dX = pos.x - x;
		float dY = pos.y - y;
		float rotP = sprite2.getRotation();
		float rotation = (atan2(dY,dX)) * 180 / 3.14159265 + 90; // получаем угол в радианах и переводим его в градусы
		if(rotation<0)rotation +=360;
		if(rotP+180>360){
			if(rotation>rotP || rotation < rotP-180) sprite2.rotate(0.5*time*0.1);
			else sprite2.rotate(-0.5*time*0.1);
		}else{
			if(rotP<rotation && rotation<rotP+180) sprite2.rotate(0.5*time*0.1);
			else sprite2.rotate(-0.5*time*0.1);
		}
	}

	void update(float time)
	{
		trackTimer = resetTrack.getElapsedTime().asMilliseconds();
		if(trackTimer>150) resetTrack.restart();
		rotation(time, posMouse);
		playerPos.x = x;
		playerPos.y = y;

		float c = sin(sprite.getRotation() * 3.14 / 180);
		float d = cos(sprite.getRotation() * 3.14 / 180);

		x = x + c * time*speed;
		y = y - d * time*speed;
		for (it2 = entenemy.begin(); it2 != entenemy.end();)
		{
			Entity *b = *it2;//для удобства, чтобы не писать (*it)->
			if(checkCollision(b)){
				float dX = x - b->x; 
				float dY = y - b->y; 
				float rotation1 = (atan2(dY,dX)) * 180 / 3.14159265 + 90; // получаем угол в радианах и переводим его в градусы
				if(rotation1<0)rotation1 +=360;
				float sinMove = sin(rotation1*3.14/180);
				float cosMove = cos(rotation1*3.14/180);

				if(sinMove*b->h*sinMove*b->h>cosMove*b->w*cosMove*b->w)
					x += sinMove*time*0.1,
					x = x - c * time*speed;
				else
					y -= cosMove*time*0.1,
					y = y + d * time*speed;
				if(speed>0) speed -= 0.007;//останавливаем
				else if(speed<0) speed += 0.007;//останавливаем
				if(side==0) sprite.rotate(time*0.1);
				if(side==3) sprite.rotate(-time*0.1);
				if(b->name == "Enemy")
				{
					b->x -= sinMove;
					b->y += cosMove;
				}
			}
			it2++;
		}
		if(PassedTime>moveTimer)
		{
			moveTimer = PassedTime + 0.1;
			if(move==Up)
			{
				//if(speed<0.06)
				speed = 0.1;
			}
			else if(move==Down)
			{
				//if(speed>-0.04)
				speed = -0.08;
			}
			else if(move=None)
			{
				speed = 0;
			}
		}

		/*if(speed != 0)
		{
			if(secondTimer>moveTimer)
			{
				moveTimer = secondTimer + 0.1;
				if(speed>0){
					speed -= 0.005;
				}else if(speed<0)
				{
					speed += 0.005;
				}
				if(speed<0.01 && speed>-0.01) speed = 0;
			}
		}*/
	sprite.setPosition(x, y);
	sprite2.setPosition(x, y);
	}
};
