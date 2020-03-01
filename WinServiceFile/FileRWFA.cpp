#include "FileRWFA.h"
#include "windows.h"
//debug
#include <iostream>
using namespace std;

FileRWFA::FileRWFA(const string& catalog,const string& baseName) {
	this->catalog = catalog;
	FileRWFA::baseName = baseName;
	exc.push_back(ConvertStringToWstring(baseName));
}
// Две функции конвертации строк
std::string ConvertWStringToString(const std::wstring& wstr)
{
	std::string strTo;
	char* szTo = new char[wstr.length() + 1];
	szTo[wstr.size()] = '\0';
	WideCharToMultiByte(CP_ACP, NULL, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
	strTo = szTo;
	delete[] szTo;
	return strTo;
}
wstring FileRWFA::ConvertStringToWstring(const string& str)
{
	if (str.empty())
	{
		return wstring();
	}
	int num_chars = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, str.c_str(), str.length(), NULL, 0);
	wstring wstrTo;
	if (num_chars)
	{
		wstrTo.resize(num_chars);
		if (MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, str.c_str(), str.length(), &wstrTo[0], num_chars))
		{
			return wstrTo;
		}
	}
	return wstring();
}
void FileRWFA::compareFiles(const wstring& path, fstream& file) { // Цикличная функция 
	vector<wstring> start = getCF(path + L"\\*"); // Тут берём основные катологи и работаем с ними
	if (start.size() > 0) {
		for (int i = 0; i < start.size(); i++) {
			
			file << i << ":";
			file << "\"" << ConvertWStringToString(start.at(i)) << "\"=";
			if (!typeFile(start.at(i))) { // Если каталог
				compareFiles(path + start.at(i), file);
				file << ".";
			}
			else { //Если файл
				fstream bufferFile = openFile(start.at(i), false);
				bufferFile.seekg(0, ios_base::end);
				file << bufferFile.tellg() << ".";
				closeFile(bufferFile);
			}
		}
	}
	else {
		file << ".";
	}
}
int FileRWFA::typeFile(const wstring& file) { // Узнаем файл это или каталог
	wstring str = ConvertStringToWstring(catalog)+ L"\\" + file;
	WIN32_FIND_DATA data;
	FindFirstFile(str.c_str(), &data);
	if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		return 0;
	}
	else {
		return 1;
	}

}
void FileRWFA::synchronizeCatalogs(queue<vector<string>>& qCommands) { // qCommands - это очередь команд которые выполняет клиент общаясь с сервером
	fstream file = openFileT(baseName, false);
	vector<wstring> start = getCF("*"); // Тут берём основные катологи и работаем с ними
	if (start.size() > 0) {
		if (file) {
			file.seekg(0, ios_base::end);
			int len = file.tellg();
			if (len > 1) { // База создана производим сравнение
				// Если при сравннении база повреждена производим её пересоздание


			}
			else { // База не создана или удалена начинаем создание новой базы
				// А также сообщаем серверу о требуемой синхронизации
				for (int i = 0; i < start.size(); i++) {
					file << i << ":";
					file << "\"" << ConvertWStringToString(start.at(i)) << "\"=";
					if (!typeFile(start.at(i))) { // Если является каталогом
						compareFiles(start.at(i), file);
						file << ".";
					}
					else { // Если является файлом то открываем его и узнаем размер
						fstream bufferFile = openFile(start.at(i), false);
						bufferFile.seekg(0, ios_base::end);
						file << bufferFile.tellg() << ".";
						closeFile(bufferFile);
					}

				}
			}
		}
		else {
			errorFA("Can't open file");
		}
	}
	closeFile(file);
}
int FileRWFA::isExc(wstring str) {
	for (int i = 0; i < exc.size(); i++) {
		if (exc.at(i)._Equal(str))
			return 0;
	}
	return 1;
}
vector<wstring> FileRWFA::getCF(const string& path) { // Возвращает названия катологов
	WIN32_FIND_DATA data;
	wstring buff = ConvertStringToWstring(catalog + "\\" + path);
	HANDLE handle = FindFirstFile(buff.c_str(),&data);
	vector<wstring> catalogs;
	if(isExc(data.cFileName))
	catalogs.push_back(data.cFileName);
	lastInfoCF = 1;
	while (FindNextFile(handle, &data) != 0) {
		if (isExc(data.cFileName)) {
			catalogs.push_back(data.cFileName);
			lastInfoCF++;
		}
			
	}
	return catalogs;
}

vector<wstring> FileRWFA::getCF(const wstring& path) { // Возвращает названия катологов
	WIN32_FIND_DATA data;
	wstring buff = ConvertStringToWstring(catalog + "\\") + path;
	HANDLE handle = FindFirstFile(buff.c_str(), &data);
	vector<wstring> catalogs;
	if (isExc(data.cFileName))
	catalogs.push_back(data.cFileName);
	
	lastInfoCF = 1;
	while (FindNextFile(handle, &data) != 0) {
		if (isExc(data.cFileName))
		catalogs.push_back(data.cFileName);
		lastInfoCF++;
	}
	return catalogs;

}
int FileRWFA::getInfoCF() {
	return lastInfoCF;
}
bool FileRWFA::createCatalog(const string& path) {
	wstring s = ConvertStringToWstring(catalog+"\\"+path);
	return CreateDirectory(s.c_str(), NULL);
}
bool FileRWFA::createCatalog(const wstring& path) {
	wstring s = ConvertStringToWstring(catalog + "\\") + path;
	return CreateDirectory(s.c_str(), NULL);
}
std::fstream FileRWFA::openFile(const string& path,bool clear) {
	fstream file;
	if (!clear) {
		 file = fstream(catalog + "\\" + path, ios_base::in | ios_base::out | ios_base::binary | ios_base::ate);
		 if (!file) {
			 file = fstream(catalog + "\\" + path, ios_base::out |ios_base::binary | ios_base::ate);
		 }
	}
	else {
		file = fstream(catalog + "\\" + path, ios_base::in | ios_base::out | ios_base::binary | ios_base::trunc);
	}
	return file;
}

std::fstream FileRWFA::openFileT(const string& path, bool clear) {
	fstream file;
	if (!clear) {
		file = fstream(catalog+"\\"+path, ios_base::in | ios_base::out | ios_base::ate);
		if (!file) {
			file = fstream(catalog + "\\" + path, ios_base::out | ios_base::ate);
		}
	}
	else {
		file = fstream(catalog + "\\" + path, ios_base::in | ios_base::out | ios_base::trunc);
	}
	return file;
}
std::fstream FileRWFA::openFile(const wstring& path, bool clear) {
	fstream file;
	string buff = ConvertWStringToString(path);
	if (!clear) {
		file = fstream(catalog + "\\" + buff, ios_base::in | ios_base::out | ios_base::binary | ios_base::ate);
		if (!file) {
			file = fstream(catalog + "\\" + buff, ios_base::out |ios_base::binary|ios_base::ate);
		}
	}
	else {
		file = fstream(catalog + "\\" + buff, ios_base::in | ios_base::out | ios_base::binary | ios_base::trunc);
	}
	return file;
}

std::fstream FileRWFA::openFileT(const wstring& path, bool clear) {
	fstream file;
	string buff = ConvertWStringToString(path);
	if (!clear) {
		file = fstream(catalog + "\\" + buff, ios_base::in | ios_base::out | ios_base::ate);
		if (!file) {
			file = fstream(catalog + "\\" + buff, ios_base::out | ios_base::ate);
		}
	}
	else {
		file = fstream(catalog + "\\" + buff, ios_base::in | ios_base::out | ios_base::trunc);
	}
	return file;
}
bool FileRWFA::closeFile(std::fstream& file) {
	file.close();
	if(file.is_open())
	return false;
	return true;
}

bool FileRWFA::deleteFile(const string& path) {
	string buff = catalog + "\\" + path;
	if (remove(buff.c_str()) != 0) {
		return false;
	}
	else {
		return true;
	}
}
void FileRWFA::errorFA(const char* buffer) {
	cout << buffer;
}