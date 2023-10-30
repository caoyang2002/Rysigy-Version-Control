#include "folder_operation.h"

int main(int argc, char* argv[]) {
    LPWSTR commandLine = GetCommandLineW();  // 获取完整命令行字符串
    int numArgs;
    LPWSTR* args = CommandLineToArgvW(commandLine, &numArgs);  // 解析命令行为参数数组
    if (numArgs >= 2) {
        std::wstring filepath(args[1]);  // 提取第一个参数（文件或目录路径）
        std::string filepath_mb;  // 转换后的文件路径（UTF-8 编码）
        int mb_len = WideCharToMultiByte(CP_UTF8, 0, filepath.c_str(), filepath.length(),
            nullptr, 0, nullptr, nullptr);
        if (mb_len > 0) {
            filepath_mb.resize(mb_len);
            WideCharToMultiByte(CP_UTF8, 0, filepath.c_str(), filepath.length(),
                &filepath_mb[0], mb_len, nullptr, nullptr);
        }
        std::cout << "被点击的文件或目录路径：" << filepath_mb << std::endl;
    }
    LocalFree(args);  // 释放内存


    //
    folder_operation FolderOperation;

    //FolderOperation.queryFolderVersions(argv[1]);
    FolderOperation.createFolderVresions(argv[1]);
    //FolderOperation.openFolderVersions(argv[1]);
    //FolderOperation.ignoreThisFolder(argv[1]);
    //FolderOperation.deleteFolderVersions(argv[1]);
    //FolderOperation.deleteAllFolderVersions(argv[1]);





}
