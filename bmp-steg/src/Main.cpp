#include <iostream>
#include <string>
#include "BMP.hpp"

#include <core/CLIParser.hpp>

int main(int argc, char* argv[]) {
    /*
    * RETURN CODES:
    * 0: Success
    * 1: Not enough arguments
    * 2: Invalid argument(s)
    * 3: BMP file does not exist
    * 4: Could not recover password
    * -1: Unknown/Unexpected error
    */

    const char* supported_types[] = { ".BMP" };
    sp::CLIParser parser(1, supported_types);

#if defined(SP_DEBUG)
    // Make root path from the project directory
    std::string root_path = std::string(argv[0]);
    root_path = root_path.substr(0, root_path.find_last_of("\\/"));

    root_path += "\\..\\..\\..\\bmp-steg\\";
    parser.SetRootPath(root_path.c_str());
#endif

    int parse_result = parser.Parse(argc, const_cast<const char**>(argv));
    if (parse_result) {
		return parse_result;
	}

    if (!parser.TargetFileExists()) {
		std::cerr << "BMP file does not exist: " << parser.GetTargetFile() << std::endl;
		return 3;
	}

    // Get the arguments from the parser
    int action = parser.Action();
    std::string target_file = parser.GetTargetFile();
    
    Hash256 encryption_key;
    (void)parser.GetHash(encryption_key);

    // NOTE: Should not raise an exception
    BMP bmp(target_file);
    bmp.SetEncryptionKey(encryption_key);

    if (action == SP_CLI_ACTION_HIDE) {
        try {
            if (!bmp.HideMessage(parser.GetPassword())) {
				return -1;
			}

            if (!bmp.Save(target_file)) {
                std::cerr << "Could not save BMP file." << std::endl;
                return -1;
            }
		}
        catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			return -1;
        }
    }

    else if (action == SP_CLI_ACTION_EXTRACT) {
        try {
            std::string message = bmp.ExtractMessage();
            if (message.empty()) {
                return 4;
            }
            std::cout << message << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return -1;
        }
	}

    return 0;
}