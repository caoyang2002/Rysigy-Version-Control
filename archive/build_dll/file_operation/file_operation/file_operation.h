#pragma once
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <conio.h> // ��ȡ������˳�

#include <regex> //������ʽ

// ��ȡ��ǰʱ��
#include <iostream>
#include <ctime>

#include <map>

namespace fs = std::filesystem;
using namespace std;

class __declspec(dllexport) file_operation{
public:
	int QueryFileVersions(string); // ��ѯ�ļ��汾
	int CreateFileVersions(string); // �����ļ��汾
	//��ȡ�ļ�ָ���汾
	int GetSpecilVerofFile(fs::path);

	int DeleteAllFileVersions(string); // ɾ�������ļ��汾
	
	int IgnoreThisFile(string); // ���Դ��ļ�

};

