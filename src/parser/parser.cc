#include "parser.hh"
#include "parser.hh"
#include "parser.hh"
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

	std::shared_ptr<ExprAST> Parser::expression()
	{

	}

	std::shared_ptr<ExprAST> Parser::identifier()
	{
		std::string idName = lexer.now();

		if (lexer.go().value != "(")
		{
			return std::make_shared<VariableExprAST(idName)>();
		}

		auto cur = lexer.go();

		std::vector<std::make_shared<ExprAST>> args{};

		if (cur.value != ")")
		{
			while (true)
			{
				std::shared_ptr<ExprAST> arg = expression();
				if (!arg)
				{
					return 0;
				}
				args.push_back(arg);

				if (cur.value != ")")
				{
					break;
				}

				if (cur.value != ",")
				{
					// TODO...
				}

				cur = lexer.go();
			}
		}

		lexer.go();

		return std::make_shared<CallExprAST>{ idName, args };
	}

	std::shared_ptr<ExprAST> Parser::number()
	{
		std::shared_ptr<ExprAST> result = std::make_shared<NumberExprAST>();
		lexer.go();
		return result;
	}

	std::shared_ptr<ExprAST> Parser::paren()
	{
		lexer.go();
		auto v = expression();
		if (!v)
		{
			return nullptr;
		}
		if (lexer.now().value != ")")
		{
			// TODO...
		}
		lexer.go();
		return v;
	}

	std::shared_ptr<ExprAST> Parser::primary()
	{
		auto cur = lexer.now();
		switch (cur.type)
		{
		case Type::IDENTIFIER:
			return identifier();
		case Type::NUMBER:
			return number();
		case Type::OP_PAREN_BEGIN:
			return paren();
		default:
			// TODO...
			return nullptr;
		}
	}

}
}
