#ifndef _FLANER_PARSER_EXPRESSION_HH_
#define _FLANER_PARSER_EXPRESSION_HH_

#include <string>
#include <vector>
#include <map>
#include <variant>

namespace flaner
{
namespace parser
{
namespace syntax
{
	enum class Kind
	{
		ADD, MINUS, MUL, DIV, POW, INTDIV, MOD, QUOTE
	};
	class Expression;

	class Expression
	{
	public:
		Kind kind;
	};

	class LiteralNode : Expression
	{
	public:
	};

	class IDNode : Expression
	{
	public:
	};

	class BinaryNode : Expression
	{
	public:
		Expression left;
		Expression right;
	};

	class TernaryNode : Expression
	{
	public:
		Expression condition;
		Expression left;
		Expression right;
	};

	class CallNode : Expression
	{
	public:
		Expression function;
		std::vector<Expression> arguments;
	};
}
}
}

#endif // !_FLANER_PARSER_EXPRESSION_HH_
