#include "backend_testing_environment.hpp"
#include "test_utils.hpp"

#include "core/Utils.hpp"

// assumes the tests are executed from the project root directory
#define PATH_TO_BMP_STEG "../bin/Release-windows-x86_64/bmp-steg/bmp-steg.exe"
#define PATH_TO_CORE_DLL "../lib/Release-windows-x86_64/core/core.dll"

void BackendTestingEnvironment::SetUp()
{
	assert(test_utils::CopyToBin(PATH_TO_BMP_STEG));
	assert(test_utils::CopyToBin(PATH_TO_CORE_DLL));


	// ensure that cat.bmp always has the same password stored
	std::string target_file = "test_images\\cat.bmp";
	std::string hexString = "DEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF";
	std::string password = "whatIsUp!";

	std::string standard_out;
	int exit_code;

	// test save password function
	std::string save_password = ".\\bin\\bmp-steg.exe -s " + target_file + " " + password + " -h " + hexString;
	exit_code = test_utils::RunCommand(save_password, standard_out);

	if (exit_code != sp::ExitCode::Success) {
		std::cerr << "Failed to save password for cat.bmp" << std::endl;
	}
}

void BackendTestingEnvironment::TearDown()
{
	assert(test_utils::RemoveBin());
}
