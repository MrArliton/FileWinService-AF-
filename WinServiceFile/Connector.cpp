#define _CRT_SECURE_NO_WARNINGS
#include "Connector.h"
#include "cmath"
//debuf include

#include <iostream>
#include <fstream>
using namespace std;

Connector::Connector(const string ip, int port,string catalog) { // ����������� ��������
	// �������� ������
	this->catalog = catalog;
	if (FAILED(WSAStartup(MAKEWORD(2, 2), &ws))) // ���� ������������� ���������
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
int Connector::sendFile(fstream& file, const string& path) { // ���� ������ ���� ������ � �������� ����
	recvMasg = false;
	// ������
	sendData("FCIF", true); // ���������� ������� � ������ ��������� ������
	sendData(path.c_str(), true); // ���������� ���� �� �����
	string buffer;
	char_m bs;
	bs.chars = new char[32];
	bs.length = 32;
	if (file.is_open()) {
		file.seekg(0, ios_base::end);
		int length = file.tellg();
		buffer = std::to_string(length);
		sendData(buffer.c_str(), true); // ���������� ������
		// ��������� �� ������� ����� �� ������
		recvData(bs);
		uncodingBytes(bs);
		buffer = "okey";
		if (buffer._Equal(bs.chars)) { // ����������
			// ��� �������� �������� �����
			char_m rs;
			if (length < 4096) { //
				rs.chars = new char[length];
				rs.length = length;
				file.read(rs.chars,length);
				// ��� ������  ������������ ��������
				codingBytes(rs); // ��������  �����
				sendData(rs);
				recvData(bs);
				uncodingBytes(bs);
				if (buffer._Equal(bs.chars)) { //���� �� �����
					sendData("SucClose", true); // ��������� ��������
				}
				else { // ������ ������ �������� �����
					errorC(3); // �������� �����������
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
					if (!buffer._Equal(bs.chars)) { // ��������� ����� �� �������
						delete[]bs.chars;
						delete[]rs.chars;
						errorC(3); // ��������� ���������� � ��������
						return 3;
					}
					if (file.eof()) {// ���� ������ ����� �����
						cout << "eof\n";
						cout << rs.chars << "\n";
						break;
					}
				}
				sendData("SucClose", true); // ��������� ��������
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
		sendData("stop",true); // �������� ��������� �� ��������� ��������
		errorC(6); // ������ �������� �����
	}
	delete[]bs.chars;
	recvMasg = true;
	return 0;
}

int Connector::deleteFileServer(const string& path) { // ��������  ������� �������  �� �������� �����
	return 1;
}
int Connector::updateFile(const string& path) { // �������� ������� �� ����������
	return 1;
}
//
int Connector::connection() { // ��� ������ ������ ����������� � ������������ ����������
	if (SOCKET_ERROR == (connect(sock,(sockaddr*)&addr,sizeof(addr)))) { 
		return WSAGetLastError(); // ���������� ������
	}
	else {
		return 0;
	}
	// ����� ����������� ����� ���������� ����������� ��������� finishConnect
}
bool Connector::isConnect() {
	return connected;
}
int Connector::equalBytes(const char* buffer1, const char* buffer2) { // ���������� ������ ��������� ����� ������������ �� �������
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
		return 10; // ���������� ������ 10 ������������� ��������
	}
}
// ��� �����  ����� � �������� ��� ������
void Connector::errorC(int error) {
	connected = false;
	disconnect();
}

//�������� ����������
bool Connector::execCommands(queue<vector<string>> qCommands) { // ��������� �������
	while (!qCommands.empty()) {
		vector<string> str = qCommands.front();
		qCommands.pop();
		if (str.at(0)=="create") {
			fstream file = fstream(catalog+"\\"+str.at(1), ios_base::in | ios_base::out | ios_base::binary | ios_base::ate);
			sendFile(file,str.at(1)); // ���������� ���� �������
		}
		else if (str.at(0) == "update") { 
			updateFile(str.at(1)); // ���������� ������� ������ �� ����������
		}
		else if (str.at(0) == "delete") {
			deleteFileServer(str.at(1)); // ���������� ������ �� �������� ������
		}
		else { // ���� �������� �� ����������
			
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
int Connector::sendData(char_m& buffer) { // ������ ��� �������
	if (SOCKET_ERROR == (send(sock,buffer.chars,buffer.length,NULL))) {
		return WSAGetLastError();
	}
	return 0;
}
int Connector::sendData(const char* str, bool coding) { // �������� ����������� ���������
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
int Connector::recvData(char_m& buffer) { // ���� ������
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
int Connector::synchronized() { // ��������������
	filesSynchronized(); // �������������� � ������ ��������� �� �������
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
	return closesocket(sock); // ��������� �����
}
