#ifndef _FLANER_PARSER_EXPRESSION_HH_
#define _FLANER_PARSER_EXPRESSION_HH_

#include <lexer/lexer.hh>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <deps/json.hpp>
#include <deps/enum.h>

namespace flaner
{
namespace parser
{
namespace syntax
{
	using Kind = lexer::Lexer::TokenType;
	using Token = lexer::Lexer::Token;
	using json = nlohmann::json;

	class Expression
	{
	public:
		Expression() {}
		virtual ~Expression() {}
		virtual std::wstring stringify() = delete;

		json tree;
	};

	class BaseLiteralNode : public Expression
	{
	public:
		enum LiteralKind
		{
			boolean,
			number,
			bigint,
			rational,
			string,
			none,
		};
		BaseLiteralNode() {}
		BaseLiteralNode(lexer::Lexer::Token token)
		{
			value = token.value;
			switch (token.type)
			{
			case lexer::Lexer::TokenType::KEYWORD_TRUE:
			case lexer::Lexer::TokenType::KEYWORD_FALSE:
				kind = boolean;
				kind_description = L"boolean";
				break;
			case lexer::Lexer::TokenType::NUMBER:
				kind = number;
				kind_description = L"number";
				break;
			case lexer::Lexer::TokenType::STRING:
				kind = string;
				kind_description = L"string";
				break;
			case lexer::Lexer::TokenType::KEYWORD_NONE:
				kind = none;
				kind_description = L"none";
				break;
			case lexer::Lexer::TokenType::BIGINT:
				kind = bigint;
				kind_description = L"bigint";
				break;
			case lexer::Lexer::TokenType::RATIONAL:
				kind = rational;
				kind_description = L"rational";
				break;
			default:
				// TODO...
				break;
			}

			tree = {
				{ "description", "base-literal" },
				{ "type", kind_description },
				{ "value", value },
			};
		}
		std::wstring value;
		LiteralKind kind;
		std::wstring kind_description;
	};


	class PrimaryNode : public Expression
	{
	public:

	};

	class Numeric : public PrimaryNode
	{
	public:
		Numeric(Token source)
		{
			// TODO...
		}
		std::variant<int64_t, double> value;
	};

	class BigInt : public PrimaryNode
	{
	public:
		BigInt(Token token)
		{
			
		}
	};

	class Rational : public PrimaryNode
	{
	public:
		Rational(Token token)
		{

		}
	};

	class String : public PrimaryNode
	{
	public:
		String(Token source)
		{

		}
		std::wstring value;
	};

	class Boolean : public PrimaryNode
	{
	public:
		Boolean(Token source)
		{

		}
		bool value;
	};

	class None : public PrimaryNode
	{
	public:
	};

	class ListLiteral : public PrimaryNode
	{
	public:
		void push(std::shared_ptr<Expression> expr);
		void pop();
		std::vector<std::shared_ptr<Expression>> members;
	};

	class IDNode : public Expression
	{
	public:
		IDNode(std::wstring name)
			: name(name)
		{
			tree = {
				{ "description", "identifier" },
			    { "name", name },
			};
		}
		std::wstring name;
	};

	class ObjectLiteral : public PrimaryNode
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

	class UnaryNode : public Expression
	{
	public:
		UnaryNode(Kind kind, std::shared_ptr<Expression> val = {})
			: kind(kind), 
			right(val)
		{}
		Kind kind;
		std::shared_ptr<Expression> right;
	};

	class BinaryNode : public Expression
	{
	public:
		BinaryNode(Kind kind, std::shared_ptr<Expression> val1 = {}, std::shared_ptr<Expression> val2 = {})
			: kind(kind),
			left(val1),
			right(val2)
		{

			tree = {
				{ "description", "binary-expression" },
			    { "type", static_cast<int>(kind) },
				{ "left", left->tree },
			    { "right", right->tree },
			};
		}
		Kind kind;
		std::shared_ptr<Expression> left;
		std::shared_ptr<Expression> right;
	};

	class TernaryNode : public Expression
	{
	public:
		std::shared_ptr<Expression> condition;
		std::shared_ptr<Expression> left;
		std::shared_ptr<Expression> right;
	};

	class CallingNode : public Expression
	{
	public:
		std::shared_ptr<Expression> function;
		std::vector<std::shared_ptr<Expression>> arguments;
	};
}
}
}

#endif // !_FLANER_PARSER_EXPRESSION_HH_
