#include "Track.h"

Track::Track(Image &image, float X, float Y, int W, int H, float Rotation) :Entity(image,X,Y,W,H){//�� ��� ��, ������ ����� � ����� ��������� ������ (int dir)
	x = X;
	y = Y;
	rotation = Rotation;
	life = true;
	sprite.setRotation(rotation);
	sprite.setPosition(x, y);//�������� �������
}

void Track::update(float time)
{
	timeS = Time.getElapsedTime().asMilliseconds();
	float alpha = 255 - timeS/1000*100;
	sprite.setColor(Color(255,255,255,alpha));
	if(timeS >= 2500)
	life = false;
}