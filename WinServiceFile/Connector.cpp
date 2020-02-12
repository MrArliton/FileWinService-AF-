#include "Connector.h"
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
	if (SOCKET_ERROR == (connect(sock,(sockaddr*)&addr,sizeof(addr)))) { // ���������� ������
		return WSAGetLastError(); // ���������� ������
	}
	else {
		return 0;
	}
	// ����� ����������� ����� ���������� ����������� ��������� finishConnect
}
int Connector::sendData(char* buffer) { // ������ ��� �������
	if (SOCKET_ERROR == (send(sock,buffer,sizeof(buffer),NULL))) {
		return WSAGetLastError();
	}
	return 0;
}
int Connector::recvData(char* buffer) { // ���� ������
	int actual_len = 0;
	if (SOCKET_ERROR == (actual_len = recv(sock,buffer,sizeof(buffer),0))) {
		return WSAGetLastError();
	}
}
int Connector::synchronized() { // ��������������
	filesSynchronized(); // �������������� � ������ ��������� �� �������
}
void Connector::filesSynchronized() { 
	
}
int Connector::disconnect() { 
	return closesocket(sock); // ��������� �����
}