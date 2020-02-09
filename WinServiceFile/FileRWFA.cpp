#include "FileRWFA.h"
#include "windows.h"
using namespace std;

FileRWFA::FileRWFA(const string& catalog) {
	this->catalog = catalog;
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
vector<wstring> FileRWFA::getCF(const string& path) { // Возвращает названия катологов
	WIN32_FIND_DATA data;
	wstring buff = ConvertStringToWstring(catalog + "\\" + path);
	HANDLE handle = FindFirstFile(buff.c_str(),&data);
	vector<wstring> catalogs;
	catalogs.push_back(data.cFileName);
	lastInfoCF = 1;
	while (FindNextFile(handle, &data) != 0) {
		catalogs.push_back(data.cFileName);
		lastInfoCF++;
	}
	return catalogs;
}
vector<wstring> FileRWFA::getCF(const wstring& path) { // Возвращает названия катологов
	WIN32_FIND_DATA data;
	wstring buff = ConvertStringToWstring(catalog + "\\") + path;
	HANDLE handle = FindFirstFile(buff.c_str(), &data);
	vector<wstring> catalogs;
	catalogs.push_back(data.cFileName);
	lastInfoCF = 1;
	while (FindNextFile(handle, &data) != 0) {
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
		 file = fstream(catalog + "\\" + path, ios_base::in | ios_base::out | ios_base::binary);
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