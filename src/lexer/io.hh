#ifndef _FLANER_LEXER_IO_HH_
#define _FLANER_LEXER_IO_HH_

#include <string>
#include <iostream>
#include <fstream>
//#include <filesystem>

namespace flaner
{
namespace lexer
{
namespace io
{
	enum class Encoding
	{
		UTF_8,
		UTF_16,
		UTF_32,
	};

	enum class OpenMode
	{
		OpenExisting,
	};

	class Source
	{
	public:
		Source(std::string path, Encoding encoding = Encoding::UTF_8)
			: path(path),
			encoding(encoding),
			openMode(OpenMode::OpenExisting)
		{
			object.open(path, std::ios::in | std::ios::binary);
		}
		~Source()
		{
			object.close();
		}
	public:
		/*std::filesystem::path*/std::string path;
		Encoding encoding;
		OpenMode openMode;

		std::basic_fstream<wchar_t> object;
	};
}
}
}

#endif // !_FLANER_LEXER_IO_HH_
