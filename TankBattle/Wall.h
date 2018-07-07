class Wall: public Entity {
public:
	Clock resetMove,resetTrack;
	int randX, randY;
	float randB;
	Wall(Image &image, float X, float Y, int W, int H) :Entity(image,X,Y,W,H){
		x = X; y = Y; w = W; h = H; moveTimer = 0;
		name = "Wall";
		sizeWidth = w;
		sizeHeight = h;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w/2, h/2);
		sprite.setPosition(x, y);
	}

	void update(float time)
	{

	}
};