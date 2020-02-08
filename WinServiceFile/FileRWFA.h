#pragma once
#include <string>
#include <fstream>
class FileRWFA
{
private:
	
public:
	FileRWFA(std::string catalog); // � ������������ ���������� ������������� ��������
	std::string getCF(std::string path); //catalog + "\\" + path  ������
	bool createCatalog(std::string path); //    ^ ��� �������� ����������� � ����
	std::fstream openFile(std::string path); // ^ ��������� ���� ������ � �������� ����
	bool closeFile(std::fstream file); // ��������� ���� 
	bool deleteFile(std::string path); // �������� �����

	
};

