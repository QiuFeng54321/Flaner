#include "context.hh"
#include "context.hh"
#include "context.hh"
#include <lexer/context.hh>

namespace flaner
{
	namespace lexer
	{		
		wchar_t Context::getNextChar(size_t offset)
		{
			if (offset == 1)
			{
				return source.object.get();
			}
			else
			{
				source.object.seekp((offset - 1) * sizeof(wchar_t), std::ios::cur);
				return source.object.get();
			}
		}
		wchar_t Context::lookNextChar(size_t offset)
		{
			if (offset == 1)
			{
				return source.object.peek();
			}
			else
			{
				source.object.seekp((offset - 1) * sizeof(wchar_t), std::ios::cur);
				wchar_t ch = source.object.get();
				source.object.seekp(-static_cast<int>(offset * sizeof(wchar_t)), std::ios::cur);
				return ch;
			}
		}
		wchar_t Context::getLastChar()
		{
			source.object.unget();
			return getNextChar(1);
		}
		wchar_t Context::lookLastChar()
		{
			wchar_t ch = getLastChar();
			getNextChar(1);
			return ch;
		}
		bool Context::isEnd()
		{
			return lookNextChar(1) == WEOF;
		}
	}
}
