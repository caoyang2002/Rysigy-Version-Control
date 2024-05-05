#include "pch.h"
#include "file_operation.h"

// ================================================================================================================================================
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


// 查询文件版本数量,并返回文件
std::vector<std::string> GetAllFileVersions(fs::path pathName){
	cout << "[执行] 获取该文件的版本总数" << endl;
	int verTotal = 0; // 版本总数
	fs::path currentPath = pathName;//获取当前路径
	//查询当前是否有存储库
	fs::path repoPath = currentPath.parent_path() / ".Rysigy";

	//获取文件名
	std::string sourFileName = currentPath.filename().string(); // 获取文件名 filename.jpg
	size_t lastDotPosition = sourFileName.find_last_of('.');// 查找最后一次出现点的位置
	if (lastDotPosition != std::string::npos) {//如果找到了
		// 如果找到点，则将其替换为下划线
		sourFileName.replace(lastDotPosition, 1, "_");// 替换为_ (替换位置的索引, 在索引处替换的字符数,替换的字符)
		cout << "[信息] 文件名版本目录名是" << sourFileName << endl;
	}

	std::vector<std::string> verNames;// 版本名向量
	if (fs::exists(repoPath)) {
		// 获取版本路径
		fs::path verPath = repoPath / "VERSIONS" / sourFileName;
		if (fs::exists(verPath)) { // 查询该文件是否有版本库
			for (const auto& entry : fs::directory_iterator(verPath)) { // 遍历版本目录下的所有文件
				if (fs::is_directory(entry)) {
					std::string folder_name = entry.path().filename().string();
					if (folder_name.length() > 2 && folder_name.substr(0, 2) == "_V")
					{
						verNames.push_back(folder_name);//装入向量
						++verTotal;
					}
				}
			}
			cout << "[信息] 该文件共有 " << verTotal << " 个版本\n";

		}else{
			cout << "[错误] 当前文件没有进行版本管理, 无法获取版本信息" << endl;
		}
	}else{
		cout << "[错误] 当前目录下没有存储库" << endl;
	}

	return verNames;

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




// 格式化输出
// [错误] 错误简介, 错误的来源, 4(红色)
int FOut(std::string information, std::string details_path, Color color) {

	// 控制台输出
	string logCurrent = information + "\t" + details_path;
	COLOR_PRINT(logCurrent.c_str(), color);
	
	return 0;  
}

// 输出到日志文件
int FOuttoLog(std::string information, std::string details_path, Color color, fs::path logFilePath) {

	// 控制台输出
	string logCurrent = information + "\t" + details_path;
	COLOR_PRINT(logCurrent.c_str(), color);

	// 检查日志文件是否存在
	if (!fs::exists(logFilePath)) {
		COLOR_PRINT("[错误] 日志文件不存在", RED);
	}
	//日志输出
	std::fstream outputLogFile(logFilePath);
	if (outputLogFile.is_open()) {
		string currentTime = getCurrentTime();
		string logContent = currentTime + "\t" + information + "\t" + details_path + "\t";
		outputLogFile << logContent;
		outputLogFile.close();
		std::cout << "日志已成功写入文件：" << logFilePath << std::endl;
	}
	else {
		std::cerr << "无法打开文件：" << logFilePath << std::endl;
	}

	return 0; 
}




// 在日志文件中输入元信息
int enterMetaInfotoFile(fs::path pathName) {
	FOut("[执行] 输入基本信息到文件中\n", "", WHITE);

}

// 获取文件日志信息
fs::path getFileLog(fs::path pathName) {
	FOut("[执行] 获取文件版本信息", pathName.string(), WHITE);
	cout << endl;

	// 获取文件库下的版本目录名
	string fileName = pathName.filename().string();
	string repcFileName = fileName;

	size_t lastDotPosition = fileName.find_last_of('.');// 查找最后一次出现点的位置
	if (lastDotPosition != std::string::npos) {//如果找到了
		// 如果找到点，则将其替换为下划线
		repcFileName.replace(lastDotPosition, 1, "_");// 替换为_ (替换位置的索引, 在索引处替换的字符数,替换的字符)
	}
	cout << "原始文件名是: " << fileName << endl;
	cout << "替换后的文件名是: " << repcFileName << endl;

	fs::path repoPath = pathName.parent_path();
	fs::path fileVerPath = repoPath / ".Rysigy" / "VERSIONS" / repcFileName;
	if (!fs::exists(fileVerPath)) {
		FOut("[错误] 该文件未进行版本管理\n", fileVerPath.string(), RED);
		return "文件未进行版本管\n理";
	}

	std::vector<std::string> allVerName = GetAllFileVersions(pathName);
	cout << "[信息] 当前文件的版本有:\n";
	for (const auto& version : allVerName) {  // 使用范围-based for 循环
		//std::cout << "Version: " << version << std::endl;
		// 或者在这里进行其他操作，比如将 version 写入文件等等
		cout << "  \t" << version << endl;
	}
	FOut("[输入] 查询文件的版本信息\n", "输入您需要查询的版本 (仅数字)\n>>>", CYAN);
	string specVerNum;
	cin >> specVerNum;
	string specVerName = "_V" + specVerNum;
	fs::path specFileVerPath = fileVerPath / specVerName; //获取到特定的文件版本路径

	// 文件的版本日志名
	string fileLogName;
	std::string newExtension = ".log";
	size_t dotIndex = fileName.find_last_of('.');
	//cout << "测试文件名是:" << fileName << endl;
	if (dotIndex != std::string::npos) {
		std::string baseName = fileName.substr(0, dotIndex); // 把点前面的作为基础文件名
		fileLogName = baseName + newExtension; // 新的拓展名
	}
	else {
		FOut("[错误] 没有找到后缀名", fileLogName, RED);
	}

	fs::path specFileVerLogPath = specFileVerPath / fileLogName;
	if (!fs::exists(specFileVerLogPath)) {
		FOut("[错误] 文件版本日志不存在", specFileVerPath.string(), RED);
		return "文件版本日志不存在";
	}

	//读取文件版本日志内容
	std::ifstream logFile(specFileVerLogPath);  // 打开文件

	if (logFile.is_open()) {
		std::string line;
		while (std::getline(logFile, line)) {  // 按行读取文件内容
			//std::cout << line << std::endl;  // 输出每一行的内容
			FOut("|", line + "\t|\n", LIGHT_CYAN);
		}
		logFile.close();  // 关闭文件
	}
	else {
		FOut("[错误] 日志文件打开失败", "", RED);
		//std::cout << "文件打开失败" << std::endl;
	}
	return specFileVerLogPath;
}



// --------------------------------------------------------------------------------------


// 二进制文件复制 传入源文件路径和目标文件路径
void BinaryFileCopy(const std::string& srcFile, const std::string& dstFile) {
	FOut("[执行] 复制文件", "", WHITE);
	std::ifstream input{ srcFile, std::ios::in | std::ios::binary };
	if (!input) {
		std::cout << "[失败] 无法打开源文件" << std::endl;
		return;
	}

	if (std::filesystem::exists(dstFile)) {
		std::cout << "[提示] 文件已被创建, 将复制到已创建的文件\n"
			<< "复制到" << dstFile << std::endl;
	}

	std::ofstream output{ dstFile, std::ios::out | std::ios::binary };
	if (!output) {
		std::cout << "[失败] 无法打开目标文件" << std::endl;
		return;
	}

	auto file_size = std::filesystem::file_size(srcFile);
	const unsigned BufferSize = 512;
	char buffer[BufferSize]{};

	std::cout << "被复制的文件: " << srcFile << endl;
	// 文件不到一个Buffer大
	if (file_size < BufferSize) {
		if (!input.read(buffer, file_size)) {
			throw std::runtime_error("[错误] 读取文件时遇到错误");
		}
		if (!output.write(buffer, file_size)) {
			throw std::runtime_error("[错误] 写入文件时遇到错误");
		}
	}
	else {
		// 大文件分为多个块来拷，并打印进度，进度乘10，是为了不让结果看起来都是0，一直不打印
		auto chunks = file_size / BufferSize;
		// 这里用int是因为 剩余大小小于 512，用int就可以了，不需要 std::size_t
		int remaining = file_size % BufferSize;
		int progress{}, oldProgress{};
		for (int i = 0; i < chunks; ++i) {
			if (!input.read(buffer, BufferSize)) {
				throw std::runtime_error(
					"[错误] 读取文件时遇到错误");
			}

			if (!output.write(buffer, BufferSize)) {
				throw std::runtime_error(
					"[错误] 写入文件时遇到错误");
			}

			// 先转浮点避免全零，再转整型便于比较
			// 类型转换建议使用 static_cast，不要直接强转
			// 因为static_cast会做类型检查
			progress = static_cast<int>((10 * static_cast<float>(i) / chunks));
			if (progress != oldProgress) {
				std::cout << '.';
			}
			oldProgress = progress;
		}

		// 下一次的读取将会小于 BufferSize
		// 但是buffer 里面可能还有上次读取的残存数据，
		// 所以需要使用memset清空
		// 比较底层的C操作，但是没办法，因为读写二进制必须按char* [byte]处理
		memset(buffer, '\0', BufferSize);

		if (remaining > 0) {
			if (!input.read(buffer, remaining)) {
				throw std::runtime_error(
					"[错误] 读取文件时遇到错误");
			}
			if (!output.write(buffer, remaining)) {
				throw std::runtime_error(
					"[错误] 写入文件时遇到错误");
			}
			std::cout << '.';
		}

		FOut("\n[成功] 复制完成", "", GREEN);
		//std::cout << "\n[成功] 复制完成! " << std::endl;

		input.close();
		output.close();
	}
}



//获取路径下文件或文件夹的下一个版本, 传入需要查询的路径
std::string NextVersion(fs::path pathName) {
	cout << "[执行] 查询版本数量: " << pathName << endl;

	string time = getCurrentTime();//获取当前时间

	// 获取有多少个版本
	if (fs::exists(pathName)) {
		int nextNumber = 0; // 下一个版本

		fs::path fileInDirPath = pathName.parent_path() ; // 获取文件所在目录的路径
		cout << "[信息] 文件所在目录的路径是:" << fileInDirPath << endl;

		std::string sourFileName = pathName.filename().string(); // 获取文件名 filename.jpg
		size_t lastDotPosition = sourFileName.find_last_of('.');// 查找最后一次出现点的位置
		if (lastDotPosition != std::string::npos) {//如果找到了
			// 如果找到点，则将其替换为下划线
			sourFileName.replace(lastDotPosition, 1, "_");// 替换为_ (替换位置的索引, 在索引处替换的字符数,替换的字符)
			cout << "[信息] 文件名版本目录名是" << sourFileName << endl;
		}

		fs::path verPath = fileInDirPath / ".Rysigy" / "VERSIONS"/ sourFileName;
		cout << "[信息] 版本路径是: " << verPath << endl;

		if (!fs::exists(verPath) || fs::is_empty(verPath)) {
			// 版本路径下没有任何目录
			cout << "[警告] 此文件尚未进行版本管理" << endl;
			cout << "[信息] 创建第一个版本 _V1" << endl;
			return "_V1";  // 返回默认的版本号
		}

		// 获取下一个版本
		for (const auto& entry : fs::directory_iterator(verPath)) {// 获取版本路径下的所有版本
			// 定义一个正则表达式来匹配以"_V"或"_v"开头的目录名称，后跟一个或多个数字
			std::regex pattern(R"([_vV](\d+))");	//[_vV]：表示匹配单个字符，可以是小写字母 "v"、大写字母 "V" 或下划线 "_" 中的任何一个。  
			// (\\d+)：使用括号表示一个捕获组，表示匹配一个或多个数字。
			// \\d 是正则表达式中匹配数字的方式，+ 表示匹配一个或多个数字。

			// 尝试在当前目录名称中查找匹配的部分
			std::smatch match;
			fs::path itemPath = entry;
			std::string fileNameDirName = itemPath.filename().string();
			cout << "[信息] 该文件已有版本: " << fileNameDirName << endl;
			if (std::regex_search(fileNameDirName, match, pattern)) {
				// 提取匹配的数字部分并转换为整数
				int currentNumber = std::stoi(match[1].str());

				// 构造下一个目录名称（数字加一）
				nextNumber = currentNumber + 1;
				//cout << "[信息] 下一个版本为 _V" << nextNumber << endl;
				//return "_" + std::to_string(nextNumber);
			}
			else {
				// 如果没有匹配的部分，则返回默认返回下一个版本是 _V1
				cout << "[信息] 下一个版本为 _V1" << endl;
				return "_V1";
			}
		}
		string nextVer = "_V" + std::to_string(nextNumber);
		cout << "下一个版本为: " << nextVer << endl;
		return nextVer;
	}
	else {
		cout << "[警告] 当前路径不存在" << pathName << endl;
	}
	return "_V1";

}


// ================================================================================================================================================

//-------------------------------------------------------------------------------------------------------------
// 查询该文件的所有版本
int  file_operation::QueryFileVersions(string pathName) {
	cout << "[执行] 获取该文件的版本总数" << endl;
	int verTotal = 0; // 版本总数
	fs::path currentPath = pathName;//获取当前路径
	//查询当前是否有存储库
	fs::path repoPath = currentPath.parent_path() / ".Rysigy";

	//获取文件名
	std::string sourFileName = currentPath.filename().string(); // 获取文件名 filename.jpg
	size_t lastDotPosition = sourFileName.find_last_of('.');// 查找最后一次出现点的位置
	if (lastDotPosition != std::string::npos) {//如果找到了
		// 如果找到点，则将其替换为下划线
		sourFileName.replace(lastDotPosition, 1, "_");// 替换为_ (替换位置的索引, 在索引处替换的字符数,替换的字符)
		cout << "[信息] 文件名版本目录名是: " << sourFileName << endl;
	}

	if (fs::exists(repoPath)) {
		// 获取版本路径
		fs::path verPath = repoPath / "VERSIONS" / sourFileName;
		if (fs::exists(verPath)) { 
			for (const auto& entry : fs::directory_iterator(verPath)) { // 遍历版本目录下的所有文件
				if (entry.path().filename().string().substr(0, 2) == "_V" && fs::is_directory(entry)) {
					++verTotal;
				}
			}
			cout << "[信息] 该文件共有 " << verTotal << " 个版本\n";

		}else{
			cout << "[错误] 当前文件没有进行版本管理, 无法获取版本信息" << endl;
		}
	}else{
		cout << "[错误] 当前目录下没有存储库" << endl;
	}
	cout << "点击任意按键退出..." << std::endl;
	[[maybe_unused]] int waiting = _getch(); // 等待用户按下任意键后退出程序。 忽略返回值
	return 0;
}




//创建文件版本
int file_operation::CreateFileVersions(string filePath) {

// 获取当前文件路径, 创建文件名目录, 获取下一个版本, 创建版本目录, 复制文件
	fs::path currentPath = filePath; //fs::current_path();
	cout << "当前文件是: " << currentPath << endl;
	string nextFileVersion = NextVersion(currentPath); // 获取该文件的下一个版本

	std::string sourFileName = currentPath.filename().string(); // 获取文件名 filename.jpg
	size_t lastDotPosition = sourFileName.find_last_of('.');// 查找最后一次出现点的位置

	if (lastDotPosition != std::string::npos) {//如果找到了
		// 如果找到点，则将其替换为下划线
		sourFileName.replace(lastDotPosition, 1, "_");// 替换为_ (替换位置的索引, 在索引处替换的字符数,替换的字符)
		cout << "[信息] 文件名目录名称: " << sourFileName << endl;
	}
	// 声明文件名目录
	fs::path fileNameDirPath = currentPath.parent_path() / ".Rysigy" / "VERSIONS" / sourFileName; // "/"附加到后面,不会改变自身   "/=" 会改变自身 
	cout << "[信息] 文件名路径是" << fileNameDirPath << endl;

	// 创建目录
	//std::filesystem::path directoryPath(fileNameDirPath);//文件名目录路径

	if (!fs::exists(fileNameDirPath)) {  // 如果文件名目录不存在
		if (!fs::create_directories(fileNameDirPath)) {  // 创建文件名目录，并检查结果
			std::cout << "[失败] 文件名目录创建失败" << std::endl;
			return 1;
		}
		std::cout << "[成功] 文件名目录创建成功" << std::endl;
	}

	fs::path verPath = fileNameDirPath / nextFileVersion; // 版本目录路径
	if (!fs::create_directory(verPath)) { // 创建版本目录，并检查结果
		std::cout << "[失败] 版本文件夹创建失败" << std::endl;
		return 1;
	}
	std::cout << "[成功] 版本文件夹创建成功" << std::endl;

	// 创建一个目标文件
	fs::path destFilePath = verPath / currentPath.filename();
	BinaryFileCopy(currentPath.string(), destFilePath.string()); // 复制文件

	// 创建日志文件
	std::string newExtension = ".log";  // 要更改的新后缀
	fs::path logFilePath = destFilePath.replace_extension(newExtension);
	std::ofstream logFile(logFilePath);  // 创建文件
	if (logFile) {  // 检查是否成功打开


		logFile << getCurrentTime(); //在日志中输入创建时间信息

		// 询问用户是否需要输入更改信息
		int confirmChange;
		FOut("[询问] 您是否需要输入版本更改信息?\n", "\n- 0 \t需要\n- 1 \t不需要", LIGHT_YELLOW);
		cout << ">>>";
		cin >> confirmChange;
		if (0 == confirmChange) {
			FOut("[输入] 请输入更改内容, 按回车键结束输入", "", LIGHT_YELLOW);
			string logContent;
			logContent = "\n" + logContent;
			cin >> logContent;
			logFile << logContent;
		
		}
		else if(1 == confirmChange) {
			FOut("[取消] 已取消输入", "", WHITE);
		}
		logFile.close();  // 关闭文件
		std::cout << "[成功] 日志文件创建成功并写入内容" << std::endl;
	}
	else {
		std::cout << "[失败] 无法创建日志文件" << std::endl;
	}
	std::cout << "[成功] 文件版本创建成功" << std::endl;


	cout << "点击任意按键退出..." << std::endl;
	[[maybe_unused]] int waiting = _getch(); // 等待用户按下任意键后退出程序。 忽略返回值

	return 0;
}

// --------------------------------------------------- 获取文件指定版本
int file_operation::GetSpecilVerofFile(fs::path pathName) {
	FOut("[执行] 获取文件指定版本", pathName.filename().string(), WHITE);
	cout << endl;
	int inputSelect; // 选择
	fs::path logFilePath;//日志文件路径
	do {
		logFilePath = getFileLog(pathName);//获取文件日志信息
		cout << "文件路径: " << logFilePath.string() << endl;
		cout << "输入: \n- 0       \t选择查看版本信息\n- 其他数字\t获取数字对应的版本文件" << endl;
		cin >> inputSelect;// 获取输入
	} while (0 == inputSelect);

	//FOut("[输入] 请输入版本末尾的数字", "\n>", CYAN);
	string inputVerNum = std::to_string(inputSelect);

	//
	
	fs::path fileVerNamePath = logFilePath.parent_path() / pathName.filename();
	if (!fs::exists(fileVerNamePath)) {
		FOut("[错误] 文件不存在", fileVerNamePath.string(), RED);
	}
	
	// 把版本文件复制到当前目录
	std::string origFilename = pathName.filename().string(); // 原始文件名
	// 版本文件路径
	fs::path verFilePath = logFilePath.parent_path() / origFilename;
	//当前版本名
	string fileVerName = logFilePath.parent_path().filename().string();
	//新文件名
	std::string newFileName = origFilename.substr(0, origFilename.find_last_of('.')) + fileVerName  + origFilename.substr(origFilename.find_last_of('.'));
	fs::path newFilePath = pathName.parent_path() / newFileName;

	if (fs::exists(newFilePath)) {
		FOut("[警告] 版本文件已存在:\n- 0       \t自定义区分名称\n- 其他数字 \t覆盖", "\n>>>",RED);
		int inputEx;
		cin >> inputEx;
		if (0 == inputEx) {
			reInput:
			cout << "---- 请输入作为区分的字符\n";
			string str;
			cout << ">>>";
			cin >> str;
			std::string specialCharacters = R"(\/*|?<>"*)"; // 特殊字符列表
			size_t found = str.find_first_of(specialCharacters);
			if (found != std::string::npos) {
				cout << "---- 输入的字符不可以包含 \\/|*\"<>? 请修改" << endl;
				goto reInput;
			}

			newFileName = origFilename.substr(0, origFilename.find_last_of('.')) + fileVerName +  str + origFilename.substr(origFilename.find_last_of('.'));
			newFilePath = pathName.parent_path() / newFileName;
		}
	}
	
	std::ofstream file(newFilePath);  // 打开文件 example.txt (如果不存在则会创建)
	cout << "[信息] 导出的版本文件路径为: " << newFilePath << endl;

	if (file.is_open()) {
		//把文件复制到当前路径下
		BinaryFileCopy(verFilePath.string(), newFilePath.string()); // 复制文件
		file.close();  // 关闭文件
		//std::cout << "已导出版本文件" << std::endl;
		FOut("[成功] 版本文件已导出", newFileName, GREEN);
		cout << endl;
	}
	else {
		//std::cout << "无法导出版本文件" << std::endl;
		FOut("[错误] 无法导出版本文件", newFileName, RED);
		cout << endl;
	}
	cout << "点击任意按键退出..." << std::endl;
	[[maybe_unused]] int waiting = _getch(); // 等待用户按下任意键后退出程序。 忽略返回值
	return 0;
}
	//// 获取文件库下的版本目录名
	//fs::path fileName = pathName.filename();

	//size_t lastDotPosition = fileName.string().find_last_of('.');// 查找最后一次出现点的位置
	//if (lastDotPosition != std::string::npos) {//如果找到了
	//	// 如果找到点，则将其替换为下划线
	//	fileName.string().replace(lastDotPosition, 1, "_");// 替换为_ (替换位置的索引, 在索引处替换的字符数,替换的字符)
	//}

	////把文件名中的点替换为下划线
	//fs::path repoPath = pathName.parent_path();
	//fs::path fileVerPath = repoPath / ".Rysigy" / "VERSIONS" / fileName;
	//if (!fs::exists(fileVerPath)) {
	//	FOut("[错误] 该文件未进行版本管理", fileVerPath.string(), RED);
	//	return 1;
	//}

	//std::vector<std::string> allVerName = GetAllFileVersions(pathName);
	//cout << "[信息] 当前文件的版本有:\n";
	//for (const auto& version : allVerName) {  // 使用范围-based for 循环
	//	std::cout << "Version: " << version << std::endl;
	//	// 或者在这里进行其他操作，比如将 version 写入文件等等
	//	cout << version << endl;
	//}
	//FOut("[输入] 查询的文件版本信息", "输入您需要查询的版本 (仅数字)\n>", CYAN);
	//string specVerNum;
	//cin >> specVerNum;
	//string specVerName = "_V" + specVerNum;
	//fs::path specFileVerPath = fileVerPath / specVerName; //获取到特定的文件版本路径

	////获取该文件的日志记录信息





//删除文件所有版本
int  file_operation::DeleteAllFileVersions(string pathName) {
	cout << "[执行] 删除该文件的所有版本\n";
	fs::path currentPath = pathName;//获取当前路径
	//查询当前是否有存储库
	fs::path repoPath = currentPath.parent_path() / ".Rysigy";

	//获取文件名
	std::string sourFileName = currentPath.filename().string(); // 获取文件名 filename.jpg
	size_t lastDotPosition = sourFileName.find_last_of('.');// 查找最后一次出现点的位置
	if (lastDotPosition != std::string::npos) {//如果找到了
		// 如果找到点，则将其替换为下划线
		sourFileName.replace(lastDotPosition, 1, "_");// 替换为_ (替换位置的索引, 在索引处替换的字符数,替换的字符)
		cout << "[信息] 文件名版本目录名是" << sourFileName << endl;
	}


	if (fs::exists(repoPath)) {
		// 获取版本路径
		fs::path verPath = repoPath / "VERSIONS" / sourFileName;
		if (fs::exists(verPath)) {
			for (const auto& entry : fs::directory_iterator(verPath)) { // 遍历版本目录下的所有文件
				if (entry.is_directory()) { // 判断是否为版本文件夹
					//获取文件夹名称
					fs::path folderPath = entry.path();
					std::string folderName = folderPath.filename().string();
					if (folderName.substr(0, 2) == "_V") { // 检查文件夹名是否以 "_V" 开头
						fs::remove_all(folderPath); // 删除文件夹及其内容（递归删除）
						std::cout << "已删除版本：" << folderName << std::endl;
					}
				}
				else {
					cout << "[忽略] 不是版本文件 " << entry.path().filename().string() << endl;
				}
			}
		}else {
			cout << "[错误] 当前文件没有进行版本管理, 无法获取版本信息" << endl;
		}
		if (fs::remove(verPath)) {
			cout << "删除版本目录 " << sourFileName << endl;
		}

	}else{
		cout << "[错误] 当前目录下没有存储库" << endl;
	}


	cout << "点击任意按键退出..." << std::endl;
	[[maybe_unused]] int waiting = _getch(); // 等待用户按下任意键后退出程序。 忽略返回值


	return 0;
}

//忽略此文件版本
int  file_operation::IgnoreThisFile(string pathName) {
	// 需要在数据库中创建一个配置文件, 记录忽略的文件
	cout << "未配置\n";

	cout << "点击任意按键退出..." << std::endl;
	[[maybe_unused]] int waiting = _getch(); // 等待用户按下任意键后退出程序。 忽略返回值


	return 0;
}


