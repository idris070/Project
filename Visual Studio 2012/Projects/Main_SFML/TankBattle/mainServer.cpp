#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <list>
using namespace std;
using namespace sf;
#include "Server/Entity.h"
#include "Server/Clients.h"
#include "src/LoadResources.h"
#include "playServer.h"

struct Character																						
{																											
   float x,y;
   int health;
   string name;
};

sf::Packet& operator <<(sf::Packet& packet, const Character& character)
{
	return packet << character.x << character.y << character.health << character.name;
}

sf::Packet& operator >>(sf::Packet& packet, Character& character)
{
   return packet >> character.x >> character.y >> character.health >> character.name;
}


int x = 0;      //координаты шара
int y = 0;
int p1_x, p1_y, p2_x, p2_y = 0;

void th1();

int main() {
	setlocale(LC_ALL, "");							// Поддержка кириллицы в консоли Windows
//***********Подключение***************//
	cout << ip << endl;
	listener.listen(2000);
	text += "Serwer";
	Thread t1(&th1);
	t1.launch();
	play();
}

void th1(){
	setlocale(LC_ALL, "");	
	client1.init();
	entities.push_back(new Player (Tank("tank3"), WeaponTank("weapon3"), SpawnPlayer.x, SpawnPlayer.y));
	it = entities.end();
	it--;
	entenemy.push_back(entities.back());
	client1.p = *it;
	//client2.init();
	//TcpSocket socket;//программный интерфейс для обеспечения обмена данными между процессамиnh
	//Packet packet;	//Для осуществления пакетной передачи дынных
	//listener.accept(socket);    //который будет содержать новое соединение
	//socket.send(text.c_str(), text.length() + 1);
	//socket.receive(buffer, sizeof(buffer), received);
	while(true){
		client1.update();
		//client2.update();
		//client1.update();
		//client2.update();
		//socket.receive(packet);				//Команда которая ожидает данных в виде пакета от клиентской части
		//if(packet >> x >> y){			//вытаскиваем значение из пакета в переменную x и у (действие уже происходит)
		//	cout << x << ":" << y<< endl; 	//..и если все прошло успешно то выводим её координаты	
		//	p1_x = x; p1_y = y;
		//}
		//packet.clear();
		//packet << static_cast<sf::Uint32>(entities.size());
		//for (std::list<Entity*>::const_iterator it = entities.begin(); it != entities.end(); ++it){
		//	Entity* b = *it;
		//	packet << b;
		//}
		//socket.send(packet);	//Отправка данных
		//packet.clear();			//Чистим пакет
	}
}