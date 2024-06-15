#include <iostream>
#include <Windows.h>

#define MAX_COPY_SIZE 126

/// <summary>
/// Copies text to the clipboard but avoids it from being used by clipboard monitoring applications.
/// </summary>
/// <param name="text">text to copy to the clipboard.</param>
void CopyTextToClipboardSecure(const char* text) {

    // Register the exclusion format
    UINT excludeFormat = RegisterClipboardFormat(L"ExcludeClipboardContentFromMonitorProcessing");

    if (excludeFormat == 0) {
        std::cerr << "Failed to register clipboard format" << std::endl;
        return;
    }
    // Open the clipboard
    if (!OpenClipboard(nullptr)) {
        std::cerr << "Failed to open clipboard" << std::endl;
        return;
    }

    // Clear the clipboard
    if (!EmptyClipboard()) {
        std::cerr << "Failed to empty clipboard" << std::endl;
        CloseClipboard();
        return;
    }

    // Get the length of the text
    size_t len = strlen(text) + 1;

    // Allocate global memory for the text
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, len);
    if (hGlobal == nullptr) {
        std::cerr << "Failed to allocate global memory" << std::endl;
        CloseClipboard();
        return;
    }

    LPVOID lock = GlobalLock(hGlobal);
    if (lock == NULL) {
        std::cerr << "Failed to lock global memory" << std::endl;
		GlobalFree(hGlobal);
		CloseClipboard();
		return;
    }

    // Copy the text to the allocated memory
    memcpy(lock, text, len);
    GlobalUnlock(hGlobal);

    // Set the clipboard data with the custom format
    if (SetClipboardData(excludeFormat, hGlobal) == nullptr) {
        std::cerr << "Failed to set clipboard data using exclude format" << std::endl;
        GlobalFree(hGlobal);
        CloseClipboard();
        return;
    }

    // Also set the clipboard data as CF_TEXT so it can be used normally by other applications
    if (SetClipboardData(CF_TEXT, hGlobal) == nullptr) {
        std::cerr << "Failed to set clipboard data using CF_TEXT" << std::endl;
        GlobalFree(hGlobal);
        CloseClipboard();
        return;
    }

    // Close the clipboard
    CloseClipboard();

    // Normally, you'd free the global memory here, but the clipboard now owns it
}

int main(int argc, char* argv[]) {

	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <text_for_clipboard>" << std::endl;
		return 1;
	}
    
    CopyTextToClipboardSecure(argv[1]);

	return 0;
}