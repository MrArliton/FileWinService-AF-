#pragma once
#include <string>
#include <fstream>
#include "vector"
#include "queue"
class FileRWFA
{
private:
	bool connected;
	std::string catalog;
	std::string baseName;
	std::wstring ConvertStringToWstring(const std::string& str);
	int lastInfoCF;
	void compareFiles(const std::wstring& path,std::fstream& file); // ��� ��������� ���� �� �������� 
	void errorFA(const char* error);
	int typeFile(const std::wstring& name);
	int isExc(std::wstring str);
	std::vector<std::wstring> exc = { L".",L".." }; // ���������� ��� getCF
public:
	FileRWFA(const std::string& catalog,const std::string& baseName); // � ������������ ���������� ������������� ��������
	std::vector<std::wstring> getCF(const std::string& path); //catalog + "\\" + path  ������
	std::vector<std::wstring> getCF(const std::wstring& path); //
	int getInfoCF();
	void synchronizeCatalogs(std::queue<std::vector<std::string>>& qCommands); // �������������� �������� � ����� ������ 
	bool createCatalog(const std::string& path); //    ^ ��� �������� ����������� � ����
	bool createCatalog(const std::wstring& path); //   
	std::fstream openFile(const std::string& path, bool clear); // ^ ��������� ���� ������ � �������� ����
	std::fstream openFileT(const std::string& path,bool clear); // ^ ��������� ���� ������ � �������� ����
	std::fstream openFile(const std::wstring& path, bool clear); 
	std::fstream openFileT(const std::wstring& path, bool clear); 
	bool closeFile(std::fstream& file); // ��������� ���� 
	bool deleteFile(const std::string& path); // �������� �����
	
};

