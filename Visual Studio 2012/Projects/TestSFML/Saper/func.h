#include <SFML\Graphics.hpp>
#include <iostream>;
#include <ctime>;
#include <sstream>;

using namespace sf;

View view;




int mineCount = 70;
int widthMap = 20;
int heightMap = 20;

int windowWidth = widthMap*16+64;
int windowHeight = heightMap*16+64;

Text text;

char** click = new char*[widthMap];
float*** coords = new float**[widthMap];
char** object = new char*[widthMap];

void createMine();

void createObCl(){
	for(int i=0; i<widthMap; i++){
		click[i] = new char[heightMap];
		coords[i] = new float*[heightMap];
		object[i] = new char[heightMap];
	}
	for(int i=0; i<widthMap; i++)
	for(int j=0; j<heightMap; j++){
		click[i][j] = 0;
		object[i][j] = 0;
		coords[i][j] = new float[1];
		coords[i][j][0] = 32+i*16;
		coords[i][j][1] = 32+j*16;
	}

}

void checkWin(){
	int win = 1;
	for (int i = 0; i < widthMap; i++)
	for (int j = 0; j < heightMap; j++)
	{
		if(click[i][j] == false)
		{
			win=0;
		}
	}
	if(win){
		std::cout << "You Win!\n";
		for(int i=0; i<widthMap; i++)
			for(int j=0; j<heightMap; j++){
				click[i][j] = true;
			}
	}
}

void reset_game(){
	for(int i = 0; i < widthMap; i++)
	{
		delete []click[i];    
		delete []object[i];
		delete []coords[i];
	}
	delete [] click;
	delete [] object;
	delete [] coords;

	widthMap = 30;
	heightMap = 30;
	mineCount = 70;
	
	click = new char*[widthMap];
	object = new char*[widthMap];
	coords = new float**[widthMap];
	createObCl();
	std::ostringstream textString;
	textString << mineCount;
	text.setString(textString.str());
	createMine();

	windowWidth = widthMap*16+64;
	windowHeight = heightMap*16+64;
}

void createMine() {
	if(mineCount < widthMap*heightMap){
		srand( time( 0 ) );
		int max_i = 0;
		for(int i=0; i<mineCount; i++){
			max_i++;
			int x = 0 + rand() % widthMap;
			int y = 0 + rand() % heightMap;
			if(object[x][y] == 'm'){
				i -= 1;
			}
			object[x][y] = 'm';
		}
	}
	for (int x = 0; x < widthMap; x++)
	for (int y = 0; y < heightMap; y++)
	{
		int count_num = 0;
		if(x>0 && y>0 && object[x-1][y-1]=='m') count_num++;
		if(x>0 && object[x-1][y]=='m') count_num++;
		if(x>0 && y<heightMap-1 && object[x-1][y+1]=='m') count_num++;
		if(y>0 && object[x][y-1]=='m') count_num++;
		if(y<heightMap-1 && object[x][y+1]=='m') count_num++;
		if(x<widthMap-1 && y>0 && object[x+1][y-1]=='m') count_num++;
		if(x<widthMap-1 && object[x+1][y]=='m') count_num++;
		if(x<widthMap-1 && y<heightMap-1 && object[x+1][y+1]=='m') count_num++;
		if(object[x][y] != 'm'){
			object[x][y] = count_num;
		}
	}
}

void clear0(int x, int y){ // при нажатие на пустую клетку очистить все пустые клетки рядом
	bool lose = false;
	if(click[x][y] == false)
	{
		click[x][y] = true;
		if(object[x][y] == 0){
			if(x>0)clear0(x-1,y);
			if(x<widthMap-1)clear0(x+1,y);
			if(y>0)clear0(x,y-1);
			if(y<heightMap-1)clear0(x,y+1);
		}else if(object[x][y] == 'm')
		{
			for(int i=0; i<widthMap; i++)
			for(int j=0; j<heightMap; j++){
				click[i][j] = true;
			}
			lose = true;
			std::cout << "You LOse :( :( :( !\n";
		}
		if(!lose)
		checkWin();
	}
}
