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

		json getTree()
		{
			return tree;
		}

	private:
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
				kind_description = "boolean";
				break;
			case lexer::Lexer::TokenType::NUMBER:
				kind = number;
				kind_description = "number";
				break;
			case lexer::Lexer::TokenType::STRING:
				kind = string;
				kind_description = "string";
				break;
			case lexer::Lexer::TokenType::KEYWORD_NONE:
				kind = none;
				kind_description = "none";
				break;
			case lexer::Lexer::TokenType::BIGINT:
				kind = bigint;
				kind_description = "bigint";
				break;
			case lexer::Lexer::TokenType::RATIONAL:
				kind = rational;
				kind_description = "rational";
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
		std::string value;
		LiteralKind kind;
		std::string kind_description;

		json getTree()
		{
			return tree;
		}

	private:
		json tree;
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
		std::string value;
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

	private:
		json tree;
	};

	class IDNode : public Expression
	{
	public:
		IDNode(std::string name)
			: name(name)
		{
			tree = {
				{ "description", "identifier" },
			    { "name", name },
			};
		}
		std::string name;

	private:
		json tree;
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
		void push(std::string name, std::shared_ptr<Expression> expr);

	private:
		json tree;
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

	private:
		json tree;
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
				{ "left", left->getTree() },
			    { "right", right->getTree() },
			};
		}
		Kind kind;
		std::shared_ptr<Expression> left;
		std::shared_ptr<Expression> right;

	private:
		json tree;
	};

	class TernaryNode : public Expression
	{
	public:
		std::shared_ptr<Expression> condition;
		std::shared_ptr<Expression> left;
		std::shared_ptr<Expression> right;

	private:
		json tree;
	};

	class CallingNode : public Expression
	{
	public:
		std::shared_ptr<Expression> function;
		std::vector<std::shared_ptr<Expression>> arguments;

	private:
		json tree;
	};
}
}
}

#endif // !_FLANER_PARSER_EXPRESSION_HH_
