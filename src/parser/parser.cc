#include <parser/parser.hh>


namespace flaner
{
namespace parser
{
    std::shared_ptr<syntax::ListLiteral> Parser::parseListLiteral()
    {
        return std::shared_ptr<syntax::ListLiteral>();
    }
    std::shared_ptr<syntax::ObjectLiteral> Parser::parseObjectLiteral()
    {
        return std::shared_ptr<syntax::ObjectLiteral>();
    }
    std::shared_ptr<syntax::Expression> Parser::parsePrimary()
    {
        std::shared_ptr<syntax::Expression> primary;

        Token now = lexer.now();
        switch (now.type)
        {
        case Type::NUMBER:
            primary = std::make_shared<syntax::Numeric>(now);
            break;
        case Type::STRING:
            primary = std::make_shared<syntax::String>(now);
            break;
        case Type::BIGINT:
            primary = std::make_shared<syntax::BigInt>(now);
            break;
        case Type::RATIONAL:
            primary = std::make_shared<syntax::Rational>(now);
            break;
        case Type::KEYWORD_TRUE:
        case Type::KEYWORD_FALSE:
            primary = std::make_shared<syntax::Boolean>(now);
            break;
        case Type::KEYWORD_NONE:
            primary = std::make_shared<syntax::None>();
            break;
        case Type::OP_BRACKET_BEGIN:
            primary = parseListLiteral();
            break;
        case Type::OP_BRACE_BEGIN:
        {
            // 仅当 '{' 跟随于操作符后面时，作为对象字面量解析
            if (isOperator(lexer.backwards()))
            {
                primary = parseObjectLiteral();
            }
            else
            {
                // TODO...
            }
        }
        default:
            break;
        }

        return primary;
    }

    std::shared_ptr<syntax::Expression> Parser::parseExpression(
        std::shared_ptr<syntax::Expression> lhs, Priority base)
    {
        Token token = lexer.forwards();
        while (isBinaryOperator(token) && getPriority(token) >= base)
        {
            Token op = token;
            lexer.next();

            auto rhs = parsePrimary();

            token = lexer.forwards();

            while (isBinaryOperator(token) && getPriority(token) > getPriority(op)
                || isRightAssociation(token) && getPriority(token) == getPriority(op))
            {
                rhs = parseExpression(rhs, getPriority(token));
                token = lexer.forwards();
            }

            lhs = std::make_shared<syntax::BinaryNode>(op.type, lhs, rhs);
        }
        return lhs;
    }

    syntax::StatementSequence Parser::getProgram()
    {
        return program;
    }

    std::vector<Parser::Token> Parser::shuntingYard()
    {
        std::stack<Token> operatorStack;
        std::vector<Token> outputQueue;

        auto pop = [&]() {
            auto item = operatorStack.top();
            operatorStack.pop();
            return item;
        };

        while (!lexer.isEnd())
        {
            Token token = lexer.now();

            if (isBaseLiteral(token))
            {
                outputQueue.push_back(token);
            }
            else if (isIdentifier(token))
            {
                // 如果标识符表示一个函数，那么放入栈中
                if (lexer.forwards().type == Type::OP_PAREN_BEGIN)
                {
                    operatorStack.push(token);
                }
                else
                {
                    outputQueue.push_back(token);
                }
            }
            else if (token.type == Type::OP_COMMA)
            {
                while (operatorStack.top().type != Type::OP_PAREN_BEGIN)
                {
                    outputQueue.push_back(pop());
                }
            }
            else if (isOperator(token))
            {
                Token el = operatorStack.top();
                if (isOperator(el))
                {
                    while (isLeftAssociation(token) && (getPriority(token) <= getPriority(el))
                        || isRightAssociation(token) && (getPriority(token) < getPriority(el)))
                    {
                        outputQueue.push_back(pop());
                    }
                }
                operatorStack.push(token);
            }

            lexer.next();
        }

        return std::vector<Token>();
    }
    

    std::pair<std::vector<Parser::Token>, std::vector<Parser::Token>> Parser::convertExpression()
    {
        return std::pair<std::vector<Token>, std::vector<Token>>();
    }

    std::shared_ptr<syntax::ExpressionStatement> Parser::parseExpressionStatement()
    {
        auto stm = std::make_shared<syntax::ExpressionStatement>(
            parseExpression(parsePrimary(), static_cast<Priority>(0)));
        program.insert(stm);
        return stm;
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

    bool Parser::isEndOperator(Token token)
    {
        switch(token.type)
        {
        case Type::OP_PAREN_END:
        case Type::OP_BRACKET_END:
        case Type::OP_BRACE_END:
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

    bool Parser::isAssignment(Token token)
    {
        return false;
    }

    bool Parser::isLeftAssociation(Token token)
    {
        std::unordered_set<Type> set
        {
            Type::OP_DOT,
            Type::OP_ADD,
            Type::OP_MINUS,
            Type::OP_MUL,
            Type::OP_DIV,
            Type::OP_INTDIV,
            Type::OP_MOD,
            Type::OP_QUOTE,

        };
        return set.find(token.type) != set.end();
    }

    bool Parser::isRightAssociation(Token token)
    {
        std::unordered_set<Type> set
        {
            Type::OP_POW,

        };
        return set.find(token.type) != set.end();
    }

    Parser::Priority Parser::getPriority(Token token, bool isNegation)
    {
        switch (token.type)
        {
        case Type::OP_COMMA:
            return Priority::comma;
        case Type::OP_DOT_DOT_DOT:
            return Priority::spread;
        case Type::KEYWORD_YIELD:
            return Priority::yield;
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

    bool Parser::isBinaryOperator(Token token)
    {
        return false;
    }

}
}
