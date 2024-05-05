#include "pch.h"
#include "folder_operation.h"


// ����̨�����ɫ
void COLOR_PRINT(const char* s, int color)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color);
    printf(s);
    //cout << s << endl;
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
}
//COLOR_PRINT("���Ǻ�ɫ\n", 4);
// 0 = ��ɫ
// 1 = ��ɫ
// 2 = ��ɫ
// 3 = ����ɫ
// 4 = ��ɫ
// 5 = ��ɫ
// 6 = ��ɫ
// 7 = ��ɫ
// 8 = ��ɫ
// 9 = ����ɫ
// 10 = ����ɫ
// 11 = ��ǳ��ɫ
// 12 = ����ɫ
// 13 = ����ɫ
// 14 = ����ɫ
// 15 = ����ɫ



// �ݹ鸴��Ŀ¼
// �ļ� to �ļ�����
void TextFileCopy(const std::string& srcF, const std::string& dstF) {
    //COLOR_PRINT("[ִ��] ����: �ļ� -> �ļ�\n", 2);
    std::ifstream input{ srcF };
    if (!input) {
        //std::cout << "[����] Դ�ļ�û���ҵ�" << std::endl;
        COLOR_PRINT("[����] Դ�ļ�û���ҵ�\n", 4);
        return;
    }

    std::filesystem::path destPath{ dstF };
    if (std::filesystem::exists(destPath)) {
        //std::cout << "File already exists, will override the exist file: " << dstF << std::endl;
        COLOR_PRINT("[��ʾ] �ļ��Ѵ���, �����Ǵ��ļ�\n", 6);
    }

    std::ofstream output{ dstF };
    if (!output) {
        //std::cout << "Could not open the dest file" << std::endl;
        COLOR_PRINT("[����] ���ܴ�Ŀ���ļ�\n", 4);
        return;
    }

    std::string line;
    while (std::getline(input, line)) {
        output << line << std::endl;
    }

    input.close();
    output.close();
}

// �������ļ�����
void BinaryFileCopy(const std::string& srcF, const std::string& dstF) {
    //COLOR_PRINT("[ִ��] ����: �ļ� -> �ļ�\n", 2);

    std::ifstream input{ srcF, std::ios::in | std::ios::binary };
    if (!input) {
        //std::cout << "Could not open the source file" << std::endl;
        COLOR_PRINT("[����] Դ�ļ�û���ҵ�\n", 4);

        return;
    }

    if (std::filesystem::exists(dstF)) {
        //std::cout << "File Already exists, will override the exists file: " << dstF << std::endl;
         COLOR_PRINT("[��ʾ] �ļ��Ѵ���, �����Ǵ��ļ�\n", 6);

    }

    std::ofstream output{ dstF, std::ios::out | std::ios::binary };
    if (!output) {
        //std::cout << "Could not open the destination file" << std::endl;
        COLOR_PRINT("[����] Ŀ���ļ��޷���", 4);
        return;
    }

    auto file_size = std::filesystem::file_size(srcF);
    const unsigned BufferSize = 512;
    char buffer[BufferSize]{};

    std::cout << "����: " << srcF << endl;
    // �ļ�����һ��Buffer��
    if (file_size < BufferSize) {
        if (!input.read(buffer, file_size)) {
            throw std::runtime_error("Error occurred during read operation");
        }
        if (!output.write(buffer, file_size)) {
            throw std::runtime_error("Error occurred during write operation");
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
                    "Error occurred during read operation");
            }

            if (!output.write(buffer, BufferSize)) {
                throw std::runtime_error(
                    "Error occurred during write operation");
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
                    "Error occurred during read operation");
            }
            if (!output.write(buffer, remaining)) {
                throw std::runtime_error(
                    "Error occurred during write operation");
            }
            std::cout << '.';
        }

        std::cout << "\n���! \n" << std::endl;
        //COLOR_PRINT("[�ɹ�] �ļ��Ѹ������\n", 2);

        input.close();
        output.close();
    }
}

// Ŀ¼ to Ŀ¼
int CopyDir(const std::string& srcDir, const std::string& dstDir) {
    //cout << "[ִ��] ����: Ŀ¼ -> Ŀ¼" << endl;
    // ʹ��std::filesystem::directory_iterator�� ����Ŀ¼�µ������ļ�
    std::filesystem::directory_iterator d_begin{ srcDir };
    std::filesystem::directory_iterator d_end;

    // begin != end ˵�������ļ�
    while (d_begin != d_end) {
        // Ŀ��path
        std::filesystem::path dstPath{ dstDir };
        // ��ȡԴ�ļ�����ƴ��Ŀ��path����
        auto dstDirName = d_begin->path().filename();
        dstPath /= dstDirName;
        // �����Ŀ¼
        if (d_begin->is_directory()) {
            // Ŀ¼������
            if (!std::filesystem::exists(dstPath)) {
                // �½�Ŀ¼
                std::filesystem::create_directory(dstPath);
            }
            // �ݹ鿽����Ŀ¼
            CopyDir(d_begin->path().string(), dstPath.string());//�����string()
        }
        else {
            // ������ģʽ�����ļ�
            BinaryFileCopy(d_begin->path().string(), dstPath.string());// �����string()
        }
        ++d_begin;
    }
    return 0;
}






//��ȡ·�����ļ����ļ��е���һ���汾, ������Ҫ��ѯ��·��
std::string NextVersion(fs::path pathName) {
    cout << "[ִ��] ��ѯ�汾����: " << pathName << endl;

    // ��ȡ�ж��ٸ��汾
    if (fs::exists(pathName)) {
        int nextNumber = 0; // ��һ���汾

        fs::path fileInDirPath = pathName.parent_path(); // ��ȡ�ļ�����Ŀ¼��·��
        cout << "[��Ϣ] �ļ�����Ŀ¼��·����:" << fileInDirPath << endl;

        std::string sourFileName = pathName.filename().string(); // ��ȡ�ļ��� filename.jpg
        size_t lastDotPosition = sourFileName.find_last_of('.');// �������һ�γ��ֵ��λ��
        if (lastDotPosition != std::string::npos) {//����ҵ���
            // ����ҵ��㣬�����滻Ϊ�»���
            sourFileName.replace(lastDotPosition, 1, "_");// �滻Ϊ_ (�滻λ�õ�����, ���������滻���ַ���,�滻���ַ�)
            cout << "[��Ϣ] �ļ����汾Ŀ¼����" << sourFileName << endl;
        }

        fs::path verPath = fileInDirPath / ".Rysigy" / "VERSIONS" / sourFileName;
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


// ---------------------------------------------------------------------------
// ��ȡ�ļ��еİ汾����
int folder_operation::queryFolderVersions(string pathName) {
    COLOR_PRINT("[ִ��] ��ѯ�ļ��еİ汾����\n", 2);
    fs::path currentPath = pathName;
    cout << "[��Ϣ] ��ǰ·����: " << currentPath << endl;
    // ���洢���Ƿ����
    fs::path repoPath = currentPath.parent_path() / ".Rysigy";// ��ȡ�洢��·��
    if (!fs::exists(repoPath)) {
        COLOR_PRINT("[����] �洢�ⲻ����, �����Ҽ��˵��д���\n", 4);
        cout << "������ⰴ���˳�...\n" << std::endl;
        [[maybe_unused]] int waiting = _getch(); // �ȴ��û�������������˳����� ���Է���ֵ
        return 1;
    }else{
        COLOR_PRINT("[�ɹ�] �ѻ�ȡ�洢��\n",2);
    }

    // ���汾���Ƿ����
    fs::path verPath = repoPath / "VERSIONS";//��ȡ�汾��·��
    if (!fs::exists(verPath)) {
        COLOR_PRINT("[����] �汾�ⲻ����\n", 4);
        cout << "������ⰴ���˳�...\n" << std::endl;
        [[maybe_unused]] int waiting = _getch(); // �ȴ��û�������������˳����� ���Է���ֵ
        return 1;
    }
    else {
        COLOR_PRINT("[�ɹ�] �ѻ�ȡ�汾��\n", 2);
    }

    //���Ŀ¼��Ŀ¼�Ƿ����
    fs::path dirNamePath = verPath / currentPath.filename();
    if (!fs::exists(dirNamePath)) {
        COLOR_PRINT("[����] ��ǰ�ļ���δ���а汾����\n", 4);
        cout << "������ⰴ���˳�...\n" << std::endl;
        [[maybe_unused]] int waiting = _getch(); // �ȴ��û�������������˳����� ���Է���ֵ
        return 1;
    }else{
        COLOR_PRINT("[�ɹ�] �ѻ�ȡ���ļ��а汾\n", 2);
    }

    // ��ѯ�汾����
    int verTotal = 0;// ��ʼ���汾����Ϊ 0
    for (const auto& entry : fs::directory_iterator(dirNamePath)) { // �����汾Ŀ¼�µ������ļ�
        if (entry.path().filename().string().substr(0, 2) == "_V" && fs::is_directory(entry)) {
            ++verTotal;
        }
    }
    cout << "[��Ϣ] ��ǰ�ļ����� " << verTotal << " ���汾" << endl;

    cout << "������ⰴ���˳�...\n" << std::endl;
    [[maybe_unused]] int waiting = _getch(); // �ȴ��û�������������˳����� ���Է���ֵ
	return 0;
}


// �����ļ��а汾
int folder_operation::createFolderVresions(string namePath) {
    COLOR_PRINT("[ִ��] �����ļ��а汾\n",2);
    // ��ȡ��ǰ�ļ�·��
    fs::path currentPath = namePath;
    cout << "[��Ϣ] ��ǰ·����: " << currentPath << endl;

    // ���洢���Ƿ����
    fs::path repoPath = currentPath.parent_path() / ".Rysigy";// ��ȡ�洢��·��
    if (!fs::exists(repoPath)) {
        COLOR_PRINT("[����] �洢�ⲻ����, ���ڿհ״������Ҽ��˵����д���\n", 4);
        cout << "������ⰴ���˳�...\n" << std::endl;
        [[maybe_unused]] int waiting = _getch(); // �ȴ��û�������������˳����� ���Է���ֵ
        return 1;
    }else{
        COLOR_PRINT("[�ɹ�] �ѻ�ȡ�洢��\n",2);
    }

    // ���汾���Ƿ����
    fs::path verPath = repoPath / "VERSIONS";//��ȡ�汾��·��
    if (!fs::exists(verPath)) {
        COLOR_PRINT("[����] �汾�ⲻ����, ���ڴ���...\n", 6);
        if (!fs::create_directories(verPath)) {
            COLOR_PRINT("[�ɹ�] �����ɹ�\n", 2);
        }
    }else {
        COLOR_PRINT("[�ɹ�] �ѻ�ȡ�汾��\n", 2);
    }

    //���Ŀ¼���汾Ŀ¼�Ƿ����  -> ����
    fs::path dirNamePath = verPath / currentPath.filename();
    if (!fs::exists(dirNamePath)) {
        COLOR_PRINT("[��ʾ] ��ǰ�ļ���δ���а汾����, ���ڴ�����ǰ�ļ��İ汾��\n", 6);
        if (!fs::create_directories(dirNamePath)) {
            COLOR_PRINT("[����] ��ǰ�ļ��汾�ⴴ��ʧ��\n", 4);
        }
    }else{
        COLOR_PRINT("[�ɹ�] �ѻ�ȡ���ļ��а汾\n", 2);
    }

    //��ȡ��һ���汾
    fs::path nextDirVersion = NextVersion(currentPath);
    fs::path verDirNamePath = dirNamePath / nextDirVersion;
    if (!fs::exists(verDirNamePath)) {
        if (fs::create_directories(verDirNamePath)) {
            COLOR_PRINT("[�ɹ�] �ļ��汾�����ɹ�\n", 2);
        }
    }else{
        COLOR_PRINT("[����] �ð汾�Ѵ���\n", 4);
    }
    if (0 == CopyDir(currentPath.string(), verDirNamePath.string())) {
         COLOR_PRINT("[�ɹ�] �ļ��Ѹ������\n", 2);
    }
    else
    {
        COLOR_PRINT("[ʧ��] �ļ����Ƴ��ֹ���\n", 2);

    }
    
    cout << "������ⰴ���˳�...\n" << std::endl;
    [[maybe_unused]] int waiting = _getch(); // �ȴ��û�������������˳����� ���Է���ֵ
   
	return 0;
}


int folder_operation::openFolderVersions(string) {

	return 0;
}


int folder_operation::ignoreThisFolder(string) {
	return 0;
}


int folder_operation::deleteFolderVersions(string) {
	return 0;
}


int folder_operation::deleteAllFolderVersions(string) {
	return 0;
}
