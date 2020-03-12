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
		wchar_t Context::getLastChar()
		{
			source.object.unget();
			return getNextChar();
		}
		wchar_t Context::lookLastChar()
		{
			wchar_t ch = getLastChar();
			getNextChar();
			return ch;
		}
		bool Context::isEnd()
		{
			return lookNextChar() == WEOF;
		}
	}
}
