#pragma once
#include "string"
#include "winsock.h"
class Connector
{
private:
	void readThread(); // Для потока с чтением 
	bool readingTh = false; // Нужно ли потоку читать с сервера
	sockaddr_in addr;
	WSADATA ws;
	SOCKET sock; // сокет
	bool init = false;
	int sendData(char* chars);
	int recvData(char* buffer);
	bool sendFile(std::fstream& file); // Отправляет файл при приёме сервером 
	// Стадии подключения
	bool connectFinish(std::string pass); // Обмен паролями
	bool connectsIs(); // Проверка подключены ли  
	void filesSynchronized(); // Производим синхронизацию файлов с базой проверяя изменения после сравнивая(Сообщаем серверу о всех удалённых/изменёных файлах чтобы он внёс изменения) с серверными данными и сообщая ему о требуемых изменениях 
	// Шифрование
	//Пока отсутствует
	/* Тут описаны команды которые может выполнить клиент изза сервера*/
	bool getFile(std::fstream& file, int sizefile); // Принимает файл от сервера при подаче команды от сервера о приёме
	//
public:
	Connector(const std::string& ip,int port);
	int synchronized(); // Синхронизирует и копирует и отправляет файлы 
	/*
	Тут описаны возможные команды
	*/
	int connection();
	bool isConnect();
	int disconnect();
	//
};

