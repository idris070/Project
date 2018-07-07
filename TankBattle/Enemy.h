class Enemy: public Entity {
public:
	Clock resetMove,resetTrack;
	int randX, randY,targetX,targetY;
	float randB, moveTimer2;
	std::list <Vector2i> Act;
	std::list <Vector2i>::iterator itAct;
	Enemy(Image &image, Image &image2, float X, float Y, int W, int H) :Entity(image,X,Y,W,H){
		x = X; y = Y; w = W; h = H; moveTimer2 = 0; moveTimer = 0;
		name = "Enemy";
		randX = 1 + rand() % 1300 - x;
		randY = 1 + rand() % 1000 - y;
		randB = 1 + rand() % 3;
		speed = 0.4; health = 20; dx = 0; dy = 0;
		life = true;
		texture.loadFromImage(image);
		texture2.loadFromImage(image2);
		sprite.setTexture(texture);
		sprite2.setTexture(texture2);
		sprite.setOrigin(w/2, h/2);
		sprite.setPosition(x, y);
		sprite2.setOrigin(56/2, 56/2);
		sprite2.setPosition(sprite.getPosition().x,sprite.getPosition().y);
	}

	void TrackWay()
	{
		if(Act.empty())
		{
			targetX = rand() % 51;
			targetY = rand() % 25;
			Act = search(Vector2i(x, y), Vector2i(targetX,targetY));
			for (itAct = Act.begin(); itAct != Act.end();)
			{
				Vector2i b = *itAct;
				int CorrectX = 16;
				int CorrectY = 16;
				if(Ground[b.x-1][b.y-1]==1 || Ground[b.x-1][b.y]==1 || Ground[b.x-1][b.y+1]==1)
					CorrectX += 16;
				if(Ground[b.x+1][b.y-1]==1 || Ground[b.x+1][b.y]==1 || Ground[b.x+1][b.y+1]==1)
					CorrectX -= 16;
				if(Ground[b.x-1][b.y-1]==1 || Ground[b.x][b.y-1]==1 || Ground[b.x+1][b.y-1]==1)
					CorrectY += 16;
				if(Ground[b.x-1][b.y+1]==1 || Ground[b.x][b.y+1]==1 || Ground[b.x+1][b.y+1]==1)
					CorrectY -= 16;
				itAct->x = b.x*32+CorrectX;
				itAct->y = b.y*32+CorrectY;
				itAct++;
			}
		}
		float angl = sprite.getRotation() *3.14/180;
		vec2 vecX = rotate(vec2(0,1),angl);
		vec2 vecY = rotate(vec2(1,0),angl);
		Vector2i c1((-h/2*vecX.x + (-w/2-5)*vecY.x) + x, (-h/2*vecX.y + (-w/2-5)*vecY.y) + y);
		Vector2i c2((-h/2*vecX.x + (w/2+5)*vecY.x) + x, (-h/2*vecX.y + (w/2+5)*vecY.y) + y);

		for (itAct = Act.begin(); itAct != Act.end();)
		{
			Vector2i c = *itAct;
			int distance = sqrt((c.x - x)*(c.x - x) + (c.y - y)*(c.y - y));
			if(distance<30){Act.clear(); break;}			
restart2:
			if(itAct == Act.end())
			{
				//Act = search(Vector2i(x, y), Vector2i(targetX,targetY));
				break;
			}
			Vector2i f1((0*vecX.x + -w/2*vecY.x) + itAct->x, (0*vecX.y + -w/2*vecY.y) + itAct->y);
			Vector2i f2((0*vecX.x + w/2*vecY.x) + itAct->x, (0*vecX.y + w/2*vecY.y) + itAct->y);
			for (int x = 0; x < 51; x++)
			for (int y = 0; y < 25; y++)
			if(Ground[x][y]==1)
			if(intersection(c1,f1,Vector2i(x*32, y*32)) || intersection(c2,f2,Vector2i(x*32, y*32)))
			{
				itAct++;
				goto restart2;	
			}
			Vector2i b = *itAct;

			test1.x = f1.x; test1.y = f1.y;
			test2.x = f2.x; test2.y = f2.y;
			test3.x = c1.x; test3.y = c1.y;
			test4.x = c2.x; test4.y = c2.y;
			randX = b.x - x;
			randY = b.y - y;
			break;
		}

	}

	void rotation(float time, Vector2f pos){
		float dX = pos.x - x; // вектор колинеарный прямой, которая пересекает спрайт и курсор
		float dY = pos.y - y; // вектор колинеарный прямой, которая пересекает спрайт и курсор
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
		moveTimer = resetMove.getElapsedTime().asSeconds();
		trackTimer = resetTrack.getElapsedTime().asMilliseconds();

		float SinSprite = sin(sprite.getRotation() * 3.14 / 180);
		float CosSprite = cos(sprite.getRotation() * 3.14 / 180);

		TrackWay();

		if(moveTimer > randB) {
			randB = 10 + rand() % 30;
			randB /= 10;
			float bulletSin = sin(sprite2.getRotation() * 3.14 / 180);
			float bulletCos = cos(sprite2.getRotation() * 3.14 / 180);
			entities.push_back(new Bullet(BulletImage, x+(bulletSin*56/2), y-(bulletCos*56/2), 16, 16, sprite2.getRotation(), "enemy"));
			resetMove.restart();
		}
		
		if(trackTimer>150) resetTrack.restart();
		float rotP = sprite.getRotation();
		float rot = (atan2(randY,randX)) * 180 / 3.14159265 + 90; // получаем угол в радианах и переводим его в градусы
		if(rot<0)rot +=360;
		if(rotP+180>360){
			if(rot>rotP || rot < rotP-180) sprite.rotate(0.5*time*0.1), sprite2.rotate(0.5*time*0.1);
			else sprite.rotate(-0.5*time*0.1), sprite2.rotate(-0.5*time*0.1);
		}else{
			if(rotP<rot && rot<rotP+180) sprite.rotate(0.5*time*0.1),sprite2.rotate(0.5*time*0.1);
			else sprite.rotate(-0.5*time*0.1), sprite2.rotate(-0.5*time*0.1);
		}

		x = x + SinSprite * time*speed;
		y = y - CosSprite * time*speed;
		for (it2 = entenemy.begin(); it2 != entenemy.end();)
		{
			Entity *b = *it2;//для удобства, чтобы не писать (*it)->
			if(checkCollision(b)){
				float dX = x - b->x;
				float dY = y - b->y;
				float rotation1 = (atan2(dY,dX)) * 180 / 3.14159265 + 90;
				if(rotation1<0)rotation1 +=360;
				float sinMove = sin(rotation1*3.14/180);
				float cosMove = cos(rotation1*3.14/180);

				if(sinMove*b->h*sinMove*b->h>cosMove*b->w*cosMove*b->w)
					x += sinMove,
					x = x - SinSprite * time*speed;
				else
					y -= cosMove,
					y = y + CosSprite * time*speed;
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
		if(secondTimer>moveTimer2)
		{
			moveTimer2 = secondTimer + 0.1;
			speed = 0.05;
			//if(speed<0.01 && speed>-0.01) speed = 0;
		}

		sprite.setPosition(x, y);
		sprite2.setPosition(x, y);

		rotation(time, playerPos);
	}
	void reset(){
		resetTrack.restart();
	}
};