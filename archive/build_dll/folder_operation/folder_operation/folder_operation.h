#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <conio.h> // ��ȡ������˳�
#include <regex> //������ʽ

namespace fs = std::filesystem;
using namespace std;

class __declspec(dllexport) folder_operation{
public:
	// ��ѯ�ļ��а汾
	int queryFolderVersions(string); 	
	// �����ļ��а汾
	int createFolderVresions(string);
	// ���ļ��а汾
	int openFolderVersions(string);
	// ��������ļ���
	int ignoreThisFolder(string);	
	// ɾ�������ָ���ļ��а汾
	int deleteFolderVersions(string);
	// ɾ������ļ��е����а汾
	int deleteAllFolderVersions(string);
	// �ָ�ָ���汾���ļ�
};

