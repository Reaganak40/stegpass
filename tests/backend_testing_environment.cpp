#include "backend_testing_environment.hpp"
#include "test_utils.hpp"

// assumes the tests are executed from the project root directory
#define PATH_TO_BMP_STEG "../bin/Release-windows-x86_64/bmp-steg/bmp-steg.exe"
#define PATH_TO_CORE_DLL "../lib/Release-windows-x86_64/core/core.dll"

void BackendTestingEnvironment::SetUp()
{
	assert(test_utils::CopyToBin(PATH_TO_BMP_STEG));
	assert(test_utils::CopyToBin(PATH_TO_CORE_DLL));
}

void BackendTestingEnvironment::TearDown()
{
	assert(test_utils::RemoveBin());
}
