#ifndef _FLANER_PARSER_STATEMENT_HH_
#define _FLANER_PARSER_STATEMENT_HH_

#include <lexer/token.hh>
#include <parser/expression.hh>
#include <vector>

namespace flaner
{
namespace parser
{
namespace syntax
{
	class Statement
	{
	public:
		Statement()
		{

		}
	};

	class StatementSequence
	{
	public:
		void insert(std::shared_ptr<Statement> statement);
		bool isEnd();
		std::shared_ptr<Statement> head();
		std::shared_ptr<Statement> next();
	private:
		std::vector<std::shared_ptr<Statement>> sequence;
		size_t offset;
	};

	class NullStatement : public Statement
	{
	public:
	};

	class BlockStatement : public Statement
	{
	public:
		BlockStatement() : body({}) {}
		BlockStatement(StatementSequence sequence) : body(sequence) {}

	private:
		StatementSequence body;
	};

	class ExpressionStatement : public Statement
	{
	public:
		ExpressionStatement(std::shared_ptr<Expression> expr)
		    : expression(expr)
		{

		}
		std::shared_ptr<Expression> expression;
	};



}
}
}

#endif // !_FLANER_PARSER_STATEMENT_HH_
