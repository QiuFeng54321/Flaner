#ifndef _FLANER_PARSER_AST_HH_
#define _FLANER_PARSER_AST_HH_

#include <vector>
#include <variant>
#include <map>
#include <lexer/lexer.hh>

namespace flaner
{
namespace parser
{
	using Operator = lexer::Lexer::TokenType;

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
		BinaryExprAST(Operator op, std::shared_ptr<ExprAST> lhs, std::shared_ptr<ExprAST> rhs)
			: op(op), lhs(lhs), rhs(rhs) {}
	};

	class CallExprAST : public ExprAST
	{
		std::string callee;
		std::vector<std::shared_ptr<ExprAST>> arguments;
	public:
		CallExprAST(const std::string& s, std::vector<std::shared_ptr<ExprAST>>& args)
			: callee(s), arguments(args) {}
	};

	class FunctionDefAST
	{
		std::vector<std::string> params;
	public:
		FunctionDefAST(std::vector<std::string>& params)
			: params(params) {}
	};

	class FunctionExprAST : public ExprAST
	{
		std::shared_ptr<FunctionDefAST> def;
		std::shared_ptr<ExprAST> body;
	public:
		FunctionExprAST(std::shared_ptr<FunctionDefAST> def, std::shared_ptr<ExprAST> expr)
			: def(def), body(expr) {}
	};

}
}

#endif // !_FLANER_PARSER_AST_HH_
