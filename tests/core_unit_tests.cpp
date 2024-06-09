#include "core_unit_tests.hpp"
#include <gtest/gtest.h>

namespace {

	TEST(SP_UTILS, Test1) {
		const char* hexString = "DEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF";
		Hash256 expected = {
			0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF,
			0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF,
			0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF,
			0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF
		};
		Hash256 hash;

		EXPECT_TRUE(sp::StringToHash(hash, hexString));
		for (int i = 0; i < 32; i++) {
			EXPECT_EQ(expected[i], hash[i]);
		}
	}

	TEST(CLI_PARSER, TestFileExtensionSupport) {

		const char* supported_types[] = {".BMP", ".JPG", ".PNG"};
		sp::CLIParser parser(3, supported_types);

		EXPECT_STREQ(".bmp", parser.GetSupportedFileType(0));
		EXPECT_STREQ(".jpg", parser.GetSupportedFileType(1));
		EXPECT_STREQ(".png", parser.GetSupportedFileType(2));
	}

	TEST(CLI_PARSER, TestMissingCLIArguments) {

		const char* supported_types[] = { ".BMP" };
		sp::CLIParser parser(1, supported_types);

		// not enough arguments
		const char* args[] = { "test", };
		EXPECT_EQ(parser.Parse(1, args, false), 1);
	}

	TEST(CLI_PARSER, TestBadActionArgument) {

		const char* supported_types[] = { ".BMP" };
		sp::CLIParser parser(1, supported_types);

		// bad action argument
		const char* args[] = { "test", "-x", "image.bmp"};
		constexpr int argc = sizeof(args) / sizeof(char*);

		EXPECT_EQ(parser.Parse(argc, args, false), 2);
	}

	TEST(CLI_PARSER, TestUnsupportedFilepath1) {

		const char* supported_types[] = { ".BMP" };
		sp::CLIParser parser(1, supported_types);

		// bad action argument
		const char* args[] = { "test", "-s", "image.jpg" };
		constexpr int argc = sizeof(args) / sizeof(char*);
		EXPECT_EQ(parser.Parse(argc, args, false), 2);
	}

	TEST(CLI_PARSER, TestUnsupportedFilepath2) {

		const char* supported_types[] = { ".BMP" };
		sp::CLIParser parser(1, supported_types);

		// bad action argument
		const char* args[] = { "test", "-g", "image.jpg" };
		constexpr int argc = sizeof(args) / sizeof(char*);
		EXPECT_EQ(parser.Parse(argc, args, false), 2);
	}

	TEST(CLI_PARSER, TestValidArgument1) {

		const char* weird_extensions[] = {".bmp", ".BMP", ".BmP", ".bMp", ".bmP"};
		const char* supported_types[] = { ".BMP" };

		for (int i = 0; i < 5; i++) {
			sp::CLIParser parser(1, supported_types);

			std::string filepath = "image";
			filepath += weird_extensions[i];

			// valid action argument
			const char* args[] = { "test", "-s", filepath.c_str() };
			constexpr int argc = sizeof(args) / sizeof(char*);
			EXPECT_EQ(parser.Parse(argc, args, false), 0);
		}
	}

}