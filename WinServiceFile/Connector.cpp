#include "Connector.h"
using namespace std;
Connector::Connector(const string& ip, int port) { // Присваивает значения
	// Стартуем сокеты
	if (FAILED(WSAStartup(MAKEWORD(2, 2), &ws))) // Если инициализация провалена
	{
		return;
	}
	if (INVALID_SOCKET==(sock = socket(AF_INET,SOCK_STREAM,0))) {
		return;
	}
	addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	init = true;
	
}
int Connector::connection() { // Тут только создаём подключение с стандартными проверками
	if (SOCKET_ERROR == (connect(sock,(sockaddr*)&addr,sizeof(addr)))) { // Проивзошла ошибка
		return WSAGetLastError(); // Возвращаем ошибку
	}
	else {
		return 0;
	}
	// После подключения нужно произвести авторизацию произведя finishConnect
}
int Connector::sendData(char* buffer) { // Данные для предачи
	if (SOCKET_ERROR == (send(sock,buffer,sizeof(buffer),NULL))) {
		return WSAGetLastError();
	}
	return 0;
}
int Connector::recvData(char* buffer) { // Приём данных
	int actual_len = 0;
	if (SOCKET_ERROR == (actual_len = recv(sock,buffer,sizeof(buffer),0))) {
		return WSAGetLastError();
	}
}
int Connector::synchronized() { // Синххронизация
	filesSynchronized(); // Синхронизируем и делаем изменения на сервере
}
void Connector::filesSynchronized() { 
	
}
int Connector::disconnect() { 
	return closesocket(sock); // Закрываем сокет
}