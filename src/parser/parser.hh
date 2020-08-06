#ifndef _FLANER_PARSER_PARSER_HH_
#define _FLANER_PARSER_PARSER_HH_

#include <string>
#include <map>
#include <parser/ast.hh>

namespace flaner
{
namespace parser
{
	enum class Priority : uint16_t
	{
		comma,
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

}
}

#endif // !_FLANER_PARSER_PARSER_HH_
