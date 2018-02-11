#include "FileBuffer.h"
#include <Windows.h>


FileBuffer::FileBuffer()
{
}


FileBuffer::~FileBuffer()
{
}

/// <summary>
/// Loads all the data from given file and returns the data in a string stream.
/// </summary>
/// <param name="file">Name of file to load</param>
/// <returns>StringStream with loaded buffer data.</returns>
/// <remarks>Untested with binary and other modes except string.</remarks>
std::stringstream& FileBuffer::Load(std::string file)
{
	std::ifstream inFile(file);
	if (!inFile.is_open()) {
#ifdef _DEBUG
		printf("Error in loading save file");
#endif
		MessageBox(NULL, "Unable to load save file", "Error", MB_ICONWARNING);
		throw std::exception("Unable to load file");
	}
	buffer = std::stringstream();
	buffer << inFile.rdbuf();
	inFile.close();
	inFile.clear();
	return buffer;
}

/// <summary>
/// Save given stringstream buffer to given file. 
/// </summary>
/// <param name="file">Name of file to save to.</param>
/// <param name="buffer">Stringstream buffer with data which is to be saved.</param>
/// <remarks>Untested with binary and other modes except string.</remarks>
void FileBuffer::Save(std::string file, std::stringstream &buffer)
{
	std::ofstream outFile(file);
	outFile.write(buffer.str().c_str(), buffer.str().length());
	outFile.close();
}
