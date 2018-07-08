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


int x = 0;      //���������� ����
int y = 0;
int p1_x, p1_y, p2_x, p2_y = 0;

void th1();

int main() {
	setlocale(LC_ALL, "");							// ��������� ��������� � ������� Windows
//***********�����������***************//
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
	//TcpSocket socket;//����������� ��������� ��� ����������� ������ ������� ����� ����������nh
	//Packet packet;	//��� ������������� �������� �������� ������
	//listener.accept(socket);    //������� ����� ��������� ����� ����������
	//socket.send(text.c_str(), text.length() + 1);
	//socket.receive(buffer, sizeof(buffer), received);
	while(true){
		client1.update();
		//client2.update();
		//client1.update();
		//client2.update();
		//socket.receive(packet);				//������� ������� ������� ������ � ���� ������ �� ���������� �����
		//if(packet >> x >> y){			//����������� �������� �� ������ � ���������� x � � (�������� ��� ����������)
		//	cout << x << ":" << y<< endl; 	//..� ���� ��� ������ ������� �� ������� � ����������	
		//	p1_x = x; p1_y = y;
		//}
		//packet.clear();
		//packet << static_cast<sf::Uint32>(entities.size());
		//for (std::list<Entity*>::const_iterator it = entities.begin(); it != entities.end(); ++it){
		//	Entity* b = *it;
		//	packet << b;
		//}
		//socket.send(packet);	//�������� ������
		//packet.clear();			//������ �����
	}
}