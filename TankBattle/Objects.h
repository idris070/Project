using namespace sf;
using namespace std;


Vector2f playerPos;
Image BulletImage;
float PassedTime;
int** Ground;

int intersection(Vector2i a1, Vector2i a2, Vector2i b)
{
	float ax1,ay1,ax2,ay2,bx1,by1,bx2,by2;
	float v1,v2,v3,v4,size;
	b.x +=16;
	b.y +=16;

	size = 16;
	ax1 = a1.x;
	ay1 = a1.y;
	ax2 = a2.x;
	ay2 = a2.y;
	bx1 = b.x+size;
	by1 = b.y-size;
	bx2 = b.x-size;
	by2 = b.y+size;
	for (int i = 0; i < 2; i++)
	{
		if(i==1)
		{
			bx1 = b.x-size;
			by1 = b.y-size;
			bx2 = b.x+size;
			by2 = b.y+size;
		}
		v1=(bx2-bx1)*(ay1-by1)-(by2-by1)*(ax1-bx1);
		v2=(bx2-bx1)*(ay2-by1)-(by2-by1)*(ax2-bx1);
		v3=(ax2-ax1)*(by1-ay1)-(ay2-ay1)*(bx1-ax1);
		v4=(ax2-ax1)*(by2-ay1)-(ay2-ay1)*(bx2-ax1);

		if(v1*v2<=0 && v3*v4<=0)
		{
			//cout << "True X:Y" <<b.x<<":"<<b.y << endl;
			return true;
		}
	}
	//cout << "False" << endl;
	return false;
}

Texture explos[24];

list <Vector2i> search(Vector2i s, Vector2i finP);

#include "level.h"
#include "Entity.h"
#include "Explosion.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
#include "Track.h"
#include "Wall.h"

//list <Vector2i> search(Vector2i s)
//{
//	Sort.clear(); wave.clear(); Way.clear();
//	int finishRay[51][25] = {0};
//	int step[51][25] = {0};
//	Vector2i fin(27,15);
//	wave.push_back(history(s.x,s.y,1));
//	step[s.x][s.y] = 1;
//	int waveCol = 0;
//	for (itWave = wave.begin(); itWave != wave.end();)
//	{
//		waveCol++;
//		if(waveCol>51*25)
//		{
//			cout << "break:50000+" << endl;
//			break;
//		}
//		history b = *itWave;
//
//		//if(b.pos.x>0)
//		if(step[b.x-1][b.y]==0 && Ground[b.x-1][b.y]==0)
//			wave.push_back(history(b.x-1,b.y,b.step+1)),
//			step[b.x-1][b.y] = b.step+1;
//		//if(b.pos.x<50)
//		if(step[b.x+1][b.y]==0 && Ground[b.x+1][b.y]==0)
//			wave.push_back(history(b.x+1,b.y,b.step+1)),
//			step[b.x+1][b.y] = b.step+1;
//		//if(b.pos.y>0)
//		if(step[b.x][b.y-1]==0 && Ground[b.x][b.y-1]==0)
//			wave.push_back(history(b.x,b.y-1,b.step+1)),
//			step[b.x][b.y-1] = b.step+1;
//		//if(b.pos.y<24)
//		if(step[b.x][b.y+1]==0 && Ground[b.x][b.y+1]==0)
//			wave.push_back(history(b.x,b.y+1,b.step+1)),
//			step[b.x][b.y+1] = b.step+1;
//
//		if(fin==Vector2i(b.x,b.y))
//		{
//			int x = fin.x, y = fin.y;
//			finishRay[x][y] = step[x][y];
//			Sort.push_front(Vector2i(x*32+16, y*32+16));
//			while (true)
//			{
//				if(finishRay[x][y]==step[x-1][y]+1 && finishRay[x][y]>1)
//				{
//					finishRay[x-1][y]=step[x-1][y];
//					x -=1;
//					Sort.push_front(Vector2i(x*32+16, y*32+16));
//				}
//				if(finishRay[x][y]==step[x+1][y]+1 && finishRay[x][y]>1)
//				{
//					finishRay[x+1][y]=step[x+1][y];
//					x +=1;
//					Sort.push_front(Vector2i(x*32+16, y*32+16));
//				}
//				if(finishRay[x][y]==step[x][y-1]+1 && finishRay[x][y]>1)
//				{
//					finishRay[x][y-1]=step[x][y-1];
//					y -=1;
//					Sort.push_front(Vector2i(x*32+16, y*32+16));
//				}
//				if(finishRay[x][y]==step[x][y+1]+1 && finishRay[x][y]>1)
//				{
//					finishRay[x][y+1]=step[x][y+1];
//					y +=1;
//					Sort.push_front(Vector2i(x*32+16, y*32+16));
//				}
//				// если путь построен
//				if(finishRay[x][y]<=1)
//				{
//					itSort = Sort.begin();
//					Vector2i start((itSort->x-16)/32, (itSort->y-16)/32);
//					for(itSort = Sort.end(); itSort != Sort.begin();)
//					{
//						restart:
//						itSort--;
//						Vector2i finish((itSort->x-16)/32, (itSort->y-16)/32);
//						for (int x = 0; x < 51; x++)
//						for (int y = 0; y < 25; y++)
//						if(Ground[x][y]==1)
//						if(intersection(start,finish,Vector2i(x,y)))
//						{
//							goto restart;
//						}
//						// ≈сли нет пересечений
//						Way.push_back(Vector2i(finish.x,finish.y));
//						itWay = Way.end();
//						itWay--;
//						if(itWay->x == fin.x && itWay->y == fin.y)
//						{
//							return Way;
//							break;
//						}
//						start = finish;
//						itSort = Sort.end();
//					}
//					break;
//				}
//
//			}
//			break;
//		}
//	itWave++;
//	}
//}