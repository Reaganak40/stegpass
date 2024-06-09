#include "core_unit_tests.hpp"
#include <gtest/gtest.h>

namespace {

	TEST(CLI_PARSER, Test1) {

		const char* supported_types[] = {".BMP", ".JPG", ".PNG"};
		CLIParser parser(3, supported_types);

		EXPECT_STREQ(".BMP", parser.GetSupportedFileType(0));
		EXPECT_STREQ(".JPG", parser.GetSupportedFileType(1));
		EXPECT_STREQ(".PNG", parser.GetSupportedFileType(2));
	}
}