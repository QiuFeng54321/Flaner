#include <parser/statement.hh>

namespace flaner
{
namespace parser
{
namespace syntax
{
    void StatementSequence::insert(std::shared_ptr<Statement> statement)
    {
        sequence.push_back(statement);
    }
    bool StatementSequence::isEnd()
    {
        return offset + 1 == sequence.size();
    }
    std::shared_ptr<Statement> StatementSequence::head()
    {
        return sequence.front();
    }
    std::shared_ptr<Statement> StatementSequence::next()
    {
        offset += 1;
        return sequence.at(offset - 1);
    }
}
}
}