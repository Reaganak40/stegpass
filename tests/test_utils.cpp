#include "test_utils.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <windows.h>

#define BIN_DIR "bin/"

std::string test_utils::ConvertToString(uint8_t data[], size_t data_size)
{
	std::string result;
	for (size_t i = 0; i < data_size; i++) {
		result += data[i];
	}
	return result;
}

void test_utils::ConvertToByteStream(const std::string& data, uint8_t* out)
{
	for (size_t i = 0; i < data.size(); i++) {
		out[i] = data[i];
	}
}

bool test_utils::CopyToBin(const std::string& src_exe)
{
	// create bin directory if it does not exist
	if (!std::filesystem::exists(BIN_DIR)) {
		if (!std::filesystem::create_directory(BIN_DIR)) {
			std::cerr << "Failed to create directory: " << BIN_DIR << std::endl;
			return false;
		}
	}

	std::string exe_name = src_exe.substr(src_exe.find_last_of("\\/") + 1);
	std::string dst_exe = BIN_DIR + exe_name;

	std::ifstream src(src_exe, std::ios::binary);
	std::ofstream dst(dst_exe, std::ios::binary);

	if (!src.is_open()) {
		std::cerr << "Failed to open source file: " << src_exe << std::endl;
		return false;
	}

	if (!dst.is_open()) {
		std::cerr << "Failed to open destination file: " << dst_exe << std::endl;
		return false;
	}

	dst << src.rdbuf();

	src.close();
	dst.close();
	return true;
}

bool test_utils::RemoveBin()
{
	if (!std::filesystem::exists(BIN_DIR)) {
		return true;
	}

	try {
		std::filesystem::remove_all(BIN_DIR);
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Failed to remove directory: " << BIN_DIR << std::endl;
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

int test_utils::RunCommand(const std::string& command, std::string& output)
{
    HANDLE hPipeRead, hPipeWrite;

    // Set the bInheritHandle flag so pipe handles are inherited.
    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe for the child process's STDOUT.
    if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0)) {
        throw std::runtime_error("CreatePipe failed");
    }

    PROCESS_INFORMATION piProcInfo;
    STARTUPINFOA siStartInfo;
    BOOL bSuccess = FALSE;

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFOA));
    siStartInfo.cb = sizeof(STARTUPINFOA);
    siStartInfo.hStdOutput = hPipeWrite;
    siStartInfo.hStdError = hPipeWrite;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    bSuccess = CreateProcessA(NULL,
        const_cast<LPSTR>(command.c_str()),
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &siStartInfo,
        &piProcInfo);

    if (!bSuccess) {
        CloseHandle(hPipeWrite);
        CloseHandle(hPipeRead);
        throw std::runtime_error("CreateProcess failed");
    }

    CloseHandle(hPipeWrite);

    DWORD dwRead;
    CHAR chBuf[4096];
    output.clear();

    while (ReadFile(hPipeRead, chBuf, sizeof(chBuf) - 1, &dwRead, NULL) && dwRead > 0) {
        chBuf[dwRead] = '\0';
        output += chBuf;
    }

    CloseHandle(hPipeRead);
    WaitForSingleObject(piProcInfo.hProcess, INFINITE);

    DWORD exitCode;
    GetExitCodeProcess(piProcInfo.hProcess, &exitCode);

    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);

    return static_cast<int>(exitCode);
}

