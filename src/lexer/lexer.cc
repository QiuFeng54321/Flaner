#include <lexer/lexer.hh>

namespace flaner
{
namespace lexer
{
    bool Lexer::isBlank(char ch)
    {
        std::string blanks = "\n\r\t\f \x0b\xa0\u2000"
            "\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009"
            "\u200a\u200b\u2028\u2029\u3000";
        for (auto i = blanks.begin(); i != blanks.end(); i++)
        {
            if (*i == ch)
            {
                return true;
            }
        }
        return false;
    }

    Lexer::TokenType Lexer::getKeywordOrID(std::string s)
    {
        TokenType type;

        try
        {
            type = keywordMap.at(s);
        }
        catch (const std::exception&)
        {
            type = TokenType::IDENTIFIER;
        }
        return type;
    }

    std::vector<Lexer::Token> Lexer::process()
    {
        auto push = [&](TokenType t, std::string v) {
			try
			{
				sequence.push_back({ t, v });
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				abort();
			}
        };
        auto next = [&](size_t offset = 1) {
            return context.getNextchar(offset);
        };
        auto lastToken = [&]() {
            return sequence.at(sequence.size() - 1);
        };

        while (!context.isEnd())
        {
            char ch = next();

            if (isBlank(ch))
            {
                continue;
            }

            auto match = [&](char s) {
                return ch == s;
            };
            auto test = [&](char s, size_t offset = 1) {
                return context.lookNextchar(offset) == s;
            };

            if (iswdigit(ch))
            {
                std::string s{ ch };
                char nextchar = context.lookNextchar(1);
                while (iswdigit(nextchar))
                {
                    ch = next();
                    nextchar = context.lookNextchar(1);
                    s += ch;
                }
                push(TokenType::NUMBER, s);
            }      
            else if (iswalpha(ch) || ch == L'_' || ch == L'$')
            {
                std::string word{ ch };
                char nextchar = context.lookNextchar(1);
                while (iswalnum(nextchar) || ch == L'_' || ch == L'$')
                {
                    ch = next();
                    nextchar = context.lookNextchar(1);
                    word += ch;
                }
                if (sequence.size() != 0 && sequence.back().type == TokenType::OP_DOT)
                {
                    push(TokenType::IDENTIFIER, word);
                }
                else
                {
                    push(getKeywordOrID(word), word);
                }
            }
            else if (match('+'))
            {
                if (test('='))
                {
                    push(TokenType::OP_ADD_ASSIGN, "+=");
                    next();
                }
                else
                {
                    push(TokenType::OP_ADD, "+");
                }
            }
            else if (match('-'))
            {
                if (test('='))
                {
                    push(TokenType::OP_MINUS_ASSIGN, "-=");
                    next();
                }
                else
                {
                    push(TokenType::OP_MINUS, "-");
                }
            }
            else if (match('*'))
            {
                if (test('*'))
                {
                    push(TokenType::OP_POW, "**");
                    next();
                }
                else
                {
                    if (test('='))
                    {

                        push(TokenType::OP_MUL_ASSIGN, "*=");
                        next();
                    }
                    else
                    {
                        push(TokenType::OP_MUL, "*");
                    }
                    next();
                }
            }
            else if (match('/'))
            {
                if (test('/'))
                {
                    push(TokenType::OP_INTDIV_ASSIGN, "//");
                    next();
                }
                else
                {
                    if (test('='))
                    {
                        push(TokenType::OP_DIV_ASSIGN, "/=");
                        next();
                    }
                    else
                    {
                        push(TokenType::OP_DIV, "/");
                    }
                }
            }
            else if (match('%'))
            {
                if (test('%'))
                {
                    push(TokenType::OP_QUOTE, "%%");
                    next();
                }
                else
                {
                    if (test('='))
                    {

                        push(TokenType::OP_MOD_ASSIGN, "%=");
                        next();
                    }
                    else
                    {
                        push(TokenType::OP_MOD, "%");
                    }
                    next();
                }
            }
            else if (match('|'))
            {
                if (test('|'))
                {
                    push(TokenType::OP_LOGIC_OR, "||");
                    next();
                }
                else
                {
                    if (test('='))
                    {
                        push(TokenType::OP_BIT_OR_ASSIGN, "|=");
                        next();
                    }
                    else
                    {
                        push(TokenType::OP_BIT_OR, "|");
                    }
                }
            }
            else if (match('&'))
            {
                if (test('&'))
                {
                    push(TokenType::OP_LOGIC_AND, "&&");
                    next();
                }
                else
                {
                    if (test('='))
                    {
                        push(TokenType::OP_BIT_OR_ASSIGN, "&=");
                        next();
                    }
                    else
                    {
                        push(TokenType::OP_BIT_OR, "&");
                    }
                }
            }
            else if (match('<'))
            {
                if (test('<'))
                {
                    push(TokenType::OP_SHIFT_LEFT, "<<");
                    next();
                }
                else
                {
                    if (test('='))
                    {
                        push(TokenType::OP_LESS_EQUAL, "<=");
                        next();
                    }
                    else
                    {
                        push(TokenType::OP_LESS_THAN, "<");
                    }
                }
            }
            else if (match('>'))
            {
                if (test('>'))
                {
                    push(TokenType::OP_SHIFT_RIGHT, ">>");
                    next();
                }
                else
                {
                    if (test('='))
                    {
                        push(TokenType::OP_GREATER_EQUAL, ">=");
                        next();
                    }
                    else
                    {
                        push(TokenType::OP_GREATER_THAN, ">");
                    }
                }
            }
            else if (match('='))
            {
                bool pureAssignment = true;
                auto replace = [&](TokenType t1, TokenType t2, std::string s) {
                    if (sequence.size() != 0 && sequence.back().type == t1)
                    {
                        sequence.pop_back();
                        push(t2, s);
                        pureAssignment = false;
                    }
                };

                replace(TokenType::OP_POW, TokenType::OP_POW_ASSIGN, "**=");
                replace(TokenType::OP_QUOTE, TokenType::OP_QUOTE_ASSIGN, "%%=");
                replace(TokenType::OP_INTDIV, TokenType::OP_INTDIV_ASSIGN, "//=");
                replace(TokenType::OP_SHIFT_LEFT, TokenType::OP_SHIFT_LEFT_ASSIGN, "<<=");
                replace(TokenType::OP_SHIFT_RIGHT, TokenType::OP_SHIFT_RIGHT_ASSIGN, ">>=");

                if (pureAssignment)
                {
                    push(TokenType::OP_ASSIGN, "=");
                }

            }
            else if (match('('))
            {
                push(TokenType::OP_PAREN_BEGIN, "(");
            }
            else if (match(')'))
            {
                push(TokenType::OP_PAREN_END, ")");
            }
            else if (match('['))
            {
                push(TokenType::OP_BRACKET_BEGIN, "[");
            }
            else if (match(']'))
            {
                push(TokenType::OP_BRACKET_END, "]");
            }
            else if (match('{'))
            {
                push(TokenType::OP_BRACE_BEGIN, "{");
            }
            else if (match('}'))
            {
                push(TokenType::OP_BRACE_END, "}");
            }
            else if (match('.'))
            {
                if (sequence.size() != 0 && sequence.back().type == TokenType::OP_DOT_DOT)
                {
                    sequence.pop_back();
                    push(TokenType::OP_DOT_DOT_DOT, "...");
                }
                else if (test('.'))
                {
                    push(TokenType::OP_DOT_DOT, "..");
                    next();
                }
                else
                {
                    push(TokenType::OP_DOT, ".");
                }
            }
            else if (match(':'))
            {
                push(TokenType::OP_COLON, ":");
            }
            else if (match('?'))
            {
                push(TokenType::OP_QUESTION, "?");
            }
            else if (match(';'))
            {
                push(TokenType::OP_SEMICOLON, ";");
            }
            else
            {
                if (ch == WEOF)
                {
                    push(TokenType::END_OF_FILE, { ch });
                }
                else
                {
                    push(TokenType::UNKNOWN, { ch });
                }
            }
        }
        return sequence;
    }
    Lexer::Token Lexer::forwards(size_t n)
    {
        size_t offset = cursor + n;
        if (offset >= sequence.size())
        {
            return { TokenType::END_OF_FILE, { EOF } };
        }
        return sequence.at(offset);
    }
    Lexer::Token Lexer::backwards(size_t n)
    {
        if (n >= cursor)
        {
            // TODO...
        }
        return sequence.at(cursor - n);
    }
    Lexer::Token Lexer::next(size_t n)
    {
        cursor += n;
        return forwards(0);
    }
    Lexer::Token Lexer::last(size_t n)
    {
        cursor -= n;
        return backwards(0);
    }
    Lexer::Token Lexer::now()
    {
        return sequence.at(cursor);
    }
    bool Lexer::isEnd()
    {
        return cursor >= sequence.size();
    }
    std::unordered_map<std::string, Lexer::TokenType> Lexer::getKeywordMap()
    {
        return keywordMap;
    }
    std::unordered_set<Lexer::TokenType> Lexer::getOperatorSet()
    {
        return operatorSet;
    }
    void Lexer::error(std::string info)
    {
        throw LexError{ "SyntaxError: " + info, context.lineOffset, context.charOffset };
    }
}
}
