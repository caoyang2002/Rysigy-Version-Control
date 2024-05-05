#pragma once
#include <iostream>
#include <string>

#include "SHA256.h"
#include <filesystem>


// ��ȡ�ļ���Ϣ
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>


namespace fs = std::filesystem;
using namespace std;

class __declspec(dllexport) get_info{ //���ֻ��dll, ��ô�Ͱ���һ�����
public:
	//��ȡ�ļ�hash
	string getFileHash(fs::path pathName);
	// ��ȡ�ļ���С
	std::wstring GetFileSize(const std::wstring& filename);

	std::wstring GetFileOccupiedSpace(const std::wstring& filename);
	std::wstring GetFileCreationTime(const std::wstring& filename);
	std::wstring GetFileModificationTime(const std::wstring& filename);
	std::wstring GetFileAccessTime(const std::wstring& filename);
	std::wstring GetFileEncryptionInfo(const std::wstring& filename);





};

