#include "pch.h"
#include "repository_operation.h"


namespace fs = std::filesystem;
using namespace std;



// ��ȡ��ǰʱ�� , �����ַ���: ��-��-�� ʱ:��:��
std::string getCurrentTime() {
	std::time_t currentTime = std::time(nullptr);
	std::tm localTime;
	localtime_s(&localTime, &currentTime);

	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
	//std::cout << "Formatted time: " << buffer << "\t";

	return std::string(buffer);
}

// ����̨�����ɫ
enum Color {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	PURPLE = 5,
	YELLOW = 6,
	WHITE = 7,
	GRAY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_PURPLE = 13,
	LIGHT_YELLOW = 14,
	BRIGHT_WHITE = 15
};


void COLOR_PRINT(const char* s, Color color)
{
	std::map<Color, int> colorKV;
	colorKV[BLACK] = 0;
	colorKV[BLUE] = 1;
	colorKV[GREEN] = 2;
	colorKV[CYAN] = 3;
	colorKV[RED] = 4;
	colorKV[PURPLE] = 5;
	colorKV[YELLOW] = 6;
	colorKV[WHITE] = 7;
	colorKV[GRAY] = 8;
	colorKV[LIGHT_BLUE] = 9;
	colorKV[LIGHT_GREEN] = 10;
	colorKV[LIGHT_CYAN] = 11;
	colorKV[LIGHT_RED] = 12;
	colorKV[LIGHT_PURPLE] = 13;
	colorKV[LIGHT_YELLOW] = 14;
	colorKV[BRIGHT_WHITE] = 15;

	int colorNum;
	auto it = colorKV.find(color);
	if (it != colorKV.end()) {
		colorNum = it->second;
		//	std::cout << "��ɫ��Ӧ��ֵΪ��" << colorNum << std::endl;
	}
	else {
		//	std::cout << "�Ҳ�����Ӧ����ɫֵ" << std::endl;
	}
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | colorNum);
	printf(s);
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
}



// �洢�����ָ������� ϵͳ��־ �� ϵͳ�ڵ��û���־


// ��ʽ�����������̨
// [����] ������, �������Դ, 4(��ɫ)
int FOut(std::string information, std::string details_path, Color color) {

	// ����̨���
	string logCurrent = information + "\t" + details_path;
	COLOR_PRINT(logCurrent.c_str(), color);

	return 0;
}

// �Զ����Ҵ洢�� �����ϵͳ��־�ļ�, �����ڿ���̨��ʾ, ��Ҫ���뵱ǰ·��
// ([��Ϣ] ��Ϣ���, ��Ϣ����Դ��˵��, ��ǰ·��)
int FOuttoSysLog(std::string information, std::string details_path) {
	//cout << information << "\t" << details_path << endl;
	//cout << "[��ʼ] ϵͳ��־��¼" << endl;

	fs::path systemLogFilePath = fs::current_path() / ".Rysigy" / "DATABASE" / "system_log.ryg";

	//cout << "[��־] ϵͳ��־�ļ�·��: " << systemLogFilePath << endl;
	

	// �����־�ļ��Ƿ����
	if (!fs::exists(systemLogFilePath)) {
		COLOR_PRINT("[����] ��־�ļ�������, ��������־�ļ�\n", RED);
	}

	std::ofstream outputLogFile;
	// ׷��(����) �ļ�����
	outputLogFile.open(systemLogFilePath, std::ios::out | std::ios::app); // ��λ��(��true��true)    std::ios::out �Ķ������� 1   std::ios::app �Ķ������� 100 , ��λ������� ,���ǵĶ������� 101 ��д���׷��ģʽ
	if (outputLogFile.is_open()) {
		string currentTime = getCurrentTime(); // ��ȡ��ǰʱ�� 
		string logContent = currentTime + "\t" + information + "\t" + details_path + "\t\n";
		outputLogFile << logContent << endl;
		outputLogFile.close();
		//cout << "[�ɹ�] ��־�ѳɹ�д���ļ���" << logContent;
	}
	else {
		std::cerr << "[����] �޷�����־�ļ���" << systemLogFilePath << std::endl;
	}

//	cout << "[����] ��־��¼" << endl;
	return 0;
}

// ����־�ļ�������Ԫ��Ϣ
int enterMetaInfotoLogFile(fs::path pathName) {
	FOut("[ִ��] ���������Ϣ���ļ���\n", "", WHITE);

	// ��� hash ���ļ���
	std::ofstream outputLogFile;

	// ׷��(����) �ļ�����
	//outputLogFile.open(systemLogFilePath, std::ios::out | std::ios::app); // ��λ��(��true��true)    std::ios::out �Ķ������� 1   std::ios::app �Ķ������� 100 , ��λ������� ,���ǵĶ������� 101 ��д���׷��ģʽ
	//if (outputLogFile.is_open()) {
	//	string currentTime = getCurrentTime(); // ��ȡ��ǰʱ�� 
	//	string logContent = currentTime + "\t" + information + "\t" + details_path + "\t\n";
	//	outputLogFile << logContent << endl;
	//	outputLogFile.close();
	//	cout << "[�ɹ�] ��־�ѳɹ�д���ļ���" << endl;
	//}
	//else {
	//	std::cerr << "[����] �޷�����־�ļ���" << systemLogFilePath << std::endl;
	//}

}







// ��־�ļ�:
// 
// 1. �û�ִ�м�¼��־ (����̨�����������־)  -- .Rysigy/VERSIONS/fileane/_Vx/filename/history.log
// 
// 2. �û�������־ (ֻ��¼�û������Ĵ�����־)  -- .Rysigy/DATABASE/data/user_log.ryg
// 
// 3. ϵͳ��־ (��¼ϵͳ�Ĳ�����־)          -- .Rysigy/DATABASE/metadata/rysigy_log.ryg





//����Ŀ¼�ڵ�

struct DirectoryNode {
	std::string name;
	std::vector<DirectoryNode> subdirectories;
	std::vector<std::string> files;
};

// �ݹ���洢��Ŀ¼�ṹ�Ƿ�����
bool CheckDirectoryExist(const fs::path& basePath, const DirectoryNode& node) {
	fs::path currentPath = basePath / node.name;

	// ��鵱ǰĿ¼�Ƿ����
	if (!fs::exists(currentPath)) {
		cout << "Ŀ¼������" << currentPath << endl;
		//std::cout << "Ŀ¼�����ڣ�" << currentPath << std::endl;
		return false;
	}
	else {
		cout << "Ŀ¼����" << currentPath << endl;
	}

	// ����Ƿ����������ļ�
	for (const std::string& file : node.files) {
		fs::path filePath = currentPath / file;
		if (!fs::exists(filePath)) {
			cout << "�ļ�������" << filePath << endl;
			//std::cout << "�ļ������ڣ�" << filePath << std::endl;
			return false;
		}
		else
		{
			cout << "�ļ�����" << filePath << endl;
		}
	}

	// �ݹ�����Ŀ¼
	for (const DirectoryNode& subdir : node.subdirectories) {
		if (!CheckDirectoryExist(currentPath, subdir)) {
			return false;
		}
	}

	// ������м��ͨ�����򷵻� true
	return true;
};

// ---------------------------------------------------------------------------------------------------------------------------------------------------------
//���洢���Ƿ����������
int RepositoryOperation::CheckRepository() {
	cout << "[ִ��] ���洢���Ƿ�����..." << endl;
	std::string repoName = ".Rysigy";
	cout << "[��Ϣ] �洢���������: " << repoName << endl;
	fs::path currentPath = fs::current_path();
	cout << "[��Ϣ] ��ǰ·����: " << currentPath << endl;
	fs::path repoPath = currentPath / repoName;
	cout << "[��Ϣ] �洢���·����: " << repoPath << endl;

	DirectoryNode customDirStructure;
	customDirStructure.name = repoPath.string();  // ���û���Ŀ¼������
	customDirStructure.subdirectories = {   //��ʽ {    {  "Ŀ¼һ", {{"Ŀ¼һ/��Ŀ¼һ", "Ŀ¼һ/��Ŀ¼��"},{"Ŀ¼һ/��Ŀ¼��"}}  },   {"Ŀ¼��",{Ŀ¼��}}    }
		// {{"1" ,{"1.1", "1.2"}},  {2,{{"2.1",{"2.1.1", "2.1.2"},{"2.2",{2.2.1}}}, {1.2},{1.3}}},{2,{2.1}},{3}}
		// ʹ�� {} �洢�ļ�/Ŀ¼��, ��Ҫʹ�� "" ���,
		{ "DATABASE" }, { "VERSIONS" }  // ��Ŀ¼ VERSIONS Ϊ��
	};

	if (fs::exists(repoPath) && fs::is_directory(repoPath)) {
		cout << "[����] �洢��λ��: " << repoPath << endl;
		//fs::path basePath = fs::current_path();  // ���û���·��
		if (CheckDirectoryExist(repoPath, customDirStructure)) { //���洢���е����Ŀ¼�Ƿ����
			cout << "[���] �洢�������� " << repoPath << endl;
			FOut("[�ɹ�] �洢������ļ����� " , repoPath.string(), GREEN);
			cout << "\n";
		}
	}
	else {
		FOut("[����] �洢�� '.Rysigy' �ļ��в�����","���ڿհ״��������Ҽ�ѡ��'�����洢��'\n",RED);
	}
	cout << "������ⰴ���˳�..." << std::endl;
	[[maybe_unused]] int waiting = _getch(); // �ȴ��û�������������˳����� ���Է���ֵ
	return 0;
}

// -----------------------------------�򿪴洢��
int RepositoryOperation::OpenRepository() {
	FOuttoSysLog("[ִ��] �򿪴洢��", "");
	// ��ȡ��ǰ·��
	fs::path currentPath = fs::current_path();
	// ƴ���ļ���·��
	std::wstring filePath = currentPath.wstring() + L"\\.Rysigy";
	wcout << L"�򿪵�·����: " << filePath << endl;
	// �жϴ洢���Ƿ����
	if (!fs::exists(filePath)) {
		// �ļ��в����ڣ����������Ϣ
		cout << "[����] �ļ�������" << endl;
		cout << "������ⰴ���˳�..." << std::endl;
		[[maybe_unused]] int waiting = _getch(); // �ȴ��û�������������˳����� ���Է���ֵ
		return 1;
	}
	HINSTANCE openFileResult = ShellExecuteW(NULL, L"open", filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
	if ((long)openFileResult <= 32) {
		cout << "[ʧ��] �ļ��޷���" << endl;
	}

	cout << "[�ɹ�] �ļ��Ѵ�\n";
	wcout << L"[�ɹ�] �ļ��Ѵ�" << filePath << endl;
	cout << "������ⰴ���˳�..." << endl;
	[[maybe_unused]] int waiting = _getch(); // �ȴ��û�������������˳����� ���Է���ֵ
	return 0;
}

// �����洢��
int RepositoryOperation::CreateRepository() {
	fs::path repoPath = fs::current_path() / ".Rysigy";
	if (fs::exists(repoPath)) {
		std::cout << "[��ʾ] �洢���Ѵ���\a" << std::endl;
		std::cout << "������ⰴ���˳�..." << std::endl;
		[[maybe_unused]] int waiting = _getch();

		return 0;
	}

	if (fs::create_directories(repoPath)) {
		cout << "[�ɹ�] �洢�ⴴ�����" << endl;
		std::string cmd = "attrib +h " + repoPath.string(); // ��·������˫����
		int folderHidden = system(cmd.c_str()); // CMD����
		if (folderHidden != 0) {
			cout << "[��ʾ] �޷����洢������Ϊ��������" << endl;
		}
		else {
			cout << "[�ɹ�] �ѽ��洢������Ϊ��������" << endl;
		}
	}
	else {
		std::cout << "[ʧ��] ����ʧ��: " << repoPath << std::endl;
		std::cout << "������ⰴ���˳�..." << std::endl;
		[[maybe_unused]] int waiting = _getch();
		return 1;
	}

	fs::path databasePath = repoPath / "DATABASE";
	fs::path versionsPath = repoPath / "VERSIONS";

	if (!fs::create_directories(databasePath) || !fs::create_directories(versionsPath)) {
		std::cout << "[ʧ��] ����ʧ��: " << versionsPath << std::endl;
		return 1;
	}
	else
	{
		cout << "[�ɹ�] ���ݿ�Ŀ¼�������" << endl;
	}

	fs::path systemLog = databasePath / "system_log.ryg";
	std::ofstream file(systemLog);
	if (!file.is_open()) {
		std::cout << "[ʧ��] ��־�ļ�����ʧ��" << std::endl;
		return 1;
	}
	else
	{
		cout << "[�ɹ�] ��־�ļ��������" << endl;
	}

	//file << "Hello, world!" << std::endl;
	file.close();
	//std::cout << "[�ɹ�] ��־�ļ������ɹ�" << std::endl;
	std::cout << "[�ɹ�] �洢��ṹ�������" << std::endl;
	FOuttoSysLog("[����] �����洢��", fs::current_path().string() + "/.Rysigy");

	std::cout << "������ⰴ���˳�..." << std::endl;
	[[maybe_unused]] int waiting = _getch();

	return 0;
}

// ʹ�ö��̺߳��첽��̷�ʽɾ���ļ��м������ݵĺ���
void deleteDirectoryMulThread(const fs::path& dirPath, int numThreads = 4)
{
	// ��ʼ��ʱ
	auto startTime = std::chrono::high_resolution_clock::now();

	//std::vector<std::future<void>> futuresToDelete;
	//std::vector<fs::directory_entry> entriesToDelete;

	//for (const auto& entry : fs::recursive_directory_iterator(dirPath))
	//{
	//	entriesToDelete.push_back(entry);
	//}

	//auto asyncDeleteFunc = [](const fs::path& path) -> void {
	//	try {
	//		if (fs::is_directory(path)) {
	//			fs::remove_all(path);
	//		}
	//		else {
	//			fs::remove(path);
	//		}
	//	}
	//	catch (const std::exception& e) {
	//		std::cerr << "Error deleting " << path << ": " << e.what() << std::endl;
	//	}
	//	};

	//int batchSize = (entriesToDelete.size() + numThreads - 1) / numThreads;

	//for (int i = 0; i < numThreads; ++i)
	//{
	//	int startIndex = i * batchSize;
	//	int endIndex = min(startIndex + batchSize, static_cast<int>(entriesToDelete.size()));

	//	for (int j = startIndex; j < endIndex; ++j) {
	//		const auto& entry = entriesToDelete[j];
	//		const auto& path = entry.path();
	//		auto future = std::async(std::launch::async, asyncDeleteFunc, path);
	//		futuresToDelete.emplace_back(std::move(future));
	//	}
	//}

	//for (auto& future : futuresToDelete)
	//{
	//	future.wait();
	//}

	/*std::vector<std::future<void>> futuresToDelete;
	std::vector<fs::directory_entry> entriesToDelete;

	for (const auto& entry : fs::recursive_directory_iterator(dirPath))
	{
		entriesToDelete.push_back(entry);
	}

	auto asyncDeleteFunc = [](const fs::path& path) -> void {
		try {
			if (fs::exists(path)) {
				if (fs::is_directory(path)) {
					fs::remove_all(path);
				}
				else {
					fs::remove(path);
				}
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Error deleting " << path << ": " << e.what() << std::endl;
		}
		};

	int batchSize = (entriesToDelete.size() + numThreads - 1) / numThreads;

	for (int i = 0; i < numThreads; ++i)
	{
		int startIndex = i * batchSize;
		int endIndex = min(startIndex + batchSize, static_cast<int>(entriesToDelete.size()));

		for (int j = startIndex; j < endIndex; ++j) {
			const auto& entry = entriesToDelete[j];
			const auto& path = entry.path();

			if (fs::exists(path)) {
				auto future = std::async(std::launch::async, asyncDeleteFunc, path);
				futuresToDelete.emplace_back(std::move(future));
			}
		}
	}

	for (auto& future : futuresToDelete)
	{
		future.wait();
	}*/












	std::vector<std::future<void>> futuresToDelete;
	std::vector<fs::directory_entry> entriesToDelete;

	fs::path repoPath = dirPath;

	for (const auto& entry : fs::recursive_directory_iterator(dirPath))
	{
		entriesToDelete.push_back(entry);
	}

	auto asyncDeleteFunc = [](const fs::path& path) -> void {
		if (fs::exists(path)) {
			try {
				if (fs::is_directory(path)) {
					cout << "[ɾ��] Ŀ¼: " << path << endl;
					fs::remove_all(path);
				}
				else {
					cout << "[ɾ��] �ļ�: " << path << endl;
					fs::remove(path);
				}
			}
			catch (const std::exception& e) {
				throw std::runtime_error("ɾ�� " + path.string() + " ʧ��: " + e.what());
			}
		}
		else {
			throw std::runtime_error("�ļ���Ŀ¼ " + path.string() + " ������");
		}
	};

	int batchSize = (entriesToDelete.size() + numThreads - 1) / numThreads;

	for (int i = 0; i < numThreads; ++i)
	{
		int startIndex = i * batchSize;
		int endIndex = min(startIndex + batchSize, static_cast<int>(entriesToDelete.size()));

		for (int j = startIndex; j < endIndex; ++j) {
			const auto& entry = entriesToDelete[j];
			const auto& path = entry.path();

			if (fs::exists(path)) {
				auto future = std::async(std::launch::async, asyncDeleteFunc, path);
				futuresToDelete.emplace_back(std::move(future));
			}
		}
	}

	// �ȴ������첽ɾ���������
	for (auto& future : futuresToDelete)
	{
		future.wait();
	}

	// �ȴ�������ɺ�ſɷ����ѱ�ɾ�����ļ���Ŀ¼
	// ...

	// ���ɾ����Ŀ¼
	if (fs::exists(repoPath)) {
		fs::remove(repoPath);
	}


	// ������ʱ
	auto endTime = std::chrono::high_resolution_clock::now();
	// �����ʱ���Ժ���Ϊ��λ��
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
	// �����ʱ
	std::cout << "����ִ�к�ʱ: " << duration << " ����" << std::endl;
}

//ɾ���洢��-------------------------------------------
void deleteDirectory(const fs::path& dirPath) {
	//if (dirPath.empty()){
	//	return;
	//}
	//HANDLE hFind;
	//WIN32_FIND_DATAA findData;
	//string strTempDir = dirPath.string() + "\\*";
	//hFind = FindFirstFileA(strTempDir.c_str(), &findData);
	//if (hFind == INVALID_HANDLE_VALUE){
	//	return;
	//}
	//fs::path prevPath; // ��¼��һ��·��,
	//do{
	//	// ����"."��".."�������
	//	if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0){
	//		continue;
	//	}
	//	string strFileName;
	//	strFileName.assign(dirPath.string());
	//	strFileName.append("\\");
	//	strFileName.append(findData.cFileName);
	//	if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {// �Ƿ���Ŀ¼
	//		if (strFileName != prevPath) {
	//			fs::remove(prevPath);
	//		}
	//		cout << "[�ݹ�] Ŀ¼: " << strFileName << endl;
	//		deleteDirectory(strFileName.c_str());
	//		prevPath = strFileName;
	//	}else{
	//		cout << "[ɾ��] �ļ�: " << strFileName << endl;
	//		DeleteFileA(strFileName.c_str());
	//	}
	//} while (FindNextFileA(hFind, &findData));
	//FindClose(hFind);
	//LPCSTR lpPath = dirPath.string().c_str();
	//RemoveDirectoryA(lpPath);
	//
	//

// ����ϵͳ API ɾ��
//string  command = "rd /s/q " + dirPath.string();
//system(command.c_str());//����windows apiɾ��ָ���ļ���

// �޷�ɾ�����ļ����ļ���
//if (!fs::exists(dirPath) || !fs::is_directory(dirPath))
//{
//	return;
//}

//std::vector<fs::path> pathsToDelete; // ��ɾ����·������

//// �����ѷ��ʵ�·��
//std::unordered_set <fs::path> visitedPaths;

//std::queue<fs::path> pathQueue;
//pathQueue.push(dirPath);

//while (!pathQueue.empty())
//{
//	const auto currentPath = pathQueue.front();
//	pathQueue.pop();

//	visitedPaths.insert(currentPath);

//	try
//	{
//		for (const auto& entry : fs::directory_iterator(currentPath))
//		{
//			const auto& entryPath = entry.path();

//			if (fs::is_directory(entryPath))
//			{
//				if (visitedPaths.count(entryPath) == 0) // �����ظ�����
//				{
//					pathQueue.push(entryPath);
//					visitedPaths.insert(entryPath);
//				}
//			}

//			pathsToDelete.push_back(entryPath);
//		}
//	}
//	catch (const std::exception& ex)
//	{
//		// �����쳣���������������Ϣ
//		std::cerr << "Error occurred during directory traversal: " << ex.what() << std::endl;
//	}
//}

//// ����ɾ��·��
//std::vector<std::future<void>> futures;
//const size_t batchSize = 50; // ÿ���εĴ�С
//size_t startIndex = 0;

//while (startIndex < pathsToDelete.size())
//{
//	const size_t endIndex = min(startIndex + batchSize, pathsToDelete.size());

//	futures.emplace_back(std::async(std::launch::async, [&pathsToDelete, startIndex, endIndex]() {
//		for (size_t i = startIndex; i < endIndex; ++i)
//		{
//			try
//			{
//				if (fs::is_directory(pathsToDelete[i]))
//				{
//					fs::remove(pathsToDelete[i]);
//				}
//				else if (fs::is_regular_file(pathsToDelete[i]))
//				{
//					fs::remove(pathsToDelete[i]);
//				}
//			}
//			catch (const std::exception& ex)
//			{
//				// �����쳣���������������Ϣ
//				std::cerr << "Error occurred during deletion: " << ex.what() << std::endl;
//			}
//		}
//		}));

//	startIndex = endIndex;
//}

//// �ȴ�����ɾ���������
//for (auto& future : futures)
//{
//	future.wait();
//}

// ����
	if (!fs::exists(dirPath) || !fs::is_directory(dirPath))
	{
		return;
	}

	for (const auto& entry : fs::directory_iterator(dirPath))
	{
		const auto& currentPath = entry.path();

		if (fs::is_directory(currentPath))
		{
			// �ݹ�ɾ�����ļ��е�����
			std::cout << "[����] ��Ŀ¼: " << currentPath << std::endl;
			deleteDirectory(currentPath);
		}
		else if (fs::is_regular_file(currentPath))
		{
			// ɾ���ļ�
			std::cout << "[ɾ��] �ļ�: " << currentPath << std::endl;
			fs::remove(currentPath);
		}
	}

	// ɾ���ļ���
	if (fs::is_empty(dirPath))
	{
		std::cout << "[ɾ��] Ŀ¼" << dirPath << std::endl;
		fs::remove(dirPath);
	}
	else
	{
		std::cout << "[ɾ��] Ŀ¼��������" << dirPath << std::endl;
		fs::remove_all(dirPath);
	}
}

//
int RepositoryOperation::DeleteRepository() {
	string confirm;
	FOut("[��Ҫ] ɾ��֮ǰ�����Ƿ���δ�رյ��ļ�", "���δ�ر�, ��ɾ�������д˳����쳣�˳� !\n", RED);
	cout << "��ȷ��Ҫɾ����? \n���� yes ȷ��ɾ��, ���������ַ�ȡ��ɾ��...\n>>>";
	cin >> confirm;
	fs::path currentPath = fs::current_path();//��ȡ��ǰ·��
	fs::path directoryPath = currentPath / ".Rysigy";

	if (confirm == "yes" || confirm == "Yes" || confirm == "YES") {
		if (fs::exists(directoryPath)) {
			if (fs::is_directory(directoryPath)) {
				deleteDirectoryMulThread(directoryPath, 4); //���߳�ɾ���ļ�
				std::cout << "[�ɹ�] �洢����ɾ��" << std::endl;
			}
			else {
				std::cout << "[��ʾ] ��鵽һ����洢��ͬ�����ļ�" << std::endl;
			}
		}
		else {
			cout << "[����] �洢�ⲻ����\n";
		}
	}
	else
	{
		cout << "[ȡ��] ��ȡ��ɾ��\n";
	}
	cout << "������ⰴ���˳�..." << std::endl;
	[[maybe_unused]] int waiting = _getch(); // �ȴ��û�������������˳����� ���Է���ֵ
	return 0;
}

// ��ȡ�׶��Ĵ洢��ռ�ÿռ�
std::string GetReadableSize(uintmax_t size) {
	static const char* units[] = { "B", "KB", "MB", "GB", "TB" };
	int unitIndex = 0;
	double sizeBytes = static_cast<double>(size);

	while (sizeBytes >= 1024 && unitIndex < 4) {
		sizeBytes /= 1024;
		unitIndex++;
	}

	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << sizeBytes << " " << units[unitIndex];

	return ss.str();
}

int RepositoryOperation::GetRepositorySize() {
	fs::path currentPath = fs::current_path();//��ȡ��ǰ·��
	fs::path repoPath = currentPath / ".Rysigy";
	if (!fs::exists(repoPath)) {
		cout << "��ǰ·����δ�����洢��" << endl;
		return 1;
	}

	cout << "��ǰ�洢��·����" << repoPath << endl;
	uintmax_t totalSize = 0;
	// �����ļ����е������ļ������ļ���
	for (const auto& entry : fs::recursive_directory_iterator(repoPath)) {
		if (fs::is_regular_file(entry)) {
			// ��ȡ�ļ��Ĵ�С���ۼӵ��ܴ�С
			totalSize += fs::file_size(entry);
		}
	}
	std::string readableSize = GetReadableSize(totalSize);
	cout << "�洢��ռ�ÿռ�Ϊ: " << readableSize << endl;

	cout << "������ⰴ���˳�..." << std::endl;
	[[maybe_unused]] int waiting = _getch(); // �ȴ��û�������������˳����� ���Է���ֵ

	return 0;
}