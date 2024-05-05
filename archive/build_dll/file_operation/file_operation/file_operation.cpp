#include "pch.h"
#include "file_operation.h"

// ================================================================================================================================================
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


// ��ѯ�ļ��汾����,�������ļ�
std::vector<std::string> GetAllFileVersions(fs::path pathName){
	cout << "[ִ��] ��ȡ���ļ��İ汾����" << endl;
	int verTotal = 0; // �汾����
	fs::path currentPath = pathName;//��ȡ��ǰ·��
	//��ѯ��ǰ�Ƿ��д洢��
	fs::path repoPath = currentPath.parent_path() / ".Rysigy";

	//��ȡ�ļ���
	std::string sourFileName = currentPath.filename().string(); // ��ȡ�ļ��� filename.jpg
	size_t lastDotPosition = sourFileName.find_last_of('.');// �������һ�γ��ֵ��λ��
	if (lastDotPosition != std::string::npos) {//����ҵ���
		// ����ҵ��㣬�����滻Ϊ�»���
		sourFileName.replace(lastDotPosition, 1, "_");// �滻Ϊ_ (�滻λ�õ�����, ���������滻���ַ���,�滻���ַ�)
		cout << "[��Ϣ] �ļ����汾Ŀ¼����" << sourFileName << endl;
	}

	std::vector<std::string> verNames;// �汾������
	if (fs::exists(repoPath)) {
		// ��ȡ�汾·��
		fs::path verPath = repoPath / "VERSIONS" / sourFileName;
		if (fs::exists(verPath)) { // ��ѯ���ļ��Ƿ��а汾��
			for (const auto& entry : fs::directory_iterator(verPath)) { // �����汾Ŀ¼�µ������ļ�
				if (fs::is_directory(entry)) {
					std::string folder_name = entry.path().filename().string();
					if (folder_name.length() > 2 && folder_name.substr(0, 2) == "_V")
					{
						verNames.push_back(folder_name);//װ������
						++verTotal;
					}
				}
			}
			cout << "[��Ϣ] ���ļ����� " << verTotal << " ���汾\n";

		}else{
			cout << "[����] ��ǰ�ļ�û�н��а汾����, �޷���ȡ�汾��Ϣ" << endl;
		}
	}else{
		cout << "[����] ��ǰĿ¼��û�д洢��" << endl;
	}

	return verNames;

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




// ��ʽ�����
// [����] ������, �������Դ, 4(��ɫ)
int FOut(std::string information, std::string details_path, Color color) {

	// ����̨���
	string logCurrent = information + "\t" + details_path;
	COLOR_PRINT(logCurrent.c_str(), color);
	
	return 0;  
}

// �������־�ļ�
int FOuttoLog(std::string information, std::string details_path, Color color, fs::path logFilePath) {

	// ����̨���
	string logCurrent = information + "\t" + details_path;
	COLOR_PRINT(logCurrent.c_str(), color);

	// �����־�ļ��Ƿ����
	if (!fs::exists(logFilePath)) {
		COLOR_PRINT("[����] ��־�ļ�������", RED);
	}
	//��־���
	std::fstream outputLogFile(logFilePath);
	if (outputLogFile.is_open()) {
		string currentTime = getCurrentTime();
		string logContent = currentTime + "\t" + information + "\t" + details_path + "\t";
		outputLogFile << logContent;
		outputLogFile.close();
		std::cout << "��־�ѳɹ�д���ļ���" << logFilePath << std::endl;
	}
	else {
		std::cerr << "�޷����ļ���" << logFilePath << std::endl;
	}

	return 0; 
}




// ����־�ļ�������Ԫ��Ϣ
int enterMetaInfotoFile(fs::path pathName) {
	FOut("[ִ��] ���������Ϣ���ļ���\n", "", WHITE);

}

// ��ȡ�ļ���־��Ϣ
fs::path getFileLog(fs::path pathName) {
	FOut("[ִ��] ��ȡ�ļ��汾��Ϣ", pathName.string(), WHITE);
	cout << endl;

	// ��ȡ�ļ����µİ汾Ŀ¼��
	string fileName = pathName.filename().string();
	string repcFileName = fileName;

	size_t lastDotPosition = fileName.find_last_of('.');// �������һ�γ��ֵ��λ��
	if (lastDotPosition != std::string::npos) {//����ҵ���
		// ����ҵ��㣬�����滻Ϊ�»���
		repcFileName.replace(lastDotPosition, 1, "_");// �滻Ϊ_ (�滻λ�õ�����, ���������滻���ַ���,�滻���ַ�)
	}
	cout << "ԭʼ�ļ�����: " << fileName << endl;
	cout << "�滻����ļ�����: " << repcFileName << endl;

	fs::path repoPath = pathName.parent_path();
	fs::path fileVerPath = repoPath / ".Rysigy" / "VERSIONS" / repcFileName;
	if (!fs::exists(fileVerPath)) {
		FOut("[����] ���ļ�δ���а汾����\n", fileVerPath.string(), RED);
		return "�ļ�δ���а汾��\n��";
	}

	std::vector<std::string> allVerName = GetAllFileVersions(pathName);
	cout << "[��Ϣ] ��ǰ�ļ��İ汾��:\n";
	for (const auto& version : allVerName) {  // ʹ�÷�Χ-based for ѭ��
		//std::cout << "Version: " << version << std::endl;
		// ��������������������������罫 version д���ļ��ȵ�
		cout << "  \t" << version << endl;
	}
	FOut("[����] ��ѯ�ļ��İ汾��Ϣ\n", "��������Ҫ��ѯ�İ汾 (������)\n>>>", CYAN);
	string specVerNum;
	cin >> specVerNum;
	string specVerName = "_V" + specVerNum;
	fs::path specFileVerPath = fileVerPath / specVerName; //��ȡ���ض����ļ��汾·��

	// �ļ��İ汾��־��
	string fileLogName;
	std::string newExtension = ".log";
	size_t dotIndex = fileName.find_last_of('.');
	//cout << "�����ļ�����:" << fileName << endl;
	if (dotIndex != std::string::npos) {
		std::string baseName = fileName.substr(0, dotIndex); // �ѵ�ǰ�����Ϊ�����ļ���
		fileLogName = baseName + newExtension; // �µ���չ��
	}
	else {
		FOut("[����] û���ҵ���׺��", fileLogName, RED);
	}

	fs::path specFileVerLogPath = specFileVerPath / fileLogName;
	if (!fs::exists(specFileVerLogPath)) {
		FOut("[����] �ļ��汾��־������", specFileVerPath.string(), RED);
		return "�ļ��汾��־������";
	}

	//��ȡ�ļ��汾��־����
	std::ifstream logFile(specFileVerLogPath);  // ���ļ�

	if (logFile.is_open()) {
		std::string line;
		while (std::getline(logFile, line)) {  // ���ж�ȡ�ļ�����
			//std::cout << line << std::endl;  // ���ÿһ�е�����
			FOut("|", line + "\t|\n", LIGHT_CYAN);
		}
		logFile.close();  // �ر��ļ�
	}
	else {
		FOut("[����] ��־�ļ���ʧ��", "", RED);
		//std::cout << "�ļ���ʧ��" << std::endl;
	}
	return specFileVerLogPath;
}



// --------------------------------------------------------------------------------------


// �������ļ����� ����Դ�ļ�·����Ŀ���ļ�·��
void BinaryFileCopy(const std::string& srcFile, const std::string& dstFile) {
	FOut("[ִ��] �����ļ�", "", WHITE);
	std::ifstream input{ srcFile, std::ios::in | std::ios::binary };
	if (!input) {
		std::cout << "[ʧ��] �޷���Դ�ļ�" << std::endl;
		return;
	}

	if (std::filesystem::exists(dstFile)) {
		std::cout << "[��ʾ] �ļ��ѱ�����, �����Ƶ��Ѵ������ļ�\n"
			<< "���Ƶ�" << dstFile << std::endl;
	}

	std::ofstream output{ dstFile, std::ios::out | std::ios::binary };
	if (!output) {
		std::cout << "[ʧ��] �޷���Ŀ���ļ�" << std::endl;
		return;
	}

	auto file_size = std::filesystem::file_size(srcFile);
	const unsigned BufferSize = 512;
	char buffer[BufferSize]{};

	std::cout << "�����Ƶ��ļ�: " << srcFile << endl;
	// �ļ�����һ��Buffer��
	if (file_size < BufferSize) {
		if (!input.read(buffer, file_size)) {
			throw std::runtime_error("[����] ��ȡ�ļ�ʱ��������");
		}
		if (!output.write(buffer, file_size)) {
			throw std::runtime_error("[����] д���ļ�ʱ��������");
		}
	}
	else {
		// ���ļ���Ϊ���������������ӡ���ȣ����ȳ�10����Ϊ�˲��ý������������0��һֱ����ӡ
		auto chunks = file_size / BufferSize;
		// ������int����Ϊ ʣ���СС�� 512����int�Ϳ����ˣ�����Ҫ std::size_t
		int remaining = file_size % BufferSize;
		int progress{}, oldProgress{};
		for (int i = 0; i < chunks; ++i) {
			if (!input.read(buffer, BufferSize)) {
				throw std::runtime_error(
					"[����] ��ȡ�ļ�ʱ��������");
			}

			if (!output.write(buffer, BufferSize)) {
				throw std::runtime_error(
					"[����] д���ļ�ʱ��������");
			}

			// ��ת�������ȫ�㣬��ת���ͱ��ڱȽ�
			// ����ת������ʹ�� static_cast����Ҫֱ��ǿת
			// ��Ϊstatic_cast�������ͼ��
			progress = static_cast<int>((10 * static_cast<float>(i) / chunks));
			if (progress != oldProgress) {
				std::cout << '.';
			}
			oldProgress = progress;
		}

		// ��һ�εĶ�ȡ����С�� BufferSize
		// ����buffer ������ܻ����ϴζ�ȡ�Ĳд����ݣ�
		// ������Ҫʹ��memset���
		// �Ƚϵײ��C����������û�취����Ϊ��д�����Ʊ��밴char* [byte]����
		memset(buffer, '\0', BufferSize);

		if (remaining > 0) {
			if (!input.read(buffer, remaining)) {
				throw std::runtime_error(
					"[����] ��ȡ�ļ�ʱ��������");
			}
			if (!output.write(buffer, remaining)) {
				throw std::runtime_error(
					"[����] д���ļ�ʱ��������");
			}
			std::cout << '.';
		}

		FOut("\n[�ɹ�] �������", "", GREEN);
		//std::cout << "\n[�ɹ�] �������! " << std::endl;

		input.close();
		output.close();
	}
}



//��ȡ·�����ļ����ļ��е���һ���汾, ������Ҫ��ѯ��·��
std::string NextVersion(fs::path pathName) {
	cout << "[ִ��] ��ѯ�汾����: " << pathName << endl;

	string time = getCurrentTime();//��ȡ��ǰʱ��

	// ��ȡ�ж��ٸ��汾
	if (fs::exists(pathName)) {
		int nextNumber = 0; // ��һ���汾

		fs::path fileInDirPath = pathName.parent_path() ; // ��ȡ�ļ�����Ŀ¼��·��
		cout << "[��Ϣ] �ļ�����Ŀ¼��·����:" << fileInDirPath << endl;

		std::string sourFileName = pathName.filename().string(); // ��ȡ�ļ��� filename.jpg
		size_t lastDotPosition = sourFileName.find_last_of('.');// �������һ�γ��ֵ��λ��
		if (lastDotPosition != std::string::npos) {//����ҵ���
			// ����ҵ��㣬�����滻Ϊ�»���
			sourFileName.replace(lastDotPosition, 1, "_");// �滻Ϊ_ (�滻λ�õ�����, ���������滻���ַ���,�滻���ַ�)
			cout << "[��Ϣ] �ļ����汾Ŀ¼����" << sourFileName << endl;
		}

		fs::path verPath = fileInDirPath / ".Rysigy" / "VERSIONS"/ sourFileName;
		cout << "[��Ϣ] �汾·����: " << verPath << endl;

		if (!fs::exists(verPath) || fs::is_empty(verPath)) {
			// �汾·����û���κ�Ŀ¼
			cout << "[����] ���ļ���δ���а汾����" << endl;
			cout << "[��Ϣ] ������һ���汾 _V1" << endl;
			return "_V1";  // ����Ĭ�ϵİ汾��
		}

		// ��ȡ��һ���汾
		for (const auto& entry : fs::directory_iterator(verPath)) {// ��ȡ�汾·���µ����а汾
			// ����һ��������ʽ��ƥ����"_V"��"_v"��ͷ��Ŀ¼���ƣ����һ����������
			std::regex pattern(R"([_vV](\d+))");	//[_vV]����ʾƥ�䵥���ַ���������Сд��ĸ "v"����д��ĸ "V" ���»��� "_" �е��κ�һ����  
			// (\\d+)��ʹ�����ű�ʾһ�������飬��ʾƥ��һ���������֡�
			// \\d ��������ʽ��ƥ�����ֵķ�ʽ��+ ��ʾƥ��һ���������֡�

			// �����ڵ�ǰĿ¼�����в���ƥ��Ĳ���
			std::smatch match;
			fs::path itemPath = entry;
			std::string fileNameDirName = itemPath.filename().string();
			cout << "[��Ϣ] ���ļ����а汾: " << fileNameDirName << endl;
			if (std::regex_search(fileNameDirName, match, pattern)) {
				// ��ȡƥ������ֲ��ֲ�ת��Ϊ����
				int currentNumber = std::stoi(match[1].str());

				// ������һ��Ŀ¼���ƣ����ּ�һ��
				nextNumber = currentNumber + 1;
				//cout << "[��Ϣ] ��һ���汾Ϊ _V" << nextNumber << endl;
				//return "_" + std::to_string(nextNumber);
			}
			else {
				// ���û��ƥ��Ĳ��֣��򷵻�Ĭ�Ϸ�����һ���汾�� _V1
				cout << "[��Ϣ] ��һ���汾Ϊ _V1" << endl;
				return "_V1";
			}
		}
		string nextVer = "_V" + std::to_string(nextNumber);
		cout << "��һ���汾Ϊ: " << nextVer << endl;
		return nextVer;
	}
	else {
		cout << "[����] ��ǰ·��������" << pathName << endl;
	}
	return "_V1";

}


// ================================================================================================================================================

//-------------------------------------------------------------------------------------------------------------
// ��ѯ���ļ������а汾
int  file_operation::QueryFileVersions(string pathName) {
	cout << "[ִ��] ��ȡ���ļ��İ汾����" << endl;
	int verTotal = 0; // �汾����
	fs::path currentPath = pathName;//��ȡ��ǰ·��
	//��ѯ��ǰ�Ƿ��д洢��
	fs::path repoPath = currentPath.parent_path() / ".Rysigy";

	//��ȡ�ļ���
	std::string sourFileName = currentPath.filename().string(); // ��ȡ�ļ��� filename.jpg
	size_t lastDotPosition = sourFileName.find_last_of('.');// �������һ�γ��ֵ��λ��
	if (lastDotPosition != std::string::npos) {//����ҵ���
		// ����ҵ��㣬�����滻Ϊ�»���
		sourFileName.replace(lastDotPosition, 1, "_");// �滻Ϊ_ (�滻λ�õ�����, ���������滻���ַ���,�滻���ַ�)
		cout << "[��Ϣ] �ļ����汾Ŀ¼����: " << sourFileName << endl;
	}

	if (fs::exists(repoPath)) {
		// ��ȡ�汾·��
		fs::path verPath = repoPath / "VERSIONS" / sourFileName;
		if (fs::exists(verPath)) { 
			for (const auto& entry : fs::directory_iterator(verPath)) { // �����汾Ŀ¼�µ������ļ�
				if (entry.path().filename().string().substr(0, 2) == "_V" && fs::is_directory(entry)) {
					++verTotal;
				}
			}
			cout << "[��Ϣ] ���ļ����� " << verTotal << " ���汾\n";

		}else{
			cout << "[����] ��ǰ�ļ�û�н��а汾����, �޷���ȡ�汾��Ϣ" << endl;
		}
	}else{
		cout << "[����] ��ǰĿ¼��û�д洢��" << endl;
	}
	cout << "������ⰴ���˳�..." << std::endl;
	[[maybe_unused]] int waiting = _getch(); // �ȴ��û�������������˳����� ���Է���ֵ
	return 0;
}




//�����ļ��汾
int file_operation::CreateFileVersions(string filePath) {

// ��ȡ��ǰ�ļ�·��, �����ļ���Ŀ¼, ��ȡ��һ���汾, �����汾Ŀ¼, �����ļ�
	fs::path currentPath = filePath; //fs::current_path();
	cout << "��ǰ�ļ���: " << currentPath << endl;
	string nextFileVersion = NextVersion(currentPath); // ��ȡ���ļ�����һ���汾

	std::string sourFileName = currentPath.filename().string(); // ��ȡ�ļ��� filename.jpg
	size_t lastDotPosition = sourFileName.find_last_of('.');// �������һ�γ��ֵ��λ��

	if (lastDotPosition != std::string::npos) {//����ҵ���
		// ����ҵ��㣬�����滻Ϊ�»���
		sourFileName.replace(lastDotPosition, 1, "_");// �滻Ϊ_ (�滻λ�õ�����, ���������滻���ַ���,�滻���ַ�)
		cout << "[��Ϣ] �ļ���Ŀ¼����: " << sourFileName << endl;
	}
	// �����ļ���Ŀ¼
	fs::path fileNameDirPath = currentPath.parent_path() / ".Rysigy" / "VERSIONS" / sourFileName; // "/"���ӵ�����,����ı�����   "/=" ��ı����� 
	cout << "[��Ϣ] �ļ���·����" << fileNameDirPath << endl;

	// ����Ŀ¼
	//std::filesystem::path directoryPath(fileNameDirPath);//�ļ���Ŀ¼·��

	if (!fs::exists(fileNameDirPath)) {  // ����ļ���Ŀ¼������
		if (!fs::create_directories(fileNameDirPath)) {  // �����ļ���Ŀ¼���������
			std::cout << "[ʧ��] �ļ���Ŀ¼����ʧ��" << std::endl;
			return 1;
		}
		std::cout << "[�ɹ�] �ļ���Ŀ¼�����ɹ�" << std::endl;
	}

	fs::path verPath = fileNameDirPath / nextFileVersion; // �汾Ŀ¼·��
	if (!fs::create_directory(verPath)) { // �����汾Ŀ¼���������
		std::cout << "[ʧ��] �汾�ļ��д���ʧ��" << std::endl;
		return 1;
	}
	std::cout << "[�ɹ�] �汾�ļ��д����ɹ�" << std::endl;

	// ����һ��Ŀ���ļ�
	fs::path destFilePath = verPath / currentPath.filename();
	BinaryFileCopy(currentPath.string(), destFilePath.string()); // �����ļ�

	// ������־�ļ�
	std::string newExtension = ".log";  // Ҫ���ĵ��º�׺
	fs::path logFilePath = destFilePath.replace_extension(newExtension);
	std::ofstream logFile(logFilePath);  // �����ļ�
	if (logFile) {  // ����Ƿ�ɹ���


		logFile << getCurrentTime(); //����־�����봴��ʱ����Ϣ

		// ѯ���û��Ƿ���Ҫ���������Ϣ
		int confirmChange;
		FOut("[ѯ��] ���Ƿ���Ҫ����汾������Ϣ?\n", "\n- 0 \t��Ҫ\n- 1 \t����Ҫ", LIGHT_YELLOW);
		cout << ">>>";
		cin >> confirmChange;
		if (0 == confirmChange) {
			FOut("[����] �������������, ���س�����������", "", LIGHT_YELLOW);
			string logContent;
			logContent = "\n" + logContent;
			cin >> logContent;
			logFile << logContent;
		
		}
		else if(1 == confirmChange) {
			FOut("[ȡ��] ��ȡ������", "", WHITE);
		}
		logFile.close();  // �ر��ļ�
		std::cout << "[�ɹ�] ��־�ļ������ɹ���д������" << std::endl;
	}
	else {
		std::cout << "[ʧ��] �޷�������־�ļ�" << std::endl;
	}
	std::cout << "[�ɹ�] �ļ��汾�����ɹ�" << std::endl;


	cout << "������ⰴ���˳�..." << std::endl;
	[[maybe_unused]] int waiting = _getch(); // �ȴ��û�������������˳����� ���Է���ֵ

	return 0;
}

// --------------------------------------------------- ��ȡ�ļ�ָ���汾
int file_operation::GetSpecilVerofFile(fs::path pathName) {
	FOut("[ִ��] ��ȡ�ļ�ָ���汾", pathName.filename().string(), WHITE);
	cout << endl;
	int inputSelect; // ѡ��
	fs::path logFilePath;//��־�ļ�·��
	do {
		logFilePath = getFileLog(pathName);//��ȡ�ļ���־��Ϣ
		cout << "�ļ�·��: " << logFilePath.string() << endl;
		cout << "����: \n- 0       \tѡ��鿴�汾��Ϣ\n- ��������\t��ȡ���ֶ�Ӧ�İ汾�ļ�" << endl;
		cin >> inputSelect;// ��ȡ����
	} while (0 == inputSelect);

	//FOut("[����] ������汾ĩβ������", "\n>", CYAN);
	string inputVerNum = std::to_string(inputSelect);

	//
	
	fs::path fileVerNamePath = logFilePath.parent_path() / pathName.filename();
	if (!fs::exists(fileVerNamePath)) {
		FOut("[����] �ļ�������", fileVerNamePath.string(), RED);
	}
	
	// �Ѱ汾�ļ����Ƶ���ǰĿ¼
	std::string origFilename = pathName.filename().string(); // ԭʼ�ļ���
	// �汾�ļ�·��
	fs::path verFilePath = logFilePath.parent_path() / origFilename;
	//��ǰ�汾��
	string fileVerName = logFilePath.parent_path().filename().string();
	//���ļ���
	std::string newFileName = origFilename.substr(0, origFilename.find_last_of('.')) + fileVerName  + origFilename.substr(origFilename.find_last_of('.'));
	fs::path newFilePath = pathName.parent_path() / newFileName;

	if (fs::exists(newFilePath)) {
		FOut("[����] �汾�ļ��Ѵ���:\n- 0       \t�Զ�����������\n- �������� \t����", "\n>>>",RED);
		int inputEx;
		cin >> inputEx;
		if (0 == inputEx) {
			reInput:
			cout << "---- ��������Ϊ���ֵ��ַ�\n";
			string str;
			cout << ">>>";
			cin >> str;
			std::string specialCharacters = R"(\/*|?<>"*)"; // �����ַ��б�
			size_t found = str.find_first_of(specialCharacters);
			if (found != std::string::npos) {
				cout << "---- ������ַ������԰��� \\/|*\"<>? ���޸�" << endl;
				goto reInput;
			}

			newFileName = origFilename.substr(0, origFilename.find_last_of('.')) + fileVerName +  str + origFilename.substr(origFilename.find_last_of('.'));
			newFilePath = pathName.parent_path() / newFileName;
		}
	}
	
	std::ofstream file(newFilePath);  // ���ļ� example.txt (�����������ᴴ��)
	cout << "[��Ϣ] �����İ汾�ļ�·��Ϊ: " << newFilePath << endl;

	if (file.is_open()) {
		//���ļ����Ƶ���ǰ·����
		BinaryFileCopy(verFilePath.string(), newFilePath.string()); // �����ļ�
		file.close();  // �ر��ļ�
		//std::cout << "�ѵ����汾�ļ�" << std::endl;
		FOut("[�ɹ�] �汾�ļ��ѵ���", newFileName, GREEN);
		cout << endl;
	}
	else {
		//std::cout << "�޷������汾�ļ�" << std::endl;
		FOut("[����] �޷������汾�ļ�", newFileName, RED);
		cout << endl;
	}
	cout << "������ⰴ���˳�..." << std::endl;
	[[maybe_unused]] int waiting = _getch(); // �ȴ��û�������������˳����� ���Է���ֵ
	return 0;
}
	//// ��ȡ�ļ����µİ汾Ŀ¼��
	//fs::path fileName = pathName.filename();

	//size_t lastDotPosition = fileName.string().find_last_of('.');// �������һ�γ��ֵ��λ��
	//if (lastDotPosition != std::string::npos) {//����ҵ���
	//	// ����ҵ��㣬�����滻Ϊ�»���
	//	fileName.string().replace(lastDotPosition, 1, "_");// �滻Ϊ_ (�滻λ�õ�����, ���������滻���ַ���,�滻���ַ�)
	//}

	////���ļ����еĵ��滻Ϊ�»���
	//fs::path repoPath = pathName.parent_path();
	//fs::path fileVerPath = repoPath / ".Rysigy" / "VERSIONS" / fileName;
	//if (!fs::exists(fileVerPath)) {
	//	FOut("[����] ���ļ�δ���а汾����", fileVerPath.string(), RED);
	//	return 1;
	//}

	//std::vector<std::string> allVerName = GetAllFileVersions(pathName);
	//cout << "[��Ϣ] ��ǰ�ļ��İ汾��:\n";
	//for (const auto& version : allVerName) {  // ʹ�÷�Χ-based for ѭ��
	//	std::cout << "Version: " << version << std::endl;
	//	// ��������������������������罫 version д���ļ��ȵ�
	//	cout << version << endl;
	//}
	//FOut("[����] ��ѯ���ļ��汾��Ϣ", "��������Ҫ��ѯ�İ汾 (������)\n>", CYAN);
	//string specVerNum;
	//cin >> specVerNum;
	//string specVerName = "_V" + specVerNum;
	//fs::path specFileVerPath = fileVerPath / specVerName; //��ȡ���ض����ļ��汾·��

	////��ȡ���ļ�����־��¼��Ϣ





//ɾ���ļ����а汾
int  file_operation::DeleteAllFileVersions(string pathName) {
	cout << "[ִ��] ɾ�����ļ������а汾\n";
	fs::path currentPath = pathName;//��ȡ��ǰ·��
	//��ѯ��ǰ�Ƿ��д洢��
	fs::path repoPath = currentPath.parent_path() / ".Rysigy";

	//��ȡ�ļ���
	std::string sourFileName = currentPath.filename().string(); // ��ȡ�ļ��� filename.jpg
	size_t lastDotPosition = sourFileName.find_last_of('.');// �������һ�γ��ֵ��λ��
	if (lastDotPosition != std::string::npos) {//����ҵ���
		// ����ҵ��㣬�����滻Ϊ�»���
		sourFileName.replace(lastDotPosition, 1, "_");// �滻Ϊ_ (�滻λ�õ�����, ���������滻���ַ���,�滻���ַ�)
		cout << "[��Ϣ] �ļ����汾Ŀ¼����" << sourFileName << endl;
	}


	if (fs::exists(repoPath)) {
		// ��ȡ�汾·��
		fs::path verPath = repoPath / "VERSIONS" / sourFileName;
		if (fs::exists(verPath)) {
			for (const auto& entry : fs::directory_iterator(verPath)) { // �����汾Ŀ¼�µ������ļ�
				if (entry.is_directory()) { // �ж��Ƿ�Ϊ�汾�ļ���
					//��ȡ�ļ�������
					fs::path folderPath = entry.path();
					std::string folderName = folderPath.filename().string();
					if (folderName.substr(0, 2) == "_V") { // ����ļ������Ƿ��� "_V" ��ͷ
						fs::remove_all(folderPath); // ɾ���ļ��м������ݣ��ݹ�ɾ����
						std::cout << "��ɾ���汾��" << folderName << std::endl;
					}
				}
				else {
					cout << "[����] ���ǰ汾�ļ� " << entry.path().filename().string() << endl;
				}
			}
		}else {
			cout << "[����] ��ǰ�ļ�û�н��а汾����, �޷���ȡ�汾��Ϣ" << endl;
		}
		if (fs::remove(verPath)) {
			cout << "ɾ���汾Ŀ¼ " << sourFileName << endl;
		}

	}else{
		cout << "[����] ��ǰĿ¼��û�д洢��" << endl;
	}


	cout << "������ⰴ���˳�..." << std::endl;
	[[maybe_unused]] int waiting = _getch(); // �ȴ��û�������������˳����� ���Է���ֵ


	return 0;
}

//���Դ��ļ��汾
int  file_operation::IgnoreThisFile(string pathName) {
	// ��Ҫ�����ݿ��д���һ�������ļ�, ��¼���Ե��ļ�
	cout << "δ����\n";

	cout << "������ⰴ���˳�..." << std::endl;
	[[maybe_unused]] int waiting = _getch(); // �ȴ��û�������������˳����� ���Է���ֵ


	return 0;
}


