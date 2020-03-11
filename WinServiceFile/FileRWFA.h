#pragma once
#include <string>
#include <fstream>
#include "vector"
#include "queue"
typedef struct base_s { // ���������� ��� ������ ���� ������
	std::string name;
	int catalog;
	int number;
	size_t bytes; // ������������ ������ ���� ��� ����
}base_s;
class FileRWFA
{

private:
	bool connected;
	std::string catalog;
	std::string baseName;
	std::wstring ConvertStringToWstring(const std::string& str);
	int lastInfoCF;
	std::vector<std::string> Split(const std::string& line, char del);
	//
	void compareFiles(const std::wstring& path,std::fstream& file); // ��� ��������� ���� �� �������� 
	void compareFC(const std::string& path,std::fstream& file, std::queue<std::vector<std::string>>& qCommands); // ��� ���������� ���������� ���������
	base_s readBase(std::fstream& read); // ������ ���� ������
	//
	void errorFA(const char* error);
	int typeFile(const std::wstring& name);
	int isExc(std::wstring str);
	//
	int isEqualWStrMas(std::wstring str,std::vector<std::wstring> vector);
	int isEqualStrMas(std::string str, std::vector<std::wstring> vector);
	std::vector<std::wstring> exc = { L".",L".." }; // ���������� ��� getCF
public:
	bool execCommands(std::queue<std::vector<std::string>> qCommands); // �������� ��������� ���� ������ ������������ �������
	//
	int addBaseFile(const std::string& path); // ��� ��������� ���� � ����
	int deleteBaseFile(const std::string& path); // ��� ������� ���� �� ����
	int updateBytesFile(const std::string& path); // ��� ��������� �������� ������ �����
	//
	FileRWFA(const std::string& catalog,const std::string& baseName); // � ������������ ���������� ������������� ��������
	std::vector<std::wstring> getCF(const std::string& path); //catalog + "\\" + path  ������
	std::vector<std::wstring> getCF(const std::wstring& path); //
	int getInfoCF();
	//
	void synchronizeCatalogs(std::queue<std::vector<std::string>>& qCommands); // �������������� �������� � ����� ������ 
	bool createCatalog(const std::string& path); //    ^ ��� �������� ����������� � ����
	bool createCatalog(const std::wstring& path); //   
	//
	std::fstream openFile(const std::string& path, bool clear); // ^ ��������� ���� ������ � �������� ����
	std::fstream openFileT(const std::string& path,bool clear); // ^ ��������� ���� ������ � �������� ����
	std::fstream openFile(const std::wstring& path, bool clear); 
	std::fstream openFileT(const std::wstring& path, bool clear); 
	//
	bool closeFile(std::fstream& file); // ��������� ���� 
	bool deleteFile(const std::string& path); // �������� �����

	// private (debug)
	
};

