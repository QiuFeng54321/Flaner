#include <parser/expression.hh>

namespace flaner
{
namespace parser
{
namespace syntax
{
	void ListLiteral::push(std::shared_ptr<Expression> expr)
	{
		members.push_back(expr);
	}
	void ListLiteral::pop()
	{
		members.pop_back();
	}
	void ObjectLiteral::push(std::string name, std::shared_ptr<Expression> expr)
	{
		members.push_back({ std::make_shared<IDNode>(name), expr });
	}
}
}
}
