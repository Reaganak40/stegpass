#include <iostream>
#include <string>
#include "BMP.hpp"
#include "Obfuscator.hpp"

int main(int argc, char* argv[]) {
    // Check if the number of arguments is less than 3
    if (argc < 3) {
        // If true, display the usage and return 3
        std::cerr << "Usage: .\\bmp-step.exe -[action] [path_to_bmp_file] [optional_parameters]" << std::endl;
        std::cerr << "Actions: -s (save password) [password], -g (get password)" << std::endl;
        std::cerr << "Optional parameters: -p [pepper] (added to the end of the password), -i [initial_value] (encryption key)" << std::endl;
        return 3;
    }

    // Get the action and BMP file path from the arguments
    std::string action = argv[1];
    std::string bmpFilePath = argv[2];

    // Initialize optional parameters
    std::string pepper;

    try {
        // Create a new BMP object
        BMP bmp(bmpFilePath);

        // Check the action
        if (action == "-s") {
            // Check if the number of arguments is less than 4
            if (argc < 4) {
                // If true, display the error and return 3
                std::cerr << "Password is required for -s action" << std::endl;
                return 3;
            }
            // Get the password from the arguments
            std::string password = argv[3];
            // Parse optional parameters
            for (int i = 4; i < argc; i++) {
                if (std::string(argv[i]) == "-p" && i + 1 < argc) {
                    pepper = argv[i + 1];
                    password += pepper;
                    i++;
                }
                else if (std::string(argv[i]) == "-i" && i + 1 < argc) {
                    uint32_t initialValue = std::stoul(argv[i + 1]);
                    bmp.SetEncryptionKey(initialValue);
                    i++;
                }
            }
            // Set the encryption key and hide the message
            if (bmp.HideMessage(password) == 0) {
                // If hiding the message fails, display the error and return 2
                std::cerr << "Failed to hide message" << std::endl;
                return 2;
            }
            // Save the BMP file
            if (!bmp.Save(bmpFilePath)) {
                // If saving the file fails, display the error and return 2
                std::cerr << "Failed to save BMP file" << std::endl;
                return 2;
            }
        }
        else if (action == "-g") {
            // Parse optional parameters
            for (int i = 3; i < argc; i++) {
                if (std::string(argv[i]) == "-p" && i + 1 < argc) {
                    pepper = argv[i + 1];
                    i++;
                }
                else if (std::string(argv[i]) == "-i" && i + 1 < argc) {
                    uint32_t initialValue = std::stoul(argv[i + 1]);
                    bmp.SetEncryptionKey(initialValue);
                    i++;
                }
            }
            // Set the encryption key and extract the message
            std::string password = bmp.ExtractMessage();
            if (password.empty()) {
                // If extracting the message fails, display the error and return 2
                std::cerr << "Failed to extract message" << std::endl;
                return 2;
            }
            // Remove the pepper from the password if present
            if (!pepper.empty() && password.size() > pepper.size()) {
                password.resize(password.size() - pepper.size());
            }
            // Display the extracted password
            std::cout << password << std::endl;
        }
        else {
            // If the action is invalid, display the error and return 3
            std::cerr << "Invalid action" << std::endl;
            return 3;
        }
    }
    catch (const std::runtime_error& e) {
        // If the BMP file path is invalid, display the error and return 1
        std::cerr << "Invalid BMP file path: " << e.what() << std::endl;
        return 1;
    }

    // If the program executes successfully, return 0
    return 0;
}