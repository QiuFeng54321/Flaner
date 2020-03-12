#ifndef _FLANER_LEXER_LEXER_HH_
#define _FLANER_LEXER_LEXER_HH_

#include <lexer/context.hh>

namespace flaner
{
namespace lexer
{
	class Lexer
	{
	public:
		Lexer() {}
		~Lexer() {}

	public:
		enum class TokenType
		{
			UNKNOWN,
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
