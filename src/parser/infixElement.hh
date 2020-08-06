#ifndef _FLANER_PARSER_INFIXELEMENT_HH_
#define _FLANER_PARSER_INFIXELEMENT_HH_

#include <vector>
#include <parser/expressionUnit.hh>

namespace flaner
{
namespace parser
{
	class InfixElement
	{
	public:
		struct Head
		{
			std::vector<ExpressionUnit> units;
		};

		struct Ternary
		{
			bool assign;
			std::vector<ExpressionUnit> branchTrue;
			std::vector<ExpressionUnit> branchFalse;
		};

		struct LogicalAnd
		{
			bool assign;
			std::vector<ExpressionUnit> branchTrue;
		};

		struct LogicalOr
		{
			bool assign;
			std::vector<ExpressionUnit> branchFalse;
		};

		enum class Op : uint16_t;

		// 不存在短路的运算
		struct General
		{
			Op op;
			bool assign;
			std::vector<ExpressionUnit> right;
		};

		enum class index : uint16_t
		{
			head,
			ternary,
			logicalAnd,
			logicalOr,
			coalescence,
			general,
		};

		using Storage = std::variant <
			// TODO...
			Head,
			Ternary,
			LogicalAnd,
			LogicalOr,
			General
		>;

	private:
		Storage storage;
	};
}
}



#endif // !_FLANER_PARSER_INFIXELEMENT_HH_
