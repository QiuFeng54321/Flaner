#include <parser/parser.hh>


namespace flaner
{
namespace parser
{
	lexer::Lexer::Token Parser::getNextToken()
	{
		tokenOffset += 1;
		return tokenStream.at(tokenOffset - 1);
	}
	void Parser::parseExpression()
	{
		syntax::ExpressionStatement stm;
		stm.expression = syntax::Expression();
		if (isLiteral(getNextToken()))
		{

		}
	}

	syntax::StatementSequence Parser::getProgram()
	{
		return syntax::StatementSequence();
	}
	bool Parser::isLiteral()
	{
		return false;
	}
}
}
