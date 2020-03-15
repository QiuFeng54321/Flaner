#include <parser/parser.hh>


namespace flaner
{
namespace parser
{
    void Parser::parseExpression(Token token, Priority priority)
    {
        syntax::ExpressionStatement stm;
        stm.expression = syntax::Expression();

		bool inExpression = true;

		while (inExpression && !lexer.isEnd())
		{
			if (isOperator(token))
			{
				if (isUnaryOperator(token))
				{
					parseExpression(lexer.next(), )
					syntax::UnaryNode node{ token.type,  };
				}
			}
		}

		// 3 * 6 + (2 - 8)
		while (
			(isOperator(token) || isBaseLiteral(token) || isIdentifier(token))
			&& !lexer.isEnd())
		{
			if (isBaseLiteral(token))
			{
				syntax::LiteralNode node(token.value);
			}
			else if (isOperator(token))
			{
				
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
