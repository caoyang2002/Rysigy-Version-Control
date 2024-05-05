#include "pch.h"
#include "repository_operation.h"


namespace fs = std::filesystem;
using namespace std;



// 获取当前时间 , 返回字符串: 年-月-日 时:分:秒
std::string getCurrentTime() {
	std::time_t currentTime = std::time(nullptr);
	std::tm localTime;
	localtime_s(&localTime, &currentTime);

	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
	//std::cout << "Formatted time: " << buffer << "\t";

	return std::string(buffer);
}

// 控制台输出颜色
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
		//	std::cout << "颜色对应的值为：" << colorNum << std::endl;
	}
	else {
		//	std::cout << "找不到对应的颜色值" << std::endl;
	}
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | colorNum);
	printf(s);
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
}



// 存储库操作指定输出到 系统日志 和 系统内的用户日志


// 格式化输出到控制台
// [错误] 错误简介, 错误的来源, 4(红色)
int FOut(std::string information, std::string details_path, Color color) {

	// 控制台输出
	string logCurrent = information + "\t" + details_path;
	COLOR_PRINT(logCurrent.c_str(), color);

	return 0;
}

// 自动查找存储库 输出到系统日志文件, 不会在控制台显示, 需要传入当前路径
// ([信息] 信息简介, 信息的来源或说明, 当前路径)
int FOuttoSysLog(std::string information, std::string details_path) {
	//cout << information << "\t" << details_path << endl;
	//cout << "[开始] 系统日志记录" << endl;

	fs::path systemLogFilePath = fs::current_path() / ".Rysigy" / "DATABASE" / "system_log.ryg";

	//cout << "[日志] 系统日志文件路径: " << systemLogFilePath << endl;
	

	// 检查日志文件是否存在
	if (!fs::exists(systemLogFilePath)) {
		COLOR_PRINT("[错误] 日志文件不存在, 将创建日志文件\n", RED);
	}

	std::ofstream outputLogFile;
	// 追加(创建) 文件内容
	outputLogFile.open(systemLogFilePath, std::ios::out | std::ios::app); // 按位或(见true则true)    std::ios::out 的二进制是 1   std::ios::app 的二进制是 100 , 按位或操作后 ,他们的二进制是 101 即写入和追加模式
	if (outputLogFile.is_open()) {
		string currentTime = getCurrentTime(); // 获取当前时间 
		string logContent = currentTime + "\t" + information + "\t" + details_path + "\t\n";
		outputLogFile << logContent << endl;
		outputLogFile.close();
		//cout << "[成功] 日志已成功写入文件：" << logContent;
	}
	else {
		std::cerr << "[错误] 无法打开日志文件：" << systemLogFilePath << std::endl;
	}

//	cout << "[结束] 日志记录" << endl;
	return 0;
}

// 在日志文件中输入元信息
int enterMetaInfotoLogFile(fs::path pathName) {
	FOut("[执行] 输入基本信息到文件中\n", "", WHITE);

	// 输出 hash 到文件中
	std::ofstream outputLogFile;

	// 追加(创建) 文件内容
	//outputLogFile.open(systemLogFilePath, std::ios::out | std::ios::app); // 按位或(见true则true)    std::ios::out 的二进制是 1   std::ios::app 的二进制是 100 , 按位或操作后 ,他们的二进制是 101 即写入和追加模式
	//if (outputLogFile.is_open()) {
	//	string currentTime = getCurrentTime(); // 获取当前时间 
	//	string logContent = currentTime + "\t" + information + "\t" + details_path + "\t\n";
	//	outputLogFile << logContent << endl;
	//	outputLogFile.close();
	//	cout << "[成功] 日志已成功写入文件：" << endl;
	//}
	//else {
	//	std::cerr << "[错误] 无法打开日志文件：" << systemLogFilePath << std::endl;
	//}

}







// 日志文件:
// 
// 1. 用户执行记录日志 (控制台输出的所有日志)  -- .Rysigy/VERSIONS/fileane/_Vx/filename/history.log
// 
// 2. 用户错误日志 (只记录用户操作的错误日志)  -- .Rysigy/DATABASE/data/user_log.ryg
// 
// 3. 系统日志 (记录系统的操作日志)          -- .Rysigy/DATABASE/metadata/rysigy_log.ryg





//查找目录节点

struct DirectoryNode {
	std::string name;
	std::vector<DirectoryNode> subdirectories;
	std::vector<std::string> files;
};

// 递归检查存储库目录结构是否完整
bool CheckDirectoryExist(const fs::path& basePath, const DirectoryNode& node) {
	fs::path currentPath = basePath / node.name;

	// 检查当前目录是否存在
	if (!fs::exists(currentPath)) {
		cout << "目录不存在" << currentPath << endl;
		//std::cout << "目录不存在：" << currentPath << std::endl;
		return false;
	}
	else {
		cout << "目录存在" << currentPath << endl;
	}

	// 检查是否存在所需的文件
	for (const std::string& file : node.files) {
		fs::path filePath = currentPath / file;
		if (!fs::exists(filePath)) {
			cout << "文件不存在" << filePath << endl;
			//std::cout << "文件不存在：" << filePath << std::endl;
			return false;
		}
		else
		{
			cout << "文件存在" << filePath << endl;
		}
	}

	// 递归检查子目录
	for (const DirectoryNode& subdir : node.subdirectories) {
		if (!CheckDirectoryExist(currentPath, subdir)) {
			return false;
		}
	}

	// 如果所有检查通过，则返回 true
	return true;
};

// ---------------------------------------------------------------------------------------------------------------------------------------------------------
//检查存储库是否存在且完整
int RepositoryOperation::CheckRepository() {
	cout << "[执行] 检查存储库是否完整..." << endl;
	std::string repoName = ".Rysigy";
	cout << "[信息] 存储库的名称是: " << repoName << endl;
	fs::path currentPath = fs::current_path();
	cout << "[信息] 当前路径是: " << currentPath << endl;
	fs::path repoPath = currentPath / repoName;
	cout << "[信息] 存储库的路径是: " << repoPath << endl;

	DirectoryNode customDirStructure;
	customDirStructure.name = repoPath.string();  // 设置基础目录的名称
	customDirStructure.subdirectories = {   //格式 {    {  "目录一", {{"目录一/子目录一", "目录一/子目录二"},{"目录一/子目录二"}}  },   {"目录二",{目录二}}    }
		// {{"1" ,{"1.1", "1.2"}},  {2,{{"2.1",{"2.1.1", "2.1.2"},{"2.2",{2.2.1}}}, {1.2},{1.3}}},{2,{2.1}},{3}}
		// 使用 {} 存储文件/目录名, 需要使用 "" 标记,
		{ "DATABASE" }, { "VERSIONS" }  // 子目录 VERSIONS 为空
	};

	if (fs::exists(repoPath) && fs::is_directory(repoPath)) {
		cout << "[存在] 存储库位置: " << repoPath << endl;
		//fs::path basePath = fs::current_path();  // 设置基础路径
		if (CheckDirectoryExist(repoPath, customDirStructure)) { //检查存储库中的相关目录是否存在
			cout << "[检查] 存储库完整性 " << repoPath << endl;
			FOut("[成功] 存储库相关文件存在 " , repoPath.string(), GREEN);
			cout << "\n";
		}
	}
	else {
		FOut("[错误] 存储库 '.Rysigy' 文件夹不存在","请在空白处点击鼠标右键选择'创建存储库'\n",RED);
	}
	cout << "点击任意按键退出..." << std::endl;
	[[maybe_unused]] int waiting = _getch(); // 等待用户按下任意键后退出程序。 忽略返回值
	return 0;
}

// -----------------------------------打开存储库
int RepositoryOperation::OpenRepository() {
	FOuttoSysLog("[执行] 打开存储库", "");
	// 获取当前路径
	fs::path currentPath = fs::current_path();
	// 拼接文件夹路径
	std::wstring filePath = currentPath.wstring() + L"\\.Rysigy";
	wcout << L"打开的路径是: " << filePath << endl;
	// 判断存储库是否存在
	if (!fs::exists(filePath)) {
		// 文件夹不存在，输出错误信息
		cout << "[错误] 文件不存在" << endl;
		cout << "点击任意按键退出..." << std::endl;
		[[maybe_unused]] int waiting = _getch(); // 等待用户按下任意键后退出程序。 忽略返回值
		return 1;
	}
	HINSTANCE openFileResult = ShellExecuteW(NULL, L"open", filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
	if ((long)openFileResult <= 32) {
		cout << "[失败] 文件无法打开" << endl;
	}

	cout << "[成功] 文件已打开\n";
	wcout << L"[成功] 文件已打开" << filePath << endl;
	cout << "点击任意按键退出..." << endl;
	[[maybe_unused]] int waiting = _getch(); // 等待用户按下任意键后退出程序。 忽略返回值
	return 0;
}

// 创建存储库
int RepositoryOperation::CreateRepository() {
	fs::path repoPath = fs::current_path() / ".Rysigy";
	if (fs::exists(repoPath)) {
		std::cout << "[提示] 存储库已存在\a" << std::endl;
		std::cout << "点击任意按键退出..." << std::endl;
		[[maybe_unused]] int waiting = _getch();

		return 0;
	}

	if (fs::create_directories(repoPath)) {
		cout << "[成功] 存储库创建完成" << endl;
		std::string cmd = "attrib +h " + repoPath.string(); // 将路径加上双引号
		int folderHidden = system(cmd.c_str()); // CMD命令
		if (folderHidden != 0) {
			cout << "[提示] 无法将存储库设置为隐藏属性" << endl;
		}
		else {
			cout << "[成功] 已将存储库设置为隐藏属性" << endl;
		}
	}
	else {
		std::cout << "[失败] 创建失败: " << repoPath << std::endl;
		std::cout << "点击任意按键退出..." << std::endl;
		[[maybe_unused]] int waiting = _getch();
		return 1;
	}

	fs::path databasePath = repoPath / "DATABASE";
	fs::path versionsPath = repoPath / "VERSIONS";

	if (!fs::create_directories(databasePath) || !fs::create_directories(versionsPath)) {
		std::cout << "[失败] 创建失败: " << versionsPath << std::endl;
		return 1;
	}
	else
	{
		cout << "[成功] 数据库目录创建完成" << endl;
	}

	fs::path systemLog = databasePath / "system_log.ryg";
	std::ofstream file(systemLog);
	if (!file.is_open()) {
		std::cout << "[失败] 日志文件创建失败" << std::endl;
		return 1;
	}
	else
	{
		cout << "[成功] 日志文件创建完成" << endl;
	}

	//file << "Hello, world!" << std::endl;
	file.close();
	//std::cout << "[成功] 日志文件创建成功" << std::endl;
	std::cout << "[成功] 存储库结构创建完成" << std::endl;
	FOuttoSysLog("[创建] 创建存储库", fs::current_path().string() + "/.Rysigy");

	std::cout << "点击任意按键退出..." << std::endl;
	[[maybe_unused]] int waiting = _getch();

	return 0;
}

// 使用多线程和异步编程方式删除文件夹及其内容的函数
void deleteDirectoryMulThread(const fs::path& dirPath, int numThreads = 4)
{
	// 开始计时
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
					cout << "[删除] 目录: " << path << endl;
					fs::remove_all(path);
				}
				else {
					cout << "[删除] 文件: " << path << endl;
					fs::remove(path);
				}
			}
			catch (const std::exception& e) {
				throw std::runtime_error("删除 " + path.string() + " 失败: " + e.what());
			}
		}
		else {
			throw std::runtime_error("文件或目录 " + path.string() + " 不存在");
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

	// 等待所有异步删除任务完成
	for (auto& future : futuresToDelete)
	{
		future.wait();
	}

	// 等待任务完成后才可访问已被删除的文件或目录
	// ...

	// 最后删除根目录
	if (fs::exists(repoPath)) {
		fs::remove(repoPath);
	}


	// 结束计时
	auto endTime = std::chrono::high_resolution_clock::now();
	// 计算耗时（以毫秒为单位）
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
	// 输出耗时
	std::cout << "程序执行耗时: " << duration << " 毫秒" << std::endl;
}

//删除存储库-------------------------------------------
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
	//fs::path prevPath; // 记录上一个路径,
	//do{
	//	// 忽略"."和".."两个结果
	//	if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0){
	//		continue;
	//	}
	//	string strFileName;
	//	strFileName.assign(dirPath.string());
	//	strFileName.append("\\");
	//	strFileName.append(findData.cFileName);
	//	if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {// 是否是目录
	//		if (strFileName != prevPath) {
	//			fs::remove(prevPath);
	//		}
	//		cout << "[递归] 目录: " << strFileName << endl;
	//		deleteDirectory(strFileName.c_str());
	//		prevPath = strFileName;
	//	}else{
	//		cout << "[删除] 文件: " << strFileName << endl;
	//		DeleteFileA(strFileName.c_str());
	//	}
	//} while (FindNextFileA(hFind, &findData));
	//FindClose(hFind);
	//LPCSTR lpPath = dirPath.string().c_str();
	//RemoveDirectoryA(lpPath);
	//
	//

// 调用系统 API 删除
//string  command = "rd /s/q " + dirPath.string();
//system(command.c_str());//调用windows api删除指定文件夹

// 无法删除有文件的文件夹
//if (!fs::exists(dirPath) || !fs::is_directory(dirPath))
//{
//	return;
//}

//std::vector<fs::path> pathsToDelete; // 待删除的路径集合

//// 缓存已访问的路径
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
//				if (visitedPaths.count(entryPath) == 0) // 避免重复访问
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
//		// 处理异常，例如输出错误信息
//		std::cerr << "Error occurred during directory traversal: " << ex.what() << std::endl;
//	}
//}

//// 批量删除路径
//std::vector<std::future<void>> futures;
//const size_t batchSize = 50; // 每批次的大小
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
//				// 处理异常，例如输出错误信息
//				std::cerr << "Error occurred during deletion: " << ex.what() << std::endl;
//			}
//		}
//		}));

//	startIndex = endIndex;
//}

//// 等待所有删除任务完成
//for (auto& future : futures)
//{
//	future.wait();
//}

// 可用
	if (!fs::exists(dirPath) || !fs::is_directory(dirPath))
	{
		return;
	}

	for (const auto& entry : fs::directory_iterator(dirPath))
	{
		const auto& currentPath = entry.path();

		if (fs::is_directory(currentPath))
		{
			// 递归删除子文件夹的内容
			std::cout << "[进入] 子目录: " << currentPath << std::endl;
			deleteDirectory(currentPath);
		}
		else if (fs::is_regular_file(currentPath))
		{
			// 删除文件
			std::cout << "[删除] 文件: " << currentPath << std::endl;
			fs::remove(currentPath);
		}
	}

	// 删除文件夹
	if (fs::is_empty(dirPath))
	{
		std::cout << "[删除] 目录" << dirPath << std::endl;
		fs::remove(dirPath);
	}
	else
	{
		std::cout << "[删除] 目录及其内容" << dirPath << std::endl;
		fs::remove_all(dirPath);
	}
}

//
int RepositoryOperation::DeleteRepository() {
	string confirm;
	FOut("[重要] 删除之前请检查是否有未关闭的文件", "如果未关闭, 在删除过程中此程序将异常退出 !\n", RED);
	cout << "您确定要删除吗? \n输入 yes 确定删除, 输入其他字符取消删除...\n>>>";
	cin >> confirm;
	fs::path currentPath = fs::current_path();//获取当前路径
	fs::path directoryPath = currentPath / ".Rysigy";

	if (confirm == "yes" || confirm == "Yes" || confirm == "YES") {
		if (fs::exists(directoryPath)) {
			if (fs::is_directory(directoryPath)) {
				deleteDirectoryMulThread(directoryPath, 4); //多线程删除文件
				std::cout << "[成功] 存储库已删除" << std::endl;
			}
			else {
				std::cout << "[提示] 检查到一个与存储库同名的文件" << std::endl;
			}
		}
		else {
			cout << "[错误] 存储库不存在\n";
		}
	}
	else
	{
		cout << "[取消] 已取消删除\n";
	}
	cout << "点击任意按键退出..." << std::endl;
	[[maybe_unused]] int waiting = _getch(); // 等待用户按下任意键后退出程序。 忽略返回值
	return 0;
}

// 获取易读的存储库占用空间
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
	fs::path currentPath = fs::current_path();//获取当前路径
	fs::path repoPath = currentPath / ".Rysigy";
	if (!fs::exists(repoPath)) {
		cout << "当前路径下未创建存储库" << endl;
		return 1;
	}

	cout << "当前存储库路径是" << repoPath << endl;
	uintmax_t totalSize = 0;
	// 遍历文件夹中的所有文件和子文件夹
	for (const auto& entry : fs::recursive_directory_iterator(repoPath)) {
		if (fs::is_regular_file(entry)) {
			// 获取文件的大小并累加到总大小
			totalSize += fs::file_size(entry);
		}
	}
	std::string readableSize = GetReadableSize(totalSize);
	cout << "存储库占用空间为: " << readableSize << endl;

	cout << "点击任意按键退出..." << std::endl;
	[[maybe_unused]] int waiting = _getch(); // 等待用户按下任意键后退出程序。 忽略返回值

	return 0;
}