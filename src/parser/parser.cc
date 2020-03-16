#include <parser/parser.hh>


namespace flaner
{
namespace parser
{
    syntax::Expression Parser::parseExpression(Token token, Priority priority)
    {
        syntax::ExpressionStatement stm;
		stm.expression = {};

		if (token.type == Type::IDENTIFIER)
		{
			return syntax::IDNode(token.value);
		}

		if (token.type == Type::OP_PAREN_BEGIN)
		{

		}

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
	std::vector<Parser::Token> Parser::shuntingYard()
	{
		std::vector<Token> output;
		std::stack<Token> stack;

		uint16_t parensCount = 0;

		while (true)
		{
			Token token = lexer.now();
			// 字面值可以直接移入输出流中
			// TODO: 添加对复杂字面值的支持
			if (isBaseLiteral(token))
			{
				output.push_back(token);
			}
			else if (isIdentifier(token))
			{
				if (lexer.forwards().type == Type::OP_PAREN_BEGIN)
				{
					stack.push(token);
				}
				else
				{
					output.push_back(token);
				}
			}
			else if (token.type == Type::OP_COMMA)
			{
				bool parenBegin = false;
				while (!stack.empty())
				{
					Token el = stack.top();
					if (el.type == Type::OP_PAREN_BEGIN)
					{
						parenBegin = true;
						break;
					}
					else
					{
						output.push_back(el);
						stack.pop();
					}
				}
				throw new UnexpectedToken_SyntaxError(token);
			}
			else if (isOperator(token))
			{
				while (!stack.empty())
				{
					Token el = stack.top();
					if (isOperator(el) &&
						((isLeftAssociation(token) && (getPriority(token) <= getPriority(el))) ||
						(!isLeftAssociation(token) && (getPriority(token) < getPriority(el)))))
					{
						output.push_back(el);
						stack.pop();
					}
					else
					{
						break;
					}
				}
				stack.push(token);
			}
			else if (token.type == Type::OP_PAREN_BEGIN)
			{
				parensCount += 1;
				stack.push(token);
			}
			else if (token.type == Type::OP_PAREN_END)
			{
				parensCount -= 1;
				bool parenBegin = false;
				while (!stack.empty()) {
					Token el = stack.top();
					if (el.type == Type::OP_PAREN_BEGIN)
					{
						parenBegin = true;
						break;
					}
					else
					{
						output.push_back(el);
						stack.pop();
					}
					if (!parenBegin)
					{
						throw new UnexpectedToken_SyntaxError(token);
					}

					stack.pop();
					if (!stack.empty())
					{
						Token el = stack.top();
						if (el.type == Type::IDENTIFIER)
						{
							output.push_back(el);
							stack.pop();
						}
					}


				}
			}

			// 不属于表达式的 token
			else
			{
				// 若此时还处于小括号包裹内
				if (parensCount != 0)
				{
					throw new UnexpectedToken_SyntaxError(token);
				}

				// 已经从输入流中获取到了完整的表达式，现在结束
				break;
			}

			lexer.next();
		}

		// 处理 stack 中余留的 tokens
		while (!stack.empty())
		{
			Token el = stack.top();
			if (el.type == Type::OP_PAREN_BEGIN || el.type == Type::OP_PAREN_END)
			{
				throw new UnexpectedToken_SyntaxError(el);
			}
			output.push_back(el);
			stack.pop();
		}

		// 完成
		return output;
	}

}
}
