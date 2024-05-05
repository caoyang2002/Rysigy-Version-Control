//#define NO_WIN32_LEAN_AND_MEAN
//#include <ShlObj.h> // 获取文件信息
//#include <shellapi.h>
//#pragma comment(lib,"shell32.lib")



#include "pch.h"
#include "get_info.h"

std::string CreateSHA256Hash(const std::string& input) {
	std::string s = input;
	SHA256 sha;
	sha.update(s);
	uint8_t* digest = sha.digest();
	std::string hash = SHA256::toString(digest);
	delete[] digest; // Don't forget to free the digest!

	cout << "哈希值为: " << hash << endl;
	return hash;
}





//=============================================================================================================================================

string get_info::getFileHash(fs::path pathName){
	fs::path currentPath = pathName; // 获取当前路径
	string fileHash;
	if (fs::is_directory(currentPath)) {
		cout << "[信息] 当前是文件夹" << endl;
		fileHash = CreateSHA256Hash(currentPath.string());
	}
    return fileHash;
}



// 获取文件大小
std::wstring get_info::GetFileSize(const std::wstring & filename) {
    WIN32_FIND_DATAW fileInfo;
    HANDLE hFind = FindFirstFileW(filename.c_str(), &fileInfo);
    if (hFind != INVALID_HANDLE_VALUE) {
        FindClose(hFind);
        LARGE_INTEGER size;
        size.HighPart = fileInfo.nFileSizeHigh;
        size.LowPart = fileInfo.nFileSizeLow;
        wchar_t sizeStr[50];
        _i64tow_s(size.QuadPart, sizeStr,50, 10);
        return sizeStr;
    }
    return L"";
}

// 获取文件占用空间
std::wstring get_info::GetFileOccupiedSpace(const std::wstring& filename) {
    ULARGE_INTEGER occupiedSpace;
    if (GetDiskFreeSpaceExW(filename.c_str(), &occupiedSpace, NULL, NULL)) {
        wchar_t occupiedSpaceStr[50];
        _ui64tow_s(occupiedSpace.QuadPart, occupiedSpaceStr, 50, 10);
        return std::wstring(occupiedSpaceStr); // 使用std::wstring保存转换后的字符串
    }
    return L"";
}

// 获取文件创建时间
std::wstring get_info::GetFileCreationTime(const std::wstring& filename) {
    WIN32_FIND_DATAW fileInfo;
    HANDLE hFind = FindFirstFileW(filename.c_str(), &fileInfo);
    if (hFind != INVALID_HANDLE_VALUE) {
        FindClose(hFind);
        SYSTEMTIME createTime;
        FileTimeToSystemTime(&fileInfo.ftCreationTime, &createTime);
        wchar_t createTimeStr[30];
        swprintf_s(createTimeStr, L"%04d-%02d-%02d %02d:%02d:%02d", createTime.wYear, createTime.wMonth, createTime.wDay, createTime.wHour, createTime.wMinute, createTime.wSecond);
        return createTimeStr;
    }
    return L"";
}

// 获取文件修改时间
std::wstring get_info::GetFileModificationTime(const std::wstring& filename) {
    WIN32_FIND_DATAW fileInfo;
    HANDLE hFind = FindFirstFileW(filename.c_str(), &fileInfo);
    if (hFind != INVALID_HANDLE_VALUE) {
        FindClose(hFind);
        SYSTEMTIME modifyTime;
        FileTimeToSystemTime(&fileInfo.ftLastWriteTime, &modifyTime);
        wchar_t modifyTimeStr[30];
        swprintf_s(modifyTimeStr, L"%04d-%02d-%02d %02d:%02d:%02d", modifyTime.wYear, modifyTime.wMonth, modifyTime.wDay, modifyTime.wHour, modifyTime.wMinute, modifyTime.wSecond);
        return modifyTimeStr;
    }
    return L"";
}

// 获取文件访问时间
std::wstring get_info::GetFileAccessTime(const std::wstring& filename) {
    WIN32_FIND_DATAW fileInfo;
    HANDLE hFind = FindFirstFileW(filename.c_str(), &fileInfo);
    if (hFind != INVALID_HANDLE_VALUE) {
        FindClose(hFind);
        SYSTEMTIME accessTime;
        FileTimeToSystemTime(&fileInfo.ftLastAccessTime, &accessTime);
        wchar_t accessTimeStr[30];
        swprintf_s(accessTimeStr, L"%04d-%02d-%02d %02d:%02d:%02d", accessTime.wYear, accessTime.wMonth, accessTime.wDay, accessTime.wHour, accessTime.wMinute, accessTime.wSecond);
        return accessTimeStr;
    }
    return L"";
}

// 获取文件加密信息
std::wstring get_info::GetFileEncryptionInfo(const std::wstring& filename) {
    DWORD encryptAttrs;
    if (GetFileAttributesExW(filename.c_str(), GetFileExInfoStandard, &encryptAttrs)) {
        return (encryptAttrs & FILE_ATTRIBUTE_ENCRYPTED) ? L"已加密" : L"未加密";
    }
    return L"未知";
}



