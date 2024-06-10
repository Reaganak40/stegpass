#include <string>

namespace test_utils {

	/// <summary>
	/// Converts a byte array to a string
	/// </summary>
	/// <param name="data">byte array.</param>
	/// <param name="data_size">size of the byte array.</param>
	/// <returns>string representation of the byte array.</returns>
	std::string ConvertToString(uint8_t data[], size_t data_size);

	/// <summary>
	/// Converts a string to a byte array
	/// </summary>
	/// <param name="data">string to convert.</param>
	/// <param name="out">byte array to store the converted string.</param>
	void ConvertToByteStream(const std::string& data, uint8_t* out);

	/// <summary>
	/// Copies the executable to the bin directory
	/// </summary>
	/// <param name="src_exe">source executable path</param>
	/// <returns>true if the copy was successful, false otherwise.</returns>
	bool CopyToBin(const std::string& src_exe);

	/// <summary>
	/// Removes the bin directory and its contents.
	/// </summary>
	/// <returns>true if the removal was successful, false otherwise.</returns>
	bool RemoveBin();

	/// <summary>
	/// Used to run an executable and get the standard output and return code
	/// </summary>
	/// <param name="command">command to run.</param>
	/// <param name="output">standard output of the command.</param>
	/// <returns>return code of the command.</returns>
	int RunCommand(const std::string& command, std::string& output);
}