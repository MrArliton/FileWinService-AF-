#include "Connector.h"
//debuf include
#include <iostream>
using namespace std;
Connector::Connector(const string& ip, int port) { // ����������� ��������
	// �������� ������
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
	disconnect();
}
//�������� ����������
int Connector::codingBytes(char* bytes) {
	return 0;
}
int Connector::uncodingBytes(char* bytes) {
	return 0;
}
int Connector::sendData(char_m& buffer) { // ������ ��� �������
	if (SOCKET_ERROR == (send(sock,buffer.chars,buffer.length,NULL))) {
		return WSAGetLastError();
	}
	return 0;
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