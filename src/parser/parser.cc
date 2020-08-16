#include "parser.hh"
#include "parser.hh"
#include "parser.hh"
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
			return {};
		}
		return p;
	}

	Priority getOperatorPrecedence(lexer::Lexer::Token token)
	{
		if (!isOperatorToken(token))
		{
			return {};
		}

		return getOperatorPrecedence(token.type);
	}

	bool isOperatorToken(Token token)
	{
		return token.type >= Type::OP_ADD && token.type <= Type::OP_ASSIGN
			|| token.type == Type::OP_PAREN_BEGIN || token.type == Type::OP_PAREN_END;
	}

	std::shared_ptr<ExprAST> Parser::expression()
	{
		auto lhs = primary();
		if (!lhs)
		{
			return nullptr;
		}

		return binaryOperatorRightSide(Priority{}, lhs);
	}

	std::shared_ptr<ExprAST> Parser::identifier()
	{
		std::string idName = lexer.now().value;

		if (lexer.go() != Type::OP_PAREN_BEGIN)
		{
			return std::make_shared<VariableExprAST>(idName);
		}

		auto cur = lexer.go();

		std::vector<std::shared_ptr<ExprAST>> args{};

		if (cur != Type::OP_PAREN_END)
		{
			while (true)
			{
				std::shared_ptr<ExprAST> arg = expression();
				if (!arg)
				{
					return 0;
				}
				args.push_back(arg);

				if (cur != Type::OP_PAREN_END)
				{
					break;
				}

				if (cur != Type::OP_COMMA)
				{
					// TODO...
				}

				cur = lexer.go();
			}
		}

		lexer.go();

		return std::make_shared<CallExprAST>(idName, args);
	}

	std::shared_ptr<ExprAST> Parser::number()
	{
		auto val = lexer.now().value;
		
		// TODO...
		std::shared_ptr<NumberExprAST> result = std::make_shared<NumberExprAST>(std::atoi(val.c_str()));
		lexer.go();
		return result;
	}

	std::shared_ptr<ExprAST> Parser::paren()
	{
		lexer.go();
		if (lexer.tryFindingAfter(Type::IDENTIFIER | Type::OP_COMMA, Type::OP_PAREN_END, Type::FUNCTION_ARROW))
		{
			lexer.last();
			return nullptr;
			//return std::make_shared<ExprAST>(function());
		}
		else
		{
			auto v = expression();
			if (!v)
			{
				return nullptr;
			}
			if (lexer.now() != Type::OP_PAREN_END)
			{
				// TODO...
			}
			lexer.go();
			return v;
		}
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

	std::shared_ptr<ExprAST> Parser::binaryOperatorRightSide(Priority exprPrec, std::shared_ptr<ExprAST> lhs)
	{
		while (true)
		{
			auto op = lexer.now();
			Priority prec = getOperatorPrecedence(op);

			if (prec < exprPrec)
			{
				return lhs;
			}

			lexer.go();

			auto rhs = primary();

			if (!rhs)
			{
				return nullptr;
			}

			Priority nextPrec = getOperatorPrecedence(lexer.now());

			if (prec < nextPrec)
			{
				rhs = binaryOperatorRightSide(Priority{ int(prec) + 1 }, rhs);
				if (!rhs)
				{
					return nullptr;
				}
			}

			lhs = std::make_shared<BinaryExprAST>(op, lhs, rhs);
		}
	}

	std::shared_ptr<FunctionDefAST> Parser::functionDef()
	{
		bool withParen = true;
		if (lexer.now() != Type::OP_PAREN_BEGIN)
		{
			withParen = false;
			if (lexer.now() != Type::IDENTIFIER || lexer.forwards() != Type::FUNCTION_ARROW)
			{
				// TODO...
				abort();
			}
		}
		lexer.go();
		std::vector<std::string> params{};
		while (lexer.now() == Type::IDENTIFIER)
		{
			params.push_back(lexer.now().value);
			if (lexer.go() == Type::OP_COMMA)
			{
				lexer.go();
			}
			if (lexer.now() != Type::IDENTIFIER)
			{
				if (withParen)
				{
					if (lexer.now() != Type::OP_PAREN_END)
					{
						// TODO...
						abort();
					}
					if (lexer.go() != Type::FUNCTION_ARROW)
					{
						// TODO...
						abort();
					}
				}
				else
				{
					if (lexer.now() != Type::FUNCTION_ARROW)
					{
						// TODO...
						abort();
					}
				}
			}
		}
		return std::make_shared<FunctionDefAST>(params);
	}
	std::shared_ptr<FunctionExprAST> Parser::function()
	{
		auto def = functionDef();
		if (!def)
		{
			return nullptr;
		}
		auto expr = expression();
		if (expr)
		{
			return std::make_shared<FunctionExprAST>(def, expr);
		}
		return nullptr;
	}
}
}