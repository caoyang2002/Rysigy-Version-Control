#pragma once
#include <iostream>
#include <filesystem>
#include <conio.h>
#include <string>

namespace fs = std::filesystem;

// 查询当前的目录下是否有.Rysigy
class __declspec(dllexport) Check {
public:
    int CheckRepository();//检查存储库是否完整
	int OpenRepository();// 打开存储库
	int CreateRepository();// 新建存储库
	int DeleteRepository();// 删除存储库


	//struct DirectoryNode {
	//	std::string name;
	//	std::vector<DirectoryNode> subdirectories;
	//	std::vector<std::string> files;
	//};
	//bool CheckDirectoryExist(const fs::path& basePath, const DirectoryNode& node);


    
};
