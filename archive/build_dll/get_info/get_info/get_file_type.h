#pragma once
#include <windows.h>
#include <fstream>


// 根据文件头标识获取文件类型
std::wstring GetFileType(const std::wstring& filename) {
    std::ifstream file(filename, std::ios_base::binary);
    char buffer[16];
    file.read(buffer, sizeof(buffer));

    if (file.gcount() >= 2 && buffer[0] == 'M' && buffer[1] == 'Z') {
        return L"可执行文件";
    }
    // 判断是否为JPEG图片
    else if (file.gcount() >= 4 && buffer[0] == '\xFF' && buffer[1] == '\xD8'
        && buffer[2] == '\xFF' && (buffer[3] == '\xE0' || buffer[3] == '\xE1')) {
        return L"JPEG 图片";
    }
    // 判断是否为PNG图片
    else if (file.gcount() >= 4 && buffer[0] == '\x89' && buffer[1] == 'P'
        && buffer[2] == 'N' && buffer[3] == 'G') {
        return L"PNG 图片";
    }
    // 判断是否为GIF87a图片
    else if (file.gcount() >= 6 && buffer[0] == 'G' && buffer[1] == 'I'
        && buffer[2] == 'F' && buffer[3] == '8' && buffer[4] == '7' && buffer[5] == 'a') {
        return L"GIF87a 图片";
    }
    // 判断是否为GIF89a图片
    else if (file.gcount() >= 6 && buffer[0] == 'G' && buffer[1] == 'I'
        && buffer[2] == 'F' && buffer[3] == '8' && buffer[4] == '9' && buffer[5] == 'a') {
        return L"GIF89a 图片";
    }
    // 判断是否为ZIP压缩文件
    else if (file.gcount() >= 4 && buffer[0] == '\x50' && buffer[1] == 'K'
        && buffer[2] == '\x03' && buffer[3] == '\x04') {
        return L"ZIP 压缩文件";
    }
    // 判断是否为RAR压缩文件
    else if (file.gcount() >= 6 && buffer[0] == 'R' && buffer[1] == 'a'
        && buffer[2] == 'r' && buffer[3] == '!' && buffer[4] == '\x1A' && buffer[5] == '\x7A') {
        return L"RAR 压缩文件";
    }
    // 判断是否为Outlook邮件文件
    else if (file.gcount() >= 16 && buffer[0] == 'M' && buffer[1] == 'S'
        && buffer[2] == 'C' && buffer[3] == 'F' && buffer[8] == '\x05' && buffer[9] == '\x06') {
        return L"Outlook 邮件文件";
    }
    // 判断是否为Java JAR文件
    else if (file.gcount() >= 4 && buffer[0] == 'P' && buffer[1] == 'K'
        && buffer[2] == '\x03' && buffer[3] == '\x04') {
        return L"Java JAR 文件";
    }
    // 判断是否为Bad Apple!!视频文件
    else if (file.gcount() >= 4 && buffer[0] == 'B' && buffer[1] == 'A'
        && buffer[2] == 'D' && buffer[3] == 'E') {
        return L"Bad Apple!! 视频";
    }
    // 判断是否为PDF文件
    else if (file.gcount() >= 5 && buffer[0] == '%' && buffer[1] == 'P'
        && buffer[2] == 'D' && buffer[3] == 'F' && buffer[4] == '-') {
        return L"PDF 文件";
    }
    // 判断是否为Microsoft Word文档
    else if (file.gcount() >= 8 && buffer[0] == '\xD0' && buffer[1] == '\xCF'
        && buffer[2] == '\x11' && buffer[3] == '\xE0' && buffer[4] == '\xA1' && buffer[5] == '\xB1'
        && buffer[6] == '\x1A' && buffer[7] == '\xE1') {
        return L"Microsoft Word 文档";
    }
    // 判断是否为Microsoft Excel表格
    else if (file.gcount() >= 8 && buffer[0] == '\xD0' && buffer[1] == '\xCF'
        && buffer[2] == '\x11' && buffer[3] == '\xE0' && buffer[4] == '\xA1' && buffer[5] == '\xB1'
        && buffer[6] == '\x1A' && buffer[7] == '\xE1') {
        return L"Microsoft Excel 表格";
    }
    // 还可以根据需要添加其他文件类型的判断

    return L"未知";
}






#include <fstream>
#include <string>
#include <string.h>
#include <iostream>

std::string GetFileType(const char* filename) {
    std::ifstream file(filename, std::ios_base::binary);
    char buffer[8];
    file.read(buffer, sizeof(buffer));

    // 使用魔术字判断文件类型
    // 
    // 可执行文件 的魔术字为 MZ 前两个字节
    if (file.gcount() >= 2 && buffer[0] == 'M' && buffer[1] == 'Z') {
        return "可执行文件";
    }

    // JPEG 文件的魔术数字为 FF D8 FF
    if (file.gcount() >= 3 && (unsigned char)buffer[0] == 0xFF && (unsigned char)buffer[1] == 0xD8 && (unsigned char)buffer[2] == 0xFF) {
        return "JPEG 图片";
    }

    // PNG 文件的魔术数字为 89 50 4E 47 0D 0A 1A 0A
    unsigned char png_magic[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
    if (file.gcount() >= 8 && memcmp(buffer, png_magic, sizeof(png_magic)) == 0) {
        return "PNG 图片";
    }


    // PSD 文件的魔术数字为 38 42 50 53 前四个字节
    unsigned char psd_magic[] = { 0x38, 0x42, 0x50, 0x53 };
    if (file.gcount() >= 4 && memcmp(buffer, psd_magic, sizeof(psd_magic)) == 0) {
        return "PSD 文件";
    }

    // Blender 文件的魔术数字为 42 4C 45 4E  前四个字节
    unsigned char blender_magic[] = { 0x42, 0x4C, 0x45, 0x4E };
    if (file.gcount() >= 4 && memcmp(buffer, blender_magic, sizeof(blender_magic)) == 0) {
        return "Blender 文件";
    }

    // CAD 文件的魔术数字为 42 4C 45 4E  前四个字节
    unsigned char CAD_magic[] = { 0x42, 0x4C, 0x45, 0x4E };
    if (file.gcount() >= 4 && memcmp(buffer, CAD_magic, sizeof(blender_magic)) == 0) {
        return "CAD 文件";
    }


    // 其他文件类型的判断...

    return "未知文件类型";
}

//
//int main() {
//    const char* filename = "K:/img.txt";
//    std::string fileType = GetFileType(filename);
//    std::cout << "文件类型：" << fileType << std::endl;
//
//    return 0;
//}
