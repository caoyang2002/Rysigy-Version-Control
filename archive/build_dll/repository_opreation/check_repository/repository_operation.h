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



#include <vector>  //����
//#include <queue>
#include <future> //�첽ִ��
//#include <unordered_set>
//#include <algorithm> // ���ͷ�ļ�



#include <map>


//ͳ�Ƴ����ʱ
#include <iostream>
#include <chrono>
//// ��ʼ��ʱ
//auto startTime = std::chrono::high_resolution_clock::now();
//
//// ������ִ����ĳ����߼�
//// ...
//
//// ������ʱ
//auto endTime = std::chrono::high_resolution_clock::now();
//
//// �����ʱ���Ժ���Ϊ��λ��
//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
//
//// �����ʱ
//std::cout << "����ִ�к�ʱ: " << duration << " ����" << std::endl;

namespace fs = std::filesystem;

// ��ѯ��ǰ��Ŀ¼���Ƿ���.Rysigy
class __declspec(dllexport) RepositoryOperation {
public:
	//���洢���Ƿ�����
    int CheckRepository();
	// �򿪴洢��
	int OpenRepository();
	// �½��洢��
	int CreateRepository();
	// ɾ���洢��
	int DeleteRepository();
	// ��ȡ�洢��ռ���ڴ��С
	int GetRepositorySize();


	//struct DirectoryNode {
	//	std::string name;
	//	std::vector<DirectoryNode> subdirectories;
	//	std::vector<std::string> files;
	//};
	//bool CheckDirectoryExist(const fs::path& basePath, const DirectoryNode& node);


    
};
