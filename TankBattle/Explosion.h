class Explosion :public Entity//����� ����
{
public:
	float rotation;
	Clock Time;
	float timeS;
	Explosion(Image &image, float X, float Y, int W, int H, float Rotation) :Entity(image,X,Y,W,H){//�� ��� ��, ������ ����� � ����� ��������� ������ (int dir)
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
		Texture current;
		int num;
		num = timeS/30;
		sprite.setTexture(explos[num]);

		if(timeS >= 30*24)
		life = false;
	}
};