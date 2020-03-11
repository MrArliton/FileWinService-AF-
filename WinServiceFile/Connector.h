#pragma once
#include "string"
#include "WinSock.h"
#include <vector>
#include <queue>
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
	std::string catalog;
	bool recvMasg = true;
	//
	void readThread(); // Для потока с чтением 
	bool readingTh = false; // Нужно ли потоку читать с сервера
	bool connected = false;
	sockaddr_in addr;
	WSADATA ws;
	SOCKET sock; // сокет
	bool init = false;
	int sendData(char_m& chars);
	int sendData(const char* str,bool coding); // Отправка константных сообщений
	int recvData(char_m& buffer);
	int deleteFileServer(const std::string& path); // Посылааем коммандуц удаления сервера 
	int updateFile(const std::string& path);
	// Стадии подключения
	int codingBytes(char_m buffer);
	char* codingBytes(const char* bytes);
	int uncodingBytes(char_m buffer);
	int copyBytes(char_m& buffer,const char* input);
	int equalBytes(const char* buffer1,const char* buffer2);
	void filesSynchronized(); // Производим синхронизацию файлов с базой проверяя изменения после сравнивая(Сообщаем серверу о всех удалённых/изменёных файлах чтобы он внёс изменения) с серверными данными и сообщая ему о требуемых изменениях 
	// Шифрование
	//Пока отсутствует
	/* Тут описаны команды которые может выполнить клиент изза сервера*/

	//
public:
	Connector(const std::string ip,int port,std::string catalog);
	bool execCommands(std::queue<std::vector<std::string>> qCommand); // Исполняем команду
	int synchronized(); // Синхронизирует и копирует и отправляет файлы 
	/*
	Тут описаны возможные команды
	*/
	void errorC(int error);
	int connection();
	int connectFinish(const std::string& pass); // Обмен паролями
	bool isConnect();
	int disconnect();
	int getFile(std::fstream& file, int sizefile, const std::string& path); // Принимает файл от сервера при подаче команды от сервера о приёме
	int sendFile(std::fstream& file, const std::string& path); // Отправляет файл при приёме сервером 
	//
};

