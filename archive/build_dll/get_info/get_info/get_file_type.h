#pragma once
#include <windows.h>
#include <fstream>


// �����ļ�ͷ��ʶ��ȡ�ļ�����
std::wstring GetFileType(const std::wstring& filename) {
    std::ifstream file(filename, std::ios_base::binary);
    char buffer[16];
    file.read(buffer, sizeof(buffer));

    if (file.gcount() >= 2 && buffer[0] == 'M' && buffer[1] == 'Z') {
        return L"��ִ���ļ�";
    }
    // �ж��Ƿ�ΪJPEGͼƬ
    else if (file.gcount() >= 4 && buffer[0] == '\xFF' && buffer[1] == '\xD8'
        && buffer[2] == '\xFF' && (buffer[3] == '\xE0' || buffer[3] == '\xE1')) {
        return L"JPEG ͼƬ";
    }
    // �ж��Ƿ�ΪPNGͼƬ
    else if (file.gcount() >= 4 && buffer[0] == '\x89' && buffer[1] == 'P'
        && buffer[2] == 'N' && buffer[3] == 'G') {
        return L"PNG ͼƬ";
    }
    // �ж��Ƿ�ΪGIF87aͼƬ
    else if (file.gcount() >= 6 && buffer[0] == 'G' && buffer[1] == 'I'
        && buffer[2] == 'F' && buffer[3] == '8' && buffer[4] == '7' && buffer[5] == 'a') {
        return L"GIF87a ͼƬ";
    }
    // �ж��Ƿ�ΪGIF89aͼƬ
    else if (file.gcount() >= 6 && buffer[0] == 'G' && buffer[1] == 'I'
        && buffer[2] == 'F' && buffer[3] == '8' && buffer[4] == '9' && buffer[5] == 'a') {
        return L"GIF89a ͼƬ";
    }
    // �ж��Ƿ�ΪZIPѹ���ļ�
    else if (file.gcount() >= 4 && buffer[0] == '\x50' && buffer[1] == 'K'
        && buffer[2] == '\x03' && buffer[3] == '\x04') {
        return L"ZIP ѹ���ļ�";
    }
    // �ж��Ƿ�ΪRARѹ���ļ�
    else if (file.gcount() >= 6 && buffer[0] == 'R' && buffer[1] == 'a'
        && buffer[2] == 'r' && buffer[3] == '!' && buffer[4] == '\x1A' && buffer[5] == '\x7A') {
        return L"RAR ѹ���ļ�";
    }
    // �ж��Ƿ�ΪOutlook�ʼ��ļ�
    else if (file.gcount() >= 16 && buffer[0] == 'M' && buffer[1] == 'S'
        && buffer[2] == 'C' && buffer[3] == 'F' && buffer[8] == '\x05' && buffer[9] == '\x06') {
        return L"Outlook �ʼ��ļ�";
    }
    // �ж��Ƿ�ΪJava JAR�ļ�
    else if (file.gcount() >= 4 && buffer[0] == 'P' && buffer[1] == 'K'
        && buffer[2] == '\x03' && buffer[3] == '\x04') {
        return L"Java JAR �ļ�";
    }
    // �ж��Ƿ�ΪBad Apple!!��Ƶ�ļ�
    else if (file.gcount() >= 4 && buffer[0] == 'B' && buffer[1] == 'A'
        && buffer[2] == 'D' && buffer[3] == 'E') {
        return L"Bad Apple!! ��Ƶ";
    }
    // �ж��Ƿ�ΪPDF�ļ�
    else if (file.gcount() >= 5 && buffer[0] == '%' && buffer[1] == 'P'
        && buffer[2] == 'D' && buffer[3] == 'F' && buffer[4] == '-') {
        return L"PDF �ļ�";
    }
    // �ж��Ƿ�ΪMicrosoft Word�ĵ�
    else if (file.gcount() >= 8 && buffer[0] == '\xD0' && buffer[1] == '\xCF'
        && buffer[2] == '\x11' && buffer[3] == '\xE0' && buffer[4] == '\xA1' && buffer[5] == '\xB1'
        && buffer[6] == '\x1A' && buffer[7] == '\xE1') {
        return L"Microsoft Word �ĵ�";
    }
    // �ж��Ƿ�ΪMicrosoft Excel���
    else if (file.gcount() >= 8 && buffer[0] == '\xD0' && buffer[1] == '\xCF'
        && buffer[2] == '\x11' && buffer[3] == '\xE0' && buffer[4] == '\xA1' && buffer[5] == '\xB1'
        && buffer[6] == '\x1A' && buffer[7] == '\xE1') {
        return L"Microsoft Excel ���";
    }
    // �����Ը�����Ҫ��������ļ����͵��ж�

    return L"δ֪";
}






#include <fstream>
#include <string>
#include <string.h>
#include <iostream>

std::string GetFileType(const char* filename) {
    std::ifstream file(filename, std::ios_base::binary);
    char buffer[8];
    file.read(buffer, sizeof(buffer));

    // ʹ��ħ�����ж��ļ�����
    // 
    // ��ִ���ļ� ��ħ����Ϊ MZ ǰ�����ֽ�
    if (file.gcount() >= 2 && buffer[0] == 'M' && buffer[1] == 'Z') {
        return "��ִ���ļ�";
    }

    // JPEG �ļ���ħ������Ϊ FF D8 FF
    if (file.gcount() >= 3 && (unsigned char)buffer[0] == 0xFF && (unsigned char)buffer[1] == 0xD8 && (unsigned char)buffer[2] == 0xFF) {
        return "JPEG ͼƬ";
    }

    // PNG �ļ���ħ������Ϊ 89 50 4E 47 0D 0A 1A 0A
    unsigned char png_magic[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
    if (file.gcount() >= 8 && memcmp(buffer, png_magic, sizeof(png_magic)) == 0) {
        return "PNG ͼƬ";
    }


    // PSD �ļ���ħ������Ϊ 38 42 50 53 ǰ�ĸ��ֽ�
    unsigned char psd_magic[] = { 0x38, 0x42, 0x50, 0x53 };
    if (file.gcount() >= 4 && memcmp(buffer, psd_magic, sizeof(psd_magic)) == 0) {
        return "PSD �ļ�";
    }

    // Blender �ļ���ħ������Ϊ 42 4C 45 4E  ǰ�ĸ��ֽ�
    unsigned char blender_magic[] = { 0x42, 0x4C, 0x45, 0x4E };
    if (file.gcount() >= 4 && memcmp(buffer, blender_magic, sizeof(blender_magic)) == 0) {
        return "Blender �ļ�";
    }

    // CAD �ļ���ħ������Ϊ 42 4C 45 4E  ǰ�ĸ��ֽ�
    unsigned char CAD_magic[] = { 0x42, 0x4C, 0x45, 0x4E };
    if (file.gcount() >= 4 && memcmp(buffer, CAD_magic, sizeof(blender_magic)) == 0) {
        return "CAD �ļ�";
    }


    // �����ļ����͵��ж�...

    return "δ֪�ļ�����";
}

//
//int main() {
//    const char* filename = "K:/img.txt";
//    std::string fileType = GetFileType(filename);
//    std::cout << "�ļ����ͣ�" << fileType << std::endl;
//
//    return 0;
//}
