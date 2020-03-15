#include <parser/parser.hh>


namespace flaner
{
namespace parser
{
    void Parser::parseExpression(Token token)
    {
        syntax::ExpressionStatement stm;
        stm.expression = syntax::Expression();

		// 3 * 6 + (2 - 8)
		while (
			(isOperator(token) || isLiteral(token) || isIdentifier(token))
			&& !lexer.isEnd())
		{
			if (isBaseLiteral(token))
			{
				syntax::LiteralNode node(token.value);
			}
			else if (isOperator(token))
			{
				if (isUnaryOperator(token))
				{
					syntax::UnaryNode node(token.type, token.value );
				}
			}
			Token token = lexer.next();
		}
    }

    syntax::StatementSequence Parser::getProgram()
    {
        return syntax::StatementSequence();
    }
}
}
