#include <parser/statement.hh>

namespace flaner
{
namespace parser
{
namespace syntax
{
	void StatementSequence::insert(Statement statement)
	{
		sequence.push_back(statement);
	}
	bool StatementSequence::isEnd()
	{
		return offset + 1 == sequence.size();
	}
	Statement StatementSequence::head()
	{
		return sequence.front();
	}
	Statement StatementSequence::next()
	{
		offset += 1;
		return sequence.at(offset - 1);
	}
}
}
}