#ifndef _FLANER_LEXER_IO_HH_
#define _FLANER_LEXER_IO_HH_

#include <string>
#include <iostream>
#include <filesystem>

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
		Source(std::string path)
			: path(path),
			encoding(Encoding::UTF_8),
			openMode(OpenMode::OpenExisting)
		{}
		~Source() {}
	public:
		std::filesystem::path path;
		Encoding encoding;
		OpenMode openMode;
	};
}
}
}

#endif // !_FLANER_LEXER_IO_HH_
