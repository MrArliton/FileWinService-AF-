#pragma once
#include "string"
#include "WinSock.h"
#pragma comment (lib,"Ws2_32.lib")
class Connector
{
typedef	struct charM {
		char* chars;
		int length;
	}char_m;
private:
	//Const
	const size_t buffer_pass = 32;
	//
	void readThread(); // Для потока с чтением 
	bool readingTh = false; // Нужно ли потоку читать с сервера
	bool connected = false;
	sockaddr_in addr;
	WSADATA ws;
	SOCKET sock; // сокет
	bool init = false;
	int sendData(char_m& chars);
	int recvData(char_m& buffer);
	// Стадии подключения
	int codingBytes(char* buffer);
	int uncodingBytes(char* buffer);
	int copyBytes(char_m& buffer,const char* input);
	int equalBytes(const char* buffer1,const char* buffer2);
	void filesSynchronized(); // Производим синхронизацию файлов с базой проверяя изменения после сравнивая(Сообщаем серверу о всех удалённых/изменёных файлах чтобы он внёс изменения) с серверными данными и сообщая ему о требуемых изменениях 
	// Шифрование
	//Пока отсутствует
	/* Тут описаны команды которые может выполнить клиент изза сервера*/
	int getFile(std::fstream& file, int sizefile); // Принимает файл от сервера при подаче команды от сервера о приёме
	int sendFile(std::fstream& file); // Отправляет файл при приёме сервером 
	//
public:
	Connector(const std::string& ip,int port);
	int synchronized(); // Синхронизирует и копирует и отправляет файлы 
	/*
	Тут описаны возможные команды
	*/
	void errorC(int error);
	int connection();
	int connectFinish(const std::string& pass); // Обмен паролями
	bool isConnect();
	int disconnect();
	//
};

