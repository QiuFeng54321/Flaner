#ifndef _FLANER_PARSER_PARSER_HH_
#define _FLANER_PARSER_PARSER_HH_

#include <string>
#include <map>
#include <parser/ast.hh>
#include <parser/error.hh>

namespace flaner
{
namespace parser
{
	using Token = lexer::Lexer::Token;
	using Type = lexer::Lexer::TokenType;

	enum class Priority : uint16_t
	{
		comma = 1,
		spread,
		yield,
		assignment,
		condition,
		logical_or,
		logical_and,
		bit_or,
		bit_xor,
		bit_and,
		comparision,
		bit_shift,
		addtion,
		multition,
		power,
		unary_operator,
		visitor,
		group,
	};

	Priority getOperatorPrecedence(Operator op);
	Priority getOperatorPrecedence(Token token);

	bool isOperatorToken(Token token);

	class Parser
	{
		lexer::Lexer lexer;

	public:
		std::shared_ptr<ExprAST> expression();
		std::shared_ptr<ExprAST> identifier();
		std::shared_ptr<ExprAST> number();
		std::shared_ptr<ExprAST> paren();
		std::shared_ptr<ExprAST> primary();
		std::shared_ptr<ExprAST> binaryOperatorRightSide(Priority exprPrec, std::shared_ptr<ExprAST> lhs);
		std::shared_ptr<FunctionDefAST> functionDef();
		std::shared_ptr<FunctionExprAST> function();
		std::shared_ptr<ExprAST> defintion();

	};

}
}

#endif // !_FLANER_PARSER_PARSER_HH_
