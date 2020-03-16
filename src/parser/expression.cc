#include <parser/expression.hh>

namespace flaner
{
namespace parser
{
namespace syntax
{
	void ListLiteralNode::push(std::shared_ptr<Expression> expr)
	{
		members.push_back(expr);
	}
	void ListLiteralNode::pop()
	{
		members.pop_back();
	}
	void ObjectLiteralNode::push(std::wstring name, std::shared_ptr<Expression> expr)
	{
		members.push_back({ std::make_shared<IDNode>(name), expr });
	}
}
}
}
