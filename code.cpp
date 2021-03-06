// YandexQuest.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>

using namespace std;

struct vector2i {
	int x, y;
	vector2i() {
		x = 0; y = 0;
	}
	vector2i(int _x, int _y) {
		x = _x;
		y = _y;
	}
};
struct rectangle {
	vector2i a1, a2, a3, a4;
};
class SomeClass

{
public:

	SomeClass()

	{
		cout << "\nОбычный конструктор\n";
		size_x1 = 1024;
		size_x2 = 1024 * 1024;
		x1 = new char[size_x1];
		x2 = new char[size_x2];
		cin.getline(x1, size_x1);
	}
	SomeClass(const SomeClass &obj)
	{
		this->size_x1 = obj.size_x1;
		this->size_x2 = obj.size_x2;

		x1 = new char[obj.size_x1];
		x2 = new char[obj.size_x2];

		for (int i = 0; i < obj.size_x1; i++)
		{
			this->x1[i] = obj.x1[i];
		}
		for (int i = 0; i < obj.size_x2; i++)
		{
			this->x2[i] = obj.x2[i];
		}
		cout << "\nКонструктор копирования\n";
	}
	~SomeClass()
	{
		cout << x1;
		cout << "\nДестркуктор\n";
		delete [] x1;
		delete [] x2;
	}

private:
	char *x1;
	char *x2;
	int size_x1, size_x2;
};
void funcShow(SomeClass object)
{
	cout << "\nФункция принимает объект, как параметр\n";
}

SomeClass funcReturnObject()
{
	SomeClass object;
	cout << "\nФункция возвращает объект\n";
	return object;
}

void AlgoritmGame()
{
	vector<uint16_t> intNumber;
	uint16_t countCard = 0;

	uint16_t scorePetya = 0, scoreVasya = 0;

	cout << "Введите кол-во карточек: " << endl;
	while (true) {
		cin >> countCard;
		if (bool(countCard % 3) == true) {
			cout << "Input Error!" << endl;
			continue;
		}
		if (!cin)
		{
			cout << "Input Error!" << endl;
			cin.clear();
			fflush(stdin);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else {
			break;
		}
	}

	for (size_t i = 0; i < countCard; i++)
	{
		uint16_t number;
		cin >> number;
		while (!cin)
		{
			cout << "Input Error!" << endl;
			cin.clear();
			fflush(stdin);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> number;
		}
		intNumber.push_back(number);
	}
	
	for (auto it = intNumber.begin(); it != intNumber.end(); it++)
	{
		uint16_t petya = (*it);
		scorePetya += petya;
		it++;
		uint16_t vasya = (*it);
		scoreVasya += vasya;
		it++;
		if (petya > vasya) {
			scoreVasya += (*it);
		}
		else {
			scorePetya += (*it);
		}
	}

	if (scorePetya > scoreVasya)	cout << "Petya Win" << endl;
	else	cout << "Vasya Win" << endl;
}
void HardNumber() {

}
void ValletsAndCoins() {
	int countWallets = 0;			// кол-во кошельков
	int countCoinsWallets = 0;			// монет в кошельке
	int countCoins = 0;				// общее кол-во монет
	int totalcoins = 0;
	
	cout << "Кол-во кошельков: ";
	cin >> countWallets;
	for (int i = 0; i < countWallets; i++)
	{
		cout << "Кол-во монет в кошельке " << i+1 << endl;
		cin >> countCoinsWallets;
		countCoins += countCoinsWallets;
	}
	cout << "Кол-во денег в кошельках: " << endl;
	cin >> totalcoins;
	cout << "общее кол-во монет в кошельках" << countCoins << endl;
	if (countCoins == totalcoins) {
		cout << "Yes" << endl;
	}
	else cout << "No" << endl;
}
void NestedRectangles() {
	int countRectangle = 5;
	vector<vector2i*> cordinate;
	/*cordinate.push_back(new vector2i(0,1));
	cordinate.push_back(new vector2i(1,0));
	cordinate.push_back(new vector2i(1,1));
	cordinate.push_back(new vector2i(2,1));
	cordinate.push_back(new vector2i(3,2));*/
	cordinate.push_back(new vector2i(-1,-1));
	cordinate.push_back(new vector2i(0,0));
	cordinate.push_back(new vector2i(1,1));

	int countStep = 0;
	while (!cordinate.empty()) {
		countStep++;
		auto it = cordinate.begin();
		int x = (*it)->x;
		int y = (*it)->y;
		vector<rectangle> rectV;
		rectangle rectLT = {
			vector2i(x - 1, y + 1),
			vector2i(x, y + 1),
			vector2i(x, y),
			vector2i(x - 1, y)
		};
		rectangle rectRT = {
			vector2i(x, y + 1),
			vector2i(x + 1, y + 1),
			vector2i(x + 1, y),
			vector2i(x, y),
		};
		rectangle rectRB = {
			vector2i(x, y),
			vector2i(x + 1, y),
			vector2i(x + 1, y - 1),
			vector2i(x, y - 1),
		};
		rectangle rectLB = {
			vector2i(x - 1, y),
			vector2i(x, y),
			vector2i(x, y - 1),
			vector2i(x - 1, y - 1),
		};
		rectV.push_back(rectLT);
		rectV.push_back(rectRT);
		rectV.push_back(rectRB);
		rectV.push_back(rectLB);
		int countMax = 0;
		vector<vector2i*> saveCount;
		for (auto rect = rectV.begin(); rect != rectV.end(); rect++) {
			vector<vector2i*> testSave;
			int countR = 0;
			for (auto it = cordinate.begin(); it < cordinate.end(); it++)
			{
				vector2i *b = *it;
				rectangle r = *rect;
				if (b->x == r.a1.x && b->y == r.a1.y ||
					b->x == r.a2.x && b->y == r.a2.y ||
					b->x == r.a3.x && b->y == r.a3.y ||
					b->x == r.a4.x && b->y == r.a4.y) {
					testSave.push_back(*it);
					countR++;
				}
			}
			if (countR > countMax) {
				countMax = countR;
				saveCount = testSave;
			}
			testSave.clear();
			cout << "CountR: " << countR << endl;
		}
		while (!saveCount.empty()) {
			auto it = saveCount.begin();
			vector2i *b = *it;
			for (auto it2 = cordinate.begin(); it2 != cordinate.end(); it2++) {
				if ((*it2)->x == b->x && (*it2)->y == b->y) {
					cordinate.erase(it2);
					break;
				}
			}
			saveCount.erase(it);
			delete b;
		}
	}
	cout << "Количество шагов: " << countStep << endl;
}
void PlayingNumbers() {
	int countNumber = 0;
	vector<int> number;
	cin >> countNumber;
	for (size_t i = 0; i < countNumber; i++)
	{
		int n = 0;
		cin >> n;
		number.push_back(n);
	}
	int counter = 0;
	while (true) {
		bool start = true;
		int *min = NULL;
		int *max = NULL;
		for (auto it = number.begin(); it != number.end(); it++) {
			if (start) {
				min = &(*it);
				max = &(*it);
				start = false;
			}
			else {
				min = ((*min) < (*it) ? min : &(*it));
				max = ((*max) > (*it) ? max : &(*it));
			}
		}
		if ((*max) == (*min)) break;
		*max -= (*min);
		counter++;
		//cout << "Min = " << (*min) << endl;
		//cout << "Max = " << (*max) << endl;
	}
	cout << "Кол-во итераций: " << counter << endl;
}

int main() {
	setlocale(LC_ALL, "rus");

	cout << "1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	SomeClass obj1;
	cout << "2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	funcShow(obj1);
	cout << "3-4 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	funcReturnObject();
	cout << "5 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	SomeClass obj2 = obj1;

	SomeClass obj1;
	SomeClass obj2(obj1);
	SomeClass obj3 = obj1;

	SomeClass obj4;
	SomeClass obj5 = obj4;

	//AlgoritmGame();		// 1
	//ValletsAndCoins();	// 4
	//NestedRectangles();		// 11
	//PlayingNumbers();		// 12
	
	//SomeClass a2 = a;
	//funcShow(a);

	//const char *str = "образец строки";
	//char  buf[32];
	//char *x1;
	//x1 = new char[1024*1024];
	//cin.getline(x1, 1024*1024);

	//for (int i = 0; x1[i] != '\0'; ++i) //Цикл до символа окончания строки
	//{
	//	cout << x1[i]; //Выводим посимвольно строку
	//	if (i != 0 && i%5 == 0) cout << "\t"; //Каждые k символов ставим знак табуляции
	//}
	//cout << endl;

}