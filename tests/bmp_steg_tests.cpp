#include <gtest/gtest.h>
#include "test_utils.hpp"
#include "core/Utils.hpp"

namespace {

	TEST(BMP_STEG, TestNoArguments) {
		std::string standard_out;
		int exit_code;

		// test the help command
		exit_code = test_utils::RunCommand(".\\bin\\bmp-steg.exe", standard_out);
		EXPECT_EQ(exit_code, sp::ExitCode::NotEnoughArguments);
	}

	TEST(BMP_STEG, TestGetPasswordSucess) {
		std::string standard_out;
		int exit_code;
		
		// test get password function
		std::string hexString = "DEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF";
		std::string command = ".\\bin\\bmp-steg.exe -g test_images\\cat.bmp -h " + hexString;
		const char* expected_password = "whatIsUp!";
		exit_code = test_utils::RunCommand(command, standard_out);

		EXPECT_EQ(exit_code, sp::ExitCode::Success);
		EXPECT_STREQ(expected_password, standard_out.c_str());
	}

	TEST(BMP_STEG, TestGetPasswordBadHash) {
		std::string standard_out;
		int exit_code;

		// test get password function
		std::string hexString = "DEADBEEFDEADBEEFD3ADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF";
		std::string command = ".\\bin\\bmp-steg.exe -g test_images\\cat.bmp -h " + hexString;
		const char* expected_password = "";
		exit_code = test_utils::RunCommand(command, standard_out);

		EXPECT_EQ(exit_code, sp::ExitCode::CouldNotRecoverPassword);
		EXPECT_STREQ(expected_password, standard_out.c_str());
	}

	TEST(BMP_STEG, TestPasswordSavingAndRecovery1) {
		std::string standard_out;
		int exit_code;

		std::string target_file = "test_images\\8893241.bmp";
		std::string hexString = "CAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABE";
		std::string password = "password123";

		// test save password function
		std::string save_password = ".\\bin\\bmp-steg.exe -s " + target_file + " " + password + " -h " + hexString;
		exit_code = test_utils::RunCommand(save_password, standard_out);

		EXPECT_EQ(exit_code, sp::ExitCode::Success);
		EXPECT_STREQ("", standard_out.c_str());

		// test get password function
		std::string get_password = ".\\bin\\bmp-steg.exe -g " + target_file + " -h " + hexString;
		exit_code = test_utils::RunCommand(get_password, standard_out);

		EXPECT_EQ(exit_code, sp::ExitCode::Success);
		EXPECT_STREQ(password.c_str(), standard_out.c_str());
	}

	TEST(BMP_STEG, TestPasswordSavingAndRecovery2) {
		std::string standard_out;
		int exit_code;


		std::string target_file = "test_images\\moon.bmp";
		std::string hexString = "CAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABE";
		std::string password = "password234";

		// test save password function
		std::string save_password = ".\\bin\\bmp-steg.exe -s " + target_file + " " + password + " -h " + hexString;
		exit_code = test_utils::RunCommand(save_password, standard_out);

		EXPECT_EQ(exit_code, sp::ExitCode::Success);
		EXPECT_STREQ("", standard_out.c_str());

		// test get password function
		std::string get_password = ".\\bin\\bmp-steg.exe -g " + target_file + " -h " + hexString;
		exit_code = test_utils::RunCommand(get_password, standard_out);

		EXPECT_EQ(exit_code, sp::ExitCode::Success);
		EXPECT_STREQ(password.c_str(), standard_out.c_str());
	}

	TEST(BMP_STEG, TestPasswordSavingAndRecovery3) {
		std::string standard_out;
		int exit_code;


		std::string target_file = "test_images\\pug.bmp";
		std::string hexString = "c453bda50114b8df5e12c40b00da0dc38da561042bf60c09c608e67d9af82b86";
		std::string password = "hello/4!#6?what";

		// test save password function
		std::string save_password = ".\\bin\\bmp-steg.exe -s " + target_file + " " + password + " -h " + hexString;
		exit_code = test_utils::RunCommand(save_password, standard_out);

		EXPECT_EQ(exit_code, sp::ExitCode::Success);
		EXPECT_STREQ("", standard_out.c_str());

		// test get password function
		std::string get_password = ".\\bin\\bmp-steg.exe -g " + target_file + " -h " + hexString;
		exit_code = test_utils::RunCommand(get_password, standard_out);

		EXPECT_EQ(exit_code, sp::ExitCode::Success);
		EXPECT_STREQ(password.c_str(), standard_out.c_str());
	}
}