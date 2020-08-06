#include <parser/parser.hh>

namespace flaner
{
namespace parser
{
	static const std::map<std::string, Priority> PRECEDENCE_MAP =
	{
		{ "+", Priority::addtion },
		{ "-", Priority::addtion },
		{ "*", Priority::multition },
		{ "/", Priority::multition },
		{ "%", Priority::multition },
		{ ">", Priority::comparision },
		{ ">=", Priority::comparision },
		{ "<", Priority::comparision },
		{ "<=", Priority::comparision },
		{ "==", Priority::comparision },
		{ "!=", Priority::comparision },
	};



	Priority getOperatorPrecedence(Operator op)
	{

	}
}
}
