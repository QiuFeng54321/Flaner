#ifndef _FLANER_LEXER_LEXER_HH_
#define _FLANER_LEXER_LEXER_HH_

#include <lexer/io.hh>

namespace flaner
{
namespace lexer
{
	class Lexer
	{
	public:


	public:
		enum class TokenType
		{
			NUMBER,
			OP_ADD,
			OP_MINUS,
			OP_MUL,
			OP_DIV,
			OP_MOD,
			OP_QUOTE,
		};
	};
}
}

#endif // !_FLANER_LEXER_LEXER_HH_
