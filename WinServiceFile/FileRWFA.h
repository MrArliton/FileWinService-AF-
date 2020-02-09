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
	FileRWFA(const std::string& catalog); // В конструкторе происходит инициализация католога
	std::vector<std::wstring> getCF(const std::string& path); //catalog + "\\" + path  Формат
	std::vector<std::wstring> getCF(const std::wstring& path); //
	int getInfoCF();
	bool createCatalog(const std::string& path); //    ^ имя католога указывается в пути
	bool createCatalog(const std::wstring& path); //   
	std::fstream openFile(const std::string& path, bool clear); // ^ Открывает файл всегда в бинарном виде
	std::fstream openFileT(const std::string& path,bool clear); // ^ Открывает файл всегда в тестовом виде
	std::fstream openFile(const std::wstring& path, bool clear); 
	std::fstream openFileT(const std::wstring& path, bool clear); 
	bool closeFile(std::fstream& file); // Закрывает файл 
	bool deleteFile(const std::string& path); // Удаление файла
	
};

