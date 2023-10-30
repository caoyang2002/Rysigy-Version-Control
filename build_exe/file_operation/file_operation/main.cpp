#include "file_operation.h"


int main(int argc, char* argv[]) {
    std::string start_up = "\n"
        " ____                                             \n"
        "/\\  _`\\                   __                      \n"
        "\\ \\ \\L\\ \\  __  __    ____/\\_\\     __    __  __    \n"
        " \\ \\ ,  / /\\ \\/\\ \\  /',__\\/\\ \\  /'_ `\\ /\\ \\/\\ \\   \n"
        "  \\ \\ \\\\ \\\\ \\ \\_\\ \\/\\__, `\\ \\ \\/\\ \\L\\ \\\\ \\ \\_\\ \\  \n"
        "   \\ \\_\\ \\_\\/`____ \\/\\____/\\ \\_\\ \\____ \\\\/`____ \\ \n"
        "    \\/_/\\/ /`/___/> \\/___/  \\/_/\\/___L\\ \\`/___/> \\\n"
        "               /\\___/             /\\____/   /\\___/\n"
        "               \\/__/              \\_/__/    \\/__/ ";

    cout << start_up << endl;

    LPWSTR commandLine = GetCommandLineW();  // 获取完整命令行字符串
    int numArgs;
    LPWSTR* args = CommandLineToArgvW(commandLine, &numArgs);  // 解析命令行为参数数组
    if (numArgs >= 2) {
        std::wstring filepath(args[1]);  // 提取第一个参数（文件或目录路径）
        std::string filepath_gbk;  // 转换后的文件路径（GBK 编码）
        int gbk_len = WideCharToMultiByte(936, 0, filepath.c_str(), filepath.length(),
            nullptr, 0, nullptr, nullptr);
        if (gbk_len > 0) {
            filepath_gbk.resize(gbk_len);
            WideCharToMultiByte(936, 0, filepath.c_str(), filepath.length(),
                &filepath_gbk[0], gbk_len, nullptr, nullptr);
        }
        std::cout << "被点击的文件或目录路径：" << filepath_gbk << std::endl;
    }
    LocalFree(args);  // 释放内存










	file_operation FileOperation;
    
    //FileOperation.QueryFileVersions(argv[1]); // 查询文件版本

	//FileOperation.CreateFileVersions(argv[1]); // 创建文件版本

    // 获取指定的文件版本
    FileOperation.GetSpecilVerofFile(argv[1]);

	//FileOperation.DeleteAllFileVersions(argv[1]); // 删除所有文件版本
    
	//FileOperation.IgnoreThisFile(); // 忽略此文件
}
