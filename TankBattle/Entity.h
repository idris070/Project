//  ласс Entity //

struct Cordinate
{
	float x1,x2,x3,x4,y1,y2,y3,y4;
};

struct CollisionDetect
{
	Vector2f PlayerC1,PlayerC2,PlayerC3,PlayerC4;
	Vector2f EnemyC1,EnemyC2,EnemyC3,EnemyC4;
};

enum KeyboardMove
{
	Up,Down,None
};

class Entity {
public:
	float dx, dy, x, y, speed,moveTimer,trackTimer,size,reload,secondTimer;
	int w,h,health,sizeWidth,sizeHeight,side;
	Vector2i test1,test2,test3,test4;
	bool life;
	Cordinate cord;
	Vector2f posMouse;
	Texture texture,texture2;
	Sprite sprite,sprite2;
	String name, control;
	KeyboardMove move;
	Entity(Image &image, float X, float Y, int W, int H){
		posMouse.x = 0; posMouse.y = 0; secondTimer = 0;
		x = X; y = Y; w = W; h = H; //name = Name; moveTimer = 0;
		cord.x1 = -w/2; cord.x2 = w/2; cord.x3 = w/2; cord.x4 = -w/2;
		cord.y1 = -h/2; cord.y2 = -h/2; cord.y3 = h/2; cord.y4 = h/2;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true;
		texture.loadFromImage(image);
		texture.setSmooth(true);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
		size = sqrt(w*w+h*h);
		sizeWidth = size;
		sizeHeight = size;
	}

	FloatRect getRect(){
		return FloatRect(x-sizeWidth/2, y-sizeHeight/2, sizeWidth, sizeHeight);
	}

	vec2 rotate(vec2 point, float angle){
        vec2 rotated_point;
        rotated_point.x = point.x * cos(angle) - point.y * sin(angle);
        rotated_point.y = point.x * sin(angle) + point.y * cos(angle);
        return rotated_point;
	}

	int checkCollision(Entity* b)
	{
		if (b != this && getRect().intersects(b->getRect()))
		{
			float angl = sprite.getRotation() *3.14/180;
			vec2 vecX = rotate(vec2(0,1),angl);
			vec2 vecY = rotate(vec2(1,0),angl);
			Vector2f c1((-h/2*vecX.x + -w/2*vecY.x) + x, (-h/2*vecX.y + -w/2*vecY.y) + y);
			Vector2f c2((h/2*vecX.x + -w/2*vecY.x) + x, (h/2*vecX.y + -w/2*vecY.y) + y);
			Vector2f c3((h/2*vecX.x + w/2*vecX.y) + x, (h/2*vecY.x + w/2*vecY.y) + y);
			Vector2f c4((-h/2*vecX.x + w/2*vecX.y) + x,(-h/2*vecY.x + w/2*vecY.y) + y);

			angl = b->sprite.getRotation() *3.14/180;
			vecX = rotate(vec2(0,1),angl);
			vecY = rotate(vec2(1,0),angl);
			Vector2f v1((-b->h/2*vecX.x + -b->w/2*vecY.x) + b->x, (-b->h/2*vecX.y + -b->w/2*vecY.y) + b->y);
			Vector2f v2((b->h/2*vecX.x + -b->w/2*vecY.x) + b->x, (b->h/2*vecX.y + -b->w/2*vecY.y) + b->y);
			Vector2f v3((b->h/2*vecX.x + b->w/2*vecX.y) + b->x, (b->h/2*vecY.x + b->w/2*vecY.y) + b->y);
			Vector2f v4((-b->h/2*vecX.x + b->w/2*vecX.y) + b->x, (-b->h/2*vecY.x + b->w/2*vecY.y) + b->y);

			Vector2f v5,c5;
			for (int i = 0; i < 4; i++)
			{
				if(i==0) { c5=v1; v5=c1; }
				if(i==1) { c5=v2; v5=c2; }
				if(i==2) { c5=v3; v5=c3; }
				if(i==3) { c5=v4; v5=c4; }

				float d1 = (c1.x-c5.x)*(c2.y-c1.y)-(c2.x-c1.x)*(c1.y-c5.y);
				float d2 = (c2.x-c5.x)*(c3.y-c2.y)-(c3.x-c2.x)*(c2.y-c5.y);
				float d3 = (c3.x-c5.x)*(c4.y-c3.y)-(c4.x-c3.x)*(c3.y-c5.y);
				float d4 = (c4.x-c5.x)*(c1.y-c4.y)-(c1.x-c4.x)*(c4.y-c5.y);

				float f1 = (v1.x-v5.x)*(v2.y-v1.y)-(v2.x-v1.x)*(v1.y-v5.y);
				float f2 = (v2.x-v5.x)*(v3.y-v2.y)-(v3.x-v2.x)*(v2.y-v5.y);
				float f3 = (v3.x-v5.x)*(v4.y-v3.y)-(v4.x-v3.x)*(v3.y-v5.y);
				float f4 = (v4.x-v5.x)*(v1.y-v4.y)-(v1.x-v4.x)*(v4.y-v5.y);

				if(d1<0 && d2<0 && d3<0 && d4<0 || f1<0 && f2<0 && f3<0 && f4<0){
					side = i;
					return 1;
				}
			}
		}
		return 0;
	}

	virtual void update(float time) = 0;//все потомки переопредел€ют эту ф-цию

};

std::list<Entity*> entities;
std::list<Entity*> entenemy;
std::list<Entity*>::iterator it;
std::list<Entity*>::iterator it2;//второй итератор.дл€ взаимодействи€ между объектами списка

//  ласс Entity //