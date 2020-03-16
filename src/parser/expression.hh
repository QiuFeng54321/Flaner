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

	class Expression
	{
	public:
		Expression() {}
		virtual ~Expression() {}

	};

	class BaseLiteralNode : Expression
	{
	public:
		BaseLiteralNode(std::wstring val)
			: value(val)
		{}
		std::wstring value;
	};

	class ListLiteralNode 
	{
	public:
		void push(std::shared_ptr<Expression> expr);
		void pop();
		std::vector<std::shared_ptr<Expression>> members;
	};

	class ObjectLiteralNode 
	{
	public:
		class Item
		{
		public:
			std::shared_ptr<IDNode> key;
			std::shared_ptr<Expression> value;
		};
		std::vector<Item> members;
		void push(std::wstring name, std::shared_ptr<Expression> expr);
	};

	class IDNode : Expression
	{
	public:
		using ptr = std::shared_ptr<IDNode>;
		IDNode(std::wstring val)
			: name(val)
		{}
		std::wstring getName() const
		{
			return name;
		}
	private:
		std::wstring name;
	};

	class UnaryNode 
	{
	public:
		UnaryNode(Kind kind, std::shared_ptr<Expression> val = {})
			: kind(kind), 
			right(val)
		{}
		Kind kind;
		std::shared_ptr<Expression> right;
	};

	class BinaryNode 
	{
	public:
		BinaryNode(Kind kind, std::shared_ptr<Expression> val1 = {}, std::shared_ptr<Expression> val2 = {})
			: kind(kind),
			left(val1),
			right(val2)
		{}
		Kind kind;
		std::shared_ptr<Expression> left;
		std::shared_ptr<Expression> right;
	};

	class TernaryNode 
	{
	public:
		std::shared_ptr<Expression> condition;
		std::shared_ptr<Expression> left;
		std::shared_ptr<Expression> right;
	};

	class CallingNode 
	{
	public:
		std::shared_ptr<Expression> function;
		std::vector<std::shared_ptr<Expression>> arguments;
	};
}
}
}

#endif // !_FLANER_PARSER_EXPRESSION_HH_
