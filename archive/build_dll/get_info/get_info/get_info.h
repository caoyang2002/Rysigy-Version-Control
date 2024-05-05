#pragma once
#include <iostream>
#include <string>

#include "SHA256.h"
#include <filesystem>


// 获取文件信息
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>


namespace fs = std::filesystem;
using namespace std;

class __declspec(dllexport) get_info{ //如果只有dll, 那么就把这一句加上
public:
	//获取文件hash
	string getFileHash(fs::path pathName);
	// 获取文件大小
	std::wstring GetFileSize(const std::wstring& filename);

	std::wstring GetFileOccupiedSpace(const std::wstring& filename);
	std::wstring GetFileCreationTime(const std::wstring& filename);
	std::wstring GetFileModificationTime(const std::wstring& filename);
	std::wstring GetFileAccessTime(const std::wstring& filename);
	std::wstring GetFileEncryptionInfo(const std::wstring& filename);





};

