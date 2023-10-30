#pragma once
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <conio.h> // 获取输入后退出

#include <Windows.h>

#include <regex> //正则表达式
// 获取当前时间
#include <iostream>
#include <ctime>

#include <map>

#pragma comment(lib,"file_operation.lib")

namespace fs = std::filesystem;
using namespace std;

class __declspec(dllexport) file_operation{
public:
	//int QueryFileVersions(string); // 查询文件版本
	//int CreateFileVersions(string); // 创建文件版本

	//获取指定的文件版本
	int GetSpecilVerofFile(fs::path);

	//int DeleteAllFileVersions(string); // 删除所有文件版本
	//int IgnoreThisFile(string); // 忽略此文件

};

