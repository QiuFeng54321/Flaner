#include <parser/parser.hh>

namespace flaner
{
namespace parser
{
	static const std::map<Operator, Priority> PRECEDENCE_MAP =
	{
		{ Operator::OP_ADD, Priority::addtion },
		{ Operator::OP_MINUS, Priority::addtion },
		{ Operator::OP_MUL, Priority::multition },
		{ Operator::OP_DIV, Priority::multition },
		{ Operator::OP_MOD, Priority::multition },
		{ Operator::OP_GREATER_THAN, Priority::comparision },
		{ Operator::OP_GREATER_EQUAL, Priority::comparision },
		{ Operator::OP_LESS_THAN, Priority::comparision },
		{ Operator::OP_LESS_EQUAL, Priority::comparision },
		{ Operator::OP_EQUAL, Priority::comparision },
		{ Operator::OP_NOT_EQUAL, Priority::comparision },
	};


	Priority getOperatorPrecedence(Operator op)
	{
		Priority p;
		try
		{
			p = PRECEDENCE_MAP.at(op);
		}
		catch (const std::exception&)
		{
			return 0;
		}
		return p;
	}


	Priority getOperatorPrecedence(lexer::Lexer::Token token)
	{
		if (!isOperatorToken(token))
		{
			return 0;
		}

		return getOperatorPrecedence(token.type);
	}
}
}
