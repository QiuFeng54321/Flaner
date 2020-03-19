#ifndef _FLANER_LEXER_CONTEXT_HH_
#define _FLANER_LEXER_CONTEXT_HH_

#include <lexer/io.hh>

namespace flaner
{
namespace lexer
{
	class Context
	{
	public:
		Context(std::string path)
			: source(path),
			lineOffset(0), charOffset(0)
		{

		}
		~Context() {}

	public:
		char getNextchar(size_t offset);
		char lookNextchar(size_t offset);
		char getLastchar();
		char lookLastchar();
		bool isEnd();

	public:
		io::Source source;
		size_t lineOffset, charOffset;
	};
}
}

#endif // !_FLANER_LEXER_CONTEXT_HH_
