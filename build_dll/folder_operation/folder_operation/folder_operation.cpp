#include "pch.h"
#include "folder_operation.h"


// 控制台输出颜色
void COLOR_PRINT(const char* s, int color)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color);
    printf(s);
    //cout << s << endl;
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
}
//COLOR_PRINT("这是红色\n", 4);
// 0 = 黑色
// 1 = 蓝色
// 2 = 绿色
// 3 = 湖蓝色
// 4 = 红色
// 5 = 紫色
// 6 = 黄色
// 7 = 白色
// 8 = 灰色
// 9 = 淡蓝色
// 10 = 淡绿色
// 11 = 淡浅绿色
// 12 = 淡红色
// 13 = 淡紫色
// 14 = 淡黄色
// 15 = 亮白色



// 递归复制目录
// 文件 to 文件复制
void TextFileCopy(const std::string& srcF, const std::string& dstF) {
    //COLOR_PRINT("[执行] 复制: 文件 -> 文件\n", 2);
    std::ifstream input{ srcF };
    if (!input) {
        //std::cout << "[错误] 源文件没有找到" << std::endl;
        COLOR_PRINT("[错误] 源文件没有找到\n", 4);
        return;
    }

    std::filesystem::path destPath{ dstF };
    if (std::filesystem::exists(destPath)) {
        //std::cout << "File already exists, will override the exist file: " << dstF << std::endl;
        COLOR_PRINT("[提示] 文件已存在, 将覆盖此文件\n", 6);
    }

    std::ofstream output{ dstF };
    if (!output) {
        //std::cout << "Could not open the dest file" << std::endl;
        COLOR_PRINT("[错误] 不能打开目标文件\n", 4);
        return;
    }

    std::string line;
    while (std::getline(input, line)) {
        output << line << std::endl;
    }

    input.close();
    output.close();
}

// 二进制文件复制
void BinaryFileCopy(const std::string& srcF, const std::string& dstF) {
    //COLOR_PRINT("[执行] 复制: 文件 -> 文件\n", 2);

    std::ifstream input{ srcF, std::ios::in | std::ios::binary };
    if (!input) {
        //std::cout << "Could not open the source file" << std::endl;
        COLOR_PRINT("[错误] 源文件没有找到\n", 4);

        return;
    }

    if (std::filesystem::exists(dstF)) {
        //std::cout << "File Already exists, will override the exists file: " << dstF << std::endl;
         COLOR_PRINT("[提示] 文件已存在, 将覆盖此文件\n", 6);

    }

    std::ofstream output{ dstF, std::ios::out | std::ios::binary };
    if (!output) {
        //std::cout << "Could not open the destination file" << std::endl;
        COLOR_PRINT("[错误] 目标文件无法打开", 4);
        return;
    }

    auto file_size = std::filesystem::file_size(srcF);
    const unsigned BufferSize = 512;
    char buffer[BufferSize]{};

    std::cout << "复制: " << srcF << endl;
    // 文件不到一个Buffer大
    if (file_size < BufferSize) {
        if (!input.read(buffer, file_size)) {
            throw std::runtime_error("Error occurred during read operation");
        }
        if (!output.write(buffer, file_size)) {
            throw std::runtime_error("Error occurred during write operation");
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
                    "Error occurred during read operation");
            }

            if (!output.write(buffer, BufferSize)) {
                throw std::runtime_error(
                    "Error occurred during write operation");
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
                    "Error occurred during read operation");
            }
            if (!output.write(buffer, remaining)) {
                throw std::runtime_error(
                    "Error occurred during write operation");
            }
            std::cout << '.';
        }

        std::cout << "\n完成! \n" << std::endl;
        //COLOR_PRINT("[成功] 文件已复制完成\n", 2);

        input.close();
        output.close();
    }
}

// 目录 to 目录
int CopyDir(const std::string& srcDir, const std::string& dstDir) {
    //cout << "[执行] 复制: 目录 -> 目录" << endl;
    // 使用std::filesystem::directory_iterator类 迭代目录下的所有文件
    std::filesystem::directory_iterator d_begin{ srcDir };
    std::filesystem::directory_iterator d_end;

    // begin != end 说明还有文件
    while (d_begin != d_end) {
        // 目标path
        std::filesystem::path dstPath{ dstDir };
        // 获取源文件名，拼到目标path后面
        auto dstDirName = d_begin->path().filename();
        dstPath /= dstDirName;
        // 如果是目录
        if (d_begin->is_directory()) {
            // 目录不存在
            if (!std::filesystem::exists(dstPath)) {
                // 新建目录
                std::filesystem::create_directory(dstPath);
            }
            // 递归拷贝子目录
            CopyDir(d_begin->path().string(), dstPath.string());//添加了string()
        }
        else {
            // 二进制模式拷贝文件
            BinaryFileCopy(d_begin->path().string(), dstPath.string());// 添加了string()
        }
        ++d_begin;
    }
    return 0;
}






//获取路径下文件或文件夹的下一个版本, 传入需要查询的路径
std::string NextVersion(fs::path pathName) {
    cout << "[执行] 查询版本数量: " << pathName << endl;

    // 获取有多少个版本
    if (fs::exists(pathName)) {
        int nextNumber = 0; // 下一个版本

        fs::path fileInDirPath = pathName.parent_path(); // 获取文件所在目录的路径
        cout << "[信息] 文件所在目录的路径是:" << fileInDirPath << endl;

        std::string sourFileName = pathName.filename().string(); // 获取文件名 filename.jpg
        size_t lastDotPosition = sourFileName.find_last_of('.');// 查找最后一次出现点的位置
        if (lastDotPosition != std::string::npos) {//如果找到了
            // 如果找到点，则将其替换为下划线
            sourFileName.replace(lastDotPosition, 1, "_");// 替换为_ (替换位置的索引, 在索引处替换的字符数,替换的字符)
            cout << "[信息] 文件名版本目录名是" << sourFileName << endl;
        }

        fs::path verPath = fileInDirPath / ".Rysigy" / "VERSIONS" / sourFileName;
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


// ---------------------------------------------------------------------------
// 获取文件夹的版本数量
int folder_operation::queryFolderVersions(string pathName) {
    COLOR_PRINT("[执行] 查询文件夹的版本数量\n", 2);
    fs::path currentPath = pathName;
    cout << "[信息] 当前路径是: " << currentPath << endl;
    // 检查存储库是否存在
    fs::path repoPath = currentPath.parent_path() / ".Rysigy";// 获取存储库路径
    if (!fs::exists(repoPath)) {
        COLOR_PRINT("[错误] 存储库不存在, 可在右键菜单中创建\n", 4);
        cout << "点击任意按键退出...\n" << std::endl;
        [[maybe_unused]] int waiting = _getch(); // 等待用户按下任意键后退出程序。 忽略返回值
        return 1;
    }else{
        COLOR_PRINT("[成功] 已获取存储库\n",2);
    }

    // 检查版本库是否存在
    fs::path verPath = repoPath / "VERSIONS";//获取版本库路径
    if (!fs::exists(verPath)) {
        COLOR_PRINT("[错误] 版本库不存在\n", 4);
        cout << "点击任意按键退出...\n" << std::endl;
        [[maybe_unused]] int waiting = _getch(); // 等待用户按下任意键后退出程序。 忽略返回值
        return 1;
    }
    else {
        COLOR_PRINT("[成功] 已获取版本库\n", 2);
    }

    //检查目录名目录是否存在
    fs::path dirNamePath = verPath / currentPath.filename();
    if (!fs::exists(dirNamePath)) {
        COLOR_PRINT("[错误] 当前文件夹未进行版本管理\n", 4);
        cout << "点击任意按键退出...\n" << std::endl;
        [[maybe_unused]] int waiting = _getch(); // 等待用户按下任意键后退出程序。 忽略返回值
        return 1;
    }else{
        COLOR_PRINT("[成功] 已获取到文件夹版本\n", 2);
    }

    // 查询版本数量
    int verTotal = 0;// 初始化版本数量为 0
    for (const auto& entry : fs::directory_iterator(dirNamePath)) { // 遍历版本目录下的所有文件
        if (entry.path().filename().string().substr(0, 2) == "_V" && fs::is_directory(entry)) {
            ++verTotal;
        }
    }
    cout << "[信息] 当前文件夹有 " << verTotal << " 个版本" << endl;

    cout << "点击任意按键退出...\n" << std::endl;
    [[maybe_unused]] int waiting = _getch(); // 等待用户按下任意键后退出程序。 忽略返回值
	return 0;
}


// 创建文件夹版本
int folder_operation::createFolderVresions(string namePath) {
    COLOR_PRINT("[执行] 创建文件夹版本\n",2);
    // 获取当前文件路径
    fs::path currentPath = namePath;
    cout << "[信息] 当前路径是: " << currentPath << endl;

    // 检查存储库是否存在
    fs::path repoPath = currentPath.parent_path() / ".Rysigy";// 获取存储库路径
    if (!fs::exists(repoPath)) {
        COLOR_PRINT("[错误] 存储库不存在, 可在空白处唤出右键菜单进行创建\n", 4);
        cout << "点击任意按键退出...\n" << std::endl;
        [[maybe_unused]] int waiting = _getch(); // 等待用户按下任意键后退出程序。 忽略返回值
        return 1;
    }else{
        COLOR_PRINT("[成功] 已获取存储库\n",2);
    }

    // 检查版本库是否存在
    fs::path verPath = repoPath / "VERSIONS";//获取版本库路径
    if (!fs::exists(verPath)) {
        COLOR_PRINT("[警告] 版本库不存在, 正在创建...\n", 6);
        if (!fs::create_directories(verPath)) {
            COLOR_PRINT("[成功] 创建成功\n", 2);
        }
    }else {
        COLOR_PRINT("[成功] 已获取版本库\n", 2);
    }

    //检查目录名版本目录是否存在  -> 创建
    fs::path dirNamePath = verPath / currentPath.filename();
    if (!fs::exists(dirNamePath)) {
        COLOR_PRINT("[提示] 当前文件夹未进行版本管理, 正在创建当前文件的版本库\n", 6);
        if (!fs::create_directories(dirNamePath)) {
            COLOR_PRINT("[错误] 当前文件版本库创建失败\n", 4);
        }
    }else{
        COLOR_PRINT("[成功] 已获取到文件夹版本\n", 2);
    }

    //获取下一个版本
    fs::path nextDirVersion = NextVersion(currentPath);
    fs::path verDirNamePath = dirNamePath / nextDirVersion;
    if (!fs::exists(verDirNamePath)) {
        if (fs::create_directories(verDirNamePath)) {
            COLOR_PRINT("[成功] 文件版本创建成功\n", 2);
        }
    }else{
        COLOR_PRINT("[错误] 该版本已存在\n", 4);
    }
    if (0 == CopyDir(currentPath.string(), verDirNamePath.string())) {
         COLOR_PRINT("[成功] 文件已复制完成\n", 2);
    }
    else
    {
        COLOR_PRINT("[失败] 文件复制出现故障\n", 2);

    }
    
    cout << "点击任意按键退出...\n" << std::endl;
    [[maybe_unused]] int waiting = _getch(); // 等待用户按下任意键后退出程序。 忽略返回值
   
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
