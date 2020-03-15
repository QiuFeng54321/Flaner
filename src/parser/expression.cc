#include <parser/expression.hh>

namespace flaner
{
namespace parser
{
namespace syntax
{
	void ListLiteralNode::push(Expression expr)
	{
		members.push_back(expr);
	}
	void ListLiteralNode::pop()
	{
		members.pop_back();
	}
	void ObjectLiteralNode::push(std::wstring name, Expression expr)
	{
		members.push_back({ name, expr });
	}
}
}
}
