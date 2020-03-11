#define _CRT_SECURE_NO_WARNINGS
#include "Connector.h"
#include "cmath"
//debuf include

#include <iostream>
#include <fstream>
using namespace std;

Connector::Connector(const string ip, int port,string catalog) { // Присваивает значения
	// Стартуем сокеты
	this->catalog = catalog;
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
//
int Connector::getFile(fstream& file,int size_file,const string& path) {
	return 1;
}
int Connector::sendFile(fstream& file, const string& path) { // Файл должен быть открыт в бинарном виде
	recvMasg = false;
	// начало
	sendData("FCIF", true); // Отправляем команду о начале передеачи данных
	sendData(path.c_str(), true); // Отправляем путь до файла
	string buffer;
	char_m bs;
	bs.chars = new char[32];
	bs.length = 32;
	if (file.is_open()) {
		file.seekg(0, ios_base::end);
		int length = file.tellg();
		buffer = std::to_string(length);
		sendData(buffer.c_str(), true); // Отправляем данные
		// Принимаем от сервера ответ на данные
		recvData(bs);
		uncodingBytes(bs);
		buffer = "okey";
		if (buffer._Equal(bs.chars)) { // Сравниваем
			// Тут начинаем передачу файла
			char_m rs;
			if (length < 4096) { //
				rs.chars = new char[length];
				rs.length = length;
				file.read(rs.chars,length);
				// Тут делаем  единоразовую отправку
				codingBytes(rs); // Кодируем  байты
				sendData(rs);
				recvData(bs);
				uncodingBytes(bs);
				if (buffer._Equal(bs.chars)) { //Если всё верно
					sendData("SucClose", true); // Завершена передача
				}
				else { // Сервер выслал неверный ответ
					errorC(3); // Проблема покдлючения
				}
			}
			else {
				rs.chars = new char[4096];
				rs.length = 4096;
				file.seekg(0, ios_base::beg);
				cout << "len-" << length << "\n";
				for (int i = 0; i < ceil(length/4096.0);i++) {
					if (i != ceil(length / 4096.0) - 1) {
						file.read(rs.chars, 4096);
					}
					else {
						rs.length = length - file.tellg();
						file.read(rs.chars,length-(int)file.tellg());
					}
					cout << i << "--" << file.tellg() << "()\n";
					codingBytes(rs);
					sendData(rs);
					recvData(bs);
					uncodingBytes(bs);
					if (!buffer._Equal(bs.chars)) { // Проверяем ответ от сервера
						delete[]bs.chars;
						delete[]rs.chars;
						errorC(3); // проблемыа соединения с сервером
						return 3;
					}
					if (file.eof()) {// Если равное концу файла
						cout << "eof\n";
						cout << rs.chars << "\n";
						break;
					}
				}
				sendData("SucClose", true); // Завершена передача
			}
			cout << "end\n";
			delete[]bs.chars;
			delete[]rs.chars;
			return 0;
		}
		else {
			delete[]bs.chars;
			recvMasg = true;
			return 3;
		}
	}
	else {
		sendData("stop",true); // Посылаем сообщение об остановке передачи
		errorC(6); // Ошибка открытия файла
	}
	delete[]bs.chars;
	recvMasg = true;
	return 0;
}

int Connector::deleteFileServer(const string& path) { // Посылаем  серверу команду  от удалении файла
	return 1;
}
int Connector::updateFile(const string& path) { // Посылаем команду на обновление
	return 1;
}
//
int Connector::connection() { // Тут только создаём подключение с стандартными проверками
	if (SOCKET_ERROR == (connect(sock,(sockaddr*)&addr,sizeof(addr)))) { 
		return WSAGetLastError(); // Возвращаем ошибку
	}
	else {
		return 0;
	}
	// После подключения нужно произвести авторизацию произведя finishConnect
}
bool Connector::isConnect() {
	return connected;
}
int Connector::equalBytes(const char* buffer1, const char* buffer2) { // Сравнивает только начальные байты минимального из буферов
		if (strlen(buffer1) > strlen(buffer2)) {
			for (int i = 0; i < strlen(buffer1); i++) {
				if (buffer1[i] != buffer2[i]) {
					return 1;
				}
			}
		}
		else {
			for (int i = 0; i < strlen(buffer2); i++) {
				if (buffer1[i] != buffer2[i]) {
					return 1;
				}
			}
		}
	
	return 0;
}
int Connector::connectFinish(const string& pass) {
	char_m buffer_m;
	buffer_m.chars = new char[buffer_pass];
	buffer_m.length = buffer_pass;
	if (recvData(buffer_m)!=0) {
		errorC(2);
	}
	if (!equalBytes(buffer_m.chars,"FAS")) {
		if (copyBytes(buffer_m, pass.c_str())) {
			errorC(10);
			return 10;
		}
		codingBytes(buffer_m.chars);
		cout << buffer_m.chars << "\n";
		if (sendData(buffer_m)!=0) {
			errorC(3);
			return 3;
		}
		if (recvData(buffer_m)!=0) {
			errorC(2);
			return 2;
		}
		if (equalBytes("ok",buffer_m.chars)) {
			cout << buffer_m.chars << "\n";
			errorC(1);
			return 1;
		}
		connected = true;
	}
	else {
		errorC(4);
		return 4;
	}
	return 0;
}
int Connector::copyBytes(char_m& buffer, const char* input) {
	if (buffer.length >= strlen(input)) {
		for (int o = strlen(input)-1; o < buffer.length;o++) {
			buffer.chars[o] = 0; 
		}
		for (int i = 0; i < strlen(input); i++) {
			buffer.chars[i] = input[i];
		}
		return 0;
	}
	else {
		return 10; // Возвращаем ошибку 10 несоотвествие размеров
	}
}
// Тут пилим  логер и действия при ошибке
void Connector::errorC(int error) {
	connected = false;
	disconnect();
}

//Доделать шифрование
bool Connector::execCommands(queue<vector<string>> qCommands) { // Исполняем команды
	while (!qCommands.empty()) {
		vector<string> str = qCommands.front();
		qCommands.pop();
		if (str.at(0)=="create") {
			fstream file = fstream(catalog+"\\"+str.at(1), ios_base::in | ios_base::out | ios_base::binary | ios_base::ate);
			sendFile(file,str.at(1)); // Отправляет файл серверу
		}
		else if (str.at(0) == "update") { 
			updateFile(str.at(1)); // Отправляем серверу запрос на обновление
		}
		else if (str.at(0) == "delete") {
			deleteFileServer(str.at(1)); // Отправляем запрос на удаление файлов
		}
		else { // Если комманда не обнаружена
			
		}
	
	}
	return true;
}
int Connector::codingBytes(char_m bytes) {
	return 0;
}
char* Connector::codingBytes(const char* bytes) {
	return	_strdup(bytes);
}
int Connector::uncodingBytes(char_m bytes) {
	return 0;
}
int Connector::sendData(char_m& buffer) { // Данные для предачи
	if (SOCKET_ERROR == (send(sock,buffer.chars,buffer.length,NULL))) {
		return WSAGetLastError();
	}
	return 0;
}
int Connector::sendData(const char* str, bool coding) { // Отправка константных сообщений
	if (coding) {
		char* buffer = codingBytes(str);
		if (SOCKET_ERROR == (send(sock, buffer, strlen(buffer), NULL))) {
			free(buffer);
			return WSAGetLastError();
		}
		free(buffer);
		return 0;
	}
	else {
		if (SOCKET_ERROR == (send(sock, str, strlen(str), NULL))) {
			return WSAGetLastError();
		}
		return 0;
	}
}
int Connector::recvData(char_m& buffer) { // Приём данных
	int actual_len = 0;
		if (SOCKET_ERROR == (actual_len = recv(sock, buffer.chars, buffer.length, 0))) {
			return WSAGetLastError();
		}
		if (actual_len < buffer.length)
			for (int i = actual_len; i < buffer.length; i++) {
				buffer.chars[i] = 0;
			}
		if (!actual_len) {
			return 2;
		}
	return 0;
}
int Connector::synchronized() { // Синххронизация
	filesSynchronized(); // Синхронизируем и делаем изменения на сервере
	return 1;
}
void Connector::filesSynchronized() { 
	
}
int Connector::disconnect() {
	char_m a;
	a.chars = new char[5];
	a.length = 5;
	copyBytes(a,"disc");
	sendData(a);
	return closesocket(sock); // Закрываем сокет
}
