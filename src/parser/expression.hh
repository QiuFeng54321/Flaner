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
	using Kind = lexer::Lexer::TokenType;
	class Expression;

	class Expression
	{
	public:
	};

	class BaseLiteralNode : Expression
	{
	public:
		BaseLiteralNode(std::wstring val)
			: value(val)
		{}
		std::wstring value;
	};

	class ListLiteralNode : Expression
	{
	public:
		void push(Expression expr);
		void pop();
		std::vector<Expression> members;
	};

	class ObjectLiteralNode : Expression
	{
	public:
		class Item
		{
		public:
			IDNode key;
			Expression value;
		};
		std::vector<Item> members;
		void push(std::wstring name, Expression expr);
	};

	class IDNode : Expression
	{
	public:
		IDNode(std::wstring val)
			: name(val)
		{}
		std::wstring name;
	};

	class UnaryNode : Expression
	{
	public:
		UnaryNode(Kind kind, Expression val = Expression{})
			: kind(kind), 
			right(val)
		{}
		Kind kind;
		Expression right;
	};

	class BinaryNode : Expression
	{
	public:
		BinaryNode(Kind kind, Expression val1 = {}, Expression val2 = {})
			: kind(kind),
			left(val1),
			right(val2)
		{}
		Kind kind;
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
