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

	TEST(CLI_PARSER, TestInvalidArgument1) {

		const char* supported_types[] = { ".bmp" };
		sp::CLIParser parser(1, supported_types);

		// missing password parameter
		const char* args[] = { "test", "-s", "image.bmp" };
		constexpr int argc = sizeof(args) / sizeof(char*);
		EXPECT_EQ(parser.Parse(argc, args, false), 2);
	}

	TEST(CLI_PARSER, TestInvalidArgument2) {

		const char* supported_types[] = { ".bmp" };
		sp::CLIParser parser(1, supported_types);

		// skipped password parameter
		const char* args[] = { "test", "-s", "image.bmp", "-h", "DEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF"};
		constexpr int argc = sizeof(args) / sizeof(char*);
		EXPECT_EQ(parser.Parse(argc, args, false), 2);
	}

	TEST(CLI_PARSER, TestInvalidArgument3) {

		const char* supported_types[] = { ".bmp" };
		sp::CLIParser parser(1, supported_types);

		// extra argument after password
		const char* args[] = { "test", "-s", "image.bmp", "pass123", "DEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF" };
		constexpr int argc = sizeof(args) / sizeof(char*);
		EXPECT_EQ(parser.Parse(argc, args, false), 2);
	}

	TEST(CLI_PARSER, TestInvalidArgument4) {

		const char* supported_types[] = { ".bmp" };
		sp::CLIParser parser(1, supported_types);

		// skipped password parameter
		const char* args[] = { "test", "-s", "image.bmp", "-h", "DEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF" };
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
			const char* args[] = { "test", "-g", filepath.c_str() };
			constexpr int argc = sizeof(args) / sizeof(char*);
			EXPECT_EQ(parser.Parse(argc, args, true), 0);
		}
	}

	TEST(CLI_PARSER, TestValidArgument2) {

		const char* weird_extensions[] = { ".bmp", ".BMP", ".BmP", ".bMp", ".bmP" };
		const char* supported_types[] = { ".BMP" };

		for (int i = 0; i < 5; i++) {

			sp::CLIParser parser(1, supported_types);

			std::string filepath = "image";
			filepath += weird_extensions[i];

			// valid action argument
			const char* args[] = { "test", "-s", filepath.c_str(), "mypassword"};
			constexpr int argc = sizeof(args) / sizeof(char*);
			EXPECT_EQ(parser.Parse(argc, args, true), 0);
		}
	}

	TEST(CLI_PARSER, TestValidArgument3) {

		const char* supported_types[] = { ".bmp" };
		sp::CLIParser parser(1, supported_types);

		// skipped password parameter
		const char* args[] = { "test", "-s", "image.bmp", "qwerty", "-h", "DEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF" };
		constexpr int argc = sizeof(args) / sizeof(char*);
		
		EXPECT_EQ(parser.Parse(argc, args, true), 0);
		EXPECT_EQ(parser.Action(), SP_CLI_ACTION_HIDE);
		EXPECT_STREQ("qwerty", parser.GetPassword());
	}

	TEST(CLI_PARSER, TestValidArgument4) {
		const char* supported_types[] = { ".bmp" };
		sp::CLIParser parser(1, supported_types);

		// skipped password parameter
		const char* args[] = { "test", "-g", "image.bmp", "-h", "DEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF" };
		constexpr int argc = sizeof(args) / sizeof(char*);

		EXPECT_EQ(parser.Parse(argc, args, true), 0);
		EXPECT_EQ(parser.Action(), SP_CLI_ACTION_EXTRACT);
		EXPECT_STREQ("", parser.GetPassword());
	}

	TEST(CLI_PARSER, TestValidArgument5) {
		const char* supported_types[] = { ".bmp" };
		sp::CLIParser parser(1, supported_types);

		// path with '/' instead of '\'
		const char* args[] = { "test", "-s", "res/image.bmp", "qwerty", "-h", "DEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF" };
		constexpr int argc = sizeof(args) / sizeof(char*);

		EXPECT_EQ(parser.Parse(argc, args, true), 0);
		EXPECT_EQ(parser.Action(), SP_CLI_ACTION_HIDE);
		EXPECT_STREQ("qwerty", parser.GetPassword());
	}

	TEST(SP_OBFUSCATOR, TestBBS1) {

		const char* hexString = "DEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF";
		Hash256 encryption_key;
		sp::StringToHash(encryption_key, hexString);

		std::string plaintext = "original";
		uint8_t* data = new uint8_t[plaintext.size()];
		test_utils::ConvertToByteStream(plaintext, data);

		// encrypt
		sp::Obfuscator::Crypt(data, plaintext.size(), encryption_key);

		// decrypt
		sp::Obfuscator::Crypt(data, plaintext.size(), encryption_key);

		// convert to string
		std::string decrypted;
		decrypted = test_utils::ConvertToString(data, plaintext.size());

		ASSERT_EQ(plaintext, decrypted);

		delete[] data;
	}

}