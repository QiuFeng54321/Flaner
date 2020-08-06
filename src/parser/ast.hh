#ifndef _FLANER_PARSER_AST_HH_
#define _FLANER_PARSER_AST_HH_

#include <vector>
#include <variant>
#include <map>
#include <lexer/token.hh>

namespace flaner
{
namespace parser
{
	using Operator = lexer::TokenType;

	class ExprAST
	{
	public:
		virtual ~ExprAST() {}
	};

	class NumberExprAST : public ExprAST
	{
		double value;
	public:
		NumberExprAST(double val) : value(val) {}
	};

	class VariableExprAST : public ExprAST
	{
		std::string name;
	public:
		VariableExprAST(std::string& s) : name(s) {}
	};

	class BinaryExprAST : public ExprAST
	{
		Operator op;
		std::shared_ptr<ExprAST> lhs, rhs;
	public:
		BinaryExprAST(Operator op, ExprAST* lhs, ExprAST* rhs)
			: op(op), lhs(lhs), rhs(rhs) {}
	};

	class CallExprAST : public ExprAST
	{
		std::string callee;
		std::vector<ExprAST*> arguments;
	public:
		CallExprAST(const std::string& s, std::vector<ExprAST*>& args)
			: callee(s), arguments(args) {}
	};

}
}

#endif // !_FLANER_PARSER_AST_HH_
