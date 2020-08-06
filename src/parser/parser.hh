#ifndef _FLANER_PARSER_PARSER_HH_
#define _FLANER_PARSER_PARSER_HH_

#include <string>
#include <map>
#include <parser/ast.hh>
#include <parser/error.hh>

namespace flaner
{
namespace parser
{
	using Token = lexer::Lexer::Token;

	enum class Priority : uint16_t
	{
		comma = 1,
		spread,
		yield,
		assignment,
		condition,
		logical_or,
		logical_and,
		bit_or,
		bit_xor,
		bit_and,
		comparision,
		bit_shift,
		addtion,
		multition,
		power,
		unary_operator,
		visitor,
		group,
	};

	Priority getOperatorPrecedence(Operator op);
	Priority getOperatorPrecedence(Token token);

	bool isOperatorToken(Token token);

}
}

#endif // !_FLANER_PARSER_PARSER_HH_
