#include "context.hh"
#include "context.hh"
#include "context.hh"
#include <lexer/context.hh>

namespace flaner
{
	namespace lexer
	{		
		wchar_t Context::getNextChar()
		{
			return source.object.get();
		}
		wchar_t Context::lookNextChar()
		{
			return source.object.peek();
		}
		bool Context::isEnd()
		{
			return lookNextChar() == WEOF;
		}
	}
}
