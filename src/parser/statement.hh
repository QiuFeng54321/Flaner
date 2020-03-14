#ifndef _FLANER_PARSER_STATEMENT_HH_
#define _FLANER_PARSER_STATEMENT_HH_

#include <lexer/token.hh>
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

	};

	class StatementSequence
	{
	public:
		void insert(Statement statement);
		bool isEnd();
		Statement head();
		Statement next();
	private:
		std::vector<Statement> sequence;
		size_t offset;
	};

	class NullStatement : Statement
	{
	public:
	};

	class BlockStatement : Statement
	{
	public:
		BlockStatement() : body({}) {}
		BlockStatement(StatementSequence sequence) : body(sequence) {}

	private:
		StatementSequence body;
	};

	class Expression : Statement
	{
	public:

	};


}
}
}

#endif // !_FLANER_PARSER_STATEMENT_HH_
