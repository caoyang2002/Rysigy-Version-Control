#pragma once
#include <iostream>
#include <filesystem>
#include <conio.h>
#include <string>

namespace fs = std::filesystem;

// ��ѯ��ǰ��Ŀ¼���Ƿ���.Rysigy
class __declspec(dllexport) Check {
public:
    int CheckRepository();//���洢���Ƿ�����
	int OpenRepository();// �򿪴洢��
	int CreateRepository();// �½��洢��
	int DeleteRepository();// ɾ���洢��


	//struct DirectoryNode {
	//	std::string name;
	//	std::vector<DirectoryNode> subdirectories;
	//	std::vector<std::string> files;
	//};
	//bool CheckDirectoryExist(const fs::path& basePath, const DirectoryNode& node);


    
};
