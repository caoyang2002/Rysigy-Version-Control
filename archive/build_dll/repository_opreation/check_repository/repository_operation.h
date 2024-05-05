#pragma once
#include <iostream>
#include <filesystem>
#include <conio.h>
#include <string>
#include <fstream>

#include <iomanip>
#include <sstream>


#include <Windows.h>
//#include <Shlwapi.h>
#include <Shellapi.h>



#include <vector>  //向量
//#include <queue>
#include <future> //异步执行
//#include <unordered_set>
//#include <algorithm> // 添加头文件



#include <map>


//统计程序耗时
#include <iostream>
#include <chrono>
//// 开始计时
//auto startTime = std::chrono::high_resolution_clock::now();
//
//// 在这里执行你的程序逻辑
//// ...
//
//// 结束计时
//auto endTime = std::chrono::high_resolution_clock::now();
//
//// 计算耗时（以毫秒为单位）
//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
//
//// 输出耗时
//std::cout << "程序执行耗时: " << duration << " 毫秒" << std::endl;

namespace fs = std::filesystem;

// 查询当前的目录下是否有.Rysigy
class __declspec(dllexport) RepositoryOperation {
public:
	//检查存储库是否完整
    int CheckRepository();
	// 打开存储库
	int OpenRepository();
	// 新建存储库
	int CreateRepository();
	// 删除存储库
	int DeleteRepository();
	// 获取存储库占用内存大小
	int GetRepositorySize();


	//struct DirectoryNode {
	//	std::string name;
	//	std::vector<DirectoryNode> subdirectories;
	//	std::vector<std::string> files;
	//};
	//bool CheckDirectoryExist(const fs::path& basePath, const DirectoryNode& node);


    
};
