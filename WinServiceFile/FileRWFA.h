#pragma once
#include <string>
#include <fstream>
#include "vector"
class FileRWFA
{
private:
	bool connected;
	std::string catalog;
	std::wstring ConvertStringToWstring(const std::string& str);
	int lastInfoCF;
public:
	FileRWFA(const std::string& catalog); // � ������������ ���������� ������������� ��������
	std::vector<std::wstring> getCF(const std::string& path); //catalog + "\\" + path  ������
	std::vector<std::wstring> getCF(const std::wstring& path); //
	int getInfoCF();
	bool createCatalog(const std::string& path); //    ^ ��� �������� ����������� � ����
	bool createCatalog(const std::wstring& path); //   
	std::fstream openFile(const std::string& path, bool clear); // ^ ��������� ���� ������ � �������� ����
	std::fstream openFileT(const std::string& path,bool clear); // ^ ��������� ���� ������ � �������� ����
	std::fstream openFile(const std::wstring& path, bool clear); 
	std::fstream openFileT(const std::wstring& path, bool clear); 
	bool closeFile(std::fstream& file); // ��������� ���� 
	bool deleteFile(const std::string& path); // �������� �����
	
};

