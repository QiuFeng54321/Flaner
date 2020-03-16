#include <parser/parser.hh>


namespace flaner
{
namespace parser
{
    syntax::Expression Parser::parseExpression(Token token, Priority priority)
    {
        return {};
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

            // 如果标识符后跟随 '('，那么它是一个函数名，放入栈中
            // 否则只是普通的值，直接移入输出流
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

            // 从栈当中不断地弹出操作符并且放入输出队列中，直到栈顶部的元素为 '(' 为止
            // 将 '(' 从栈的顶端弹出，但并不放入输出队列中去
            // 如果此时位于栈顶端的记号表示一个函数，那么将其弹出并放入输出队列中去
            // 如果在找到 '(' 之前栈就已经弹出了所有元素，那么就表示在表达式中存在不匹配的括号
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

            // 如果 token 是一个运算符
            // 只要栈的顶端存在另一个运算符 el，并且
            //     如果 token 是左结合的，且它的优先级 ≤ el 的优先级，或者
            //     如果 token 是右结合的，且它的优先级 ≤ el 的优先级，那么
            //     将 el 从栈的顶端弹出并且放入输出队列中（循环直至以上条件不满足为止）
            //     然后，将 el 压入栈的顶端。
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

	
    bool Parser::isBaseLiteral(Token token)
    {
        switch (token.type)
        {
        case Type::NUMBER:
        case Type::KEYWORD_TRUE:
        case Type::KEYWORD_FALSE:
        case Type::STRING:
        case Type::KEYWORD_NONE:
            return true;
        default:
            return false;
        }
    }

    bool Parser::isBeginOperator(Token token)
    {
        switch (token.type)
        {
        case Type::OP_PAREN_BEGIN:
        case Type::OP_BRACKET_BEGIN:
        case Type::OP_BRACE_BEGIN:
            return true;
        default:
            return false;
        }
    }

    bool Parser::isKeyword(Token token)
    {
        auto map = lexer.getKeywordMap();
        for (auto i : map)
        {
            if (token.type == i.second)
            {
                return true;
            }
        }
        return false;
    }

    bool Parser::isIdentifier(Token token)
    {
        return token.type == Type::IDENTIFIER;
    }

    bool Parser::isOperator(Token token)
    {
        auto set = lexer.getOperatorSet();
        return set.find(token.type) != set.end();
    }

    bool Parser::isLeftAssociation(Token token)
    {
		std::unordered_set<Type> set
		{
			Type::OP_DOT,
			Type::OP_MUL,
			Type::OP_DIV,
			Type::OP_INTDIV,
			Type::OP_MOD,
			Type::OP_QUOTE,

		};
		return set.find(token.type) != set.end();
    }

	Parser::Priority Parser::getPriority(Token token, bool isNegation)
	{
		switch (token.type)
		{
		case Type::OP_COMMA: return Priority::comma;
		case Type::OP_DOT_DOT_DOT: return Priority::spread;
		case Type::KEYWORD_YIELD: return Priority::yield;
		case Type::OP_ADD_ASSIGN:
		case Type::OP_MINUS_ASSIGN:
		case Type::OP_MUL_ASSIGN:
		case Type::OP_DIV_ASSIGN:
		case Type::OP_INTDIV_ASSIGN:
		case Type::OP_MOD_ASSIGN:
		case Type::OP_QUOTE_ASSIGN:
		case Type::OP_POW_ASSIGN:
		case Type::OP_BIT_OR_ASSIGN:
		case Type::OP_BIT_AND_ASSIGN:
		case Type::OP_BIT_XOR_ASSIGN:
		case Type::OP_SHIFT_LEFT_ASSIGN:
		case Type::OP_SHIFT_RIGHT_ASSIGN:
		case Type::OP_ASSIGN:
			return Priority::assignment;
		case Type::OP_LOGIC_OR:
			return Priority::logical_or;
		case Type::OP_LOGIC_AND:
			return Priority::logical_and;
		case Type::OP_BIT_OR:
			return Priority::bit_or;
		case Type::OP_BIT_XOR:
			return Priority::bit_xor;
		case Type::OP_BIT_AND:
			return Priority::bit_and;
		case Type::OP_EQUAL:
		case Type::OP_NOT_EUQAL:
			return Priority::equality;
		case Type::OP_LESS_THAN:
		case Type::OP_LESS_EQUAL:
		case Type::OP_GREATER_THAN:
		case Type::OP_GREATER_EQUAL:
			return Priority::relationship;
		case Type::OP_SHIFT_LEFT:
		case Type::OP_SHIFT_RIGHT:
			return Priority::bit_shift;
		case Type::OP_MINUS:
			if (isNegation)
			{
				return Priority::unary_operator;
			}
		case Type::OP_ADD:
			return Priority::addtion;
		case Type::OP_MUL:
		case Type::OP_DIV:
		case Type::OP_INTDIV:
		case Type::OP_MOD:
		case Type::OP_QUOTE:
			return Priority::multition;
		case Type::OP_POW:
			return Priority::power;
		case Type::OP_DOT:
			return Priority::visitor;
		default:
			// TODO...
			break;
		};
	}

}
}
