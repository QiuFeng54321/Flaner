#include <lexer/lexer.hh>
#include <lexer/utility.hh>

namespace flaner
{
namespace lexer
{
    std::vector<Lexer::Token> Lexer::process()
    {
        auto push = [&](TokenType t, std::wstring v) {
            sequence.push_back({ t, v });
        };
        auto next = [&](size_t offset = 1) {
            return context.getNextChar(offset);
        };
        auto lastToken = [&]() {
            return sequence.at(sequence.size() - 1);
        };

        while (!context.isEnd())
        {
            wchar_t ch = next();

            if (util::isBlank(ch))
            {
                continue;
            }

            auto match = [&](wchar_t s) {
                return ch == s;
            };
            auto test = [&](wchar_t s, size_t offset = 1) {
                return context.lookNextChar(offset) == s;
            };

            if (iswdigit(ch))
            {
                std::wstring s{ ch };
                while (iswdigit(ch) && iswdigit(context.lookNextChar(1)))
                {
                    s += ch;
                    ch = next();
                }
                push(TokenType::NUMBER, s);
            }            
            else if (match('+'))
            {
                if (test('='))
                {
                    push(TokenType::OP_ADD_ASSIGN, L"+=");
                    next();
                }
                else
                {
                    push(TokenType::OP_ADD, L"+");
                }
            }
            else if (match('-'))
            {
                if (test('='))
                {
                    push(TokenType::OP_MINUS_ASSIGN, L"-=");
                    next();
                }
                else
                {
                    push(TokenType::OP_MINUS, L"-");
                }
            }
            else if (match('*'))
            {
                if (test('*'))
                {
                    push(TokenType::OP_POW, L"**");
                    next();
                }
                else
                {
                    if (test('='))
                    {

                        push(TokenType::OP_MUL_ASSIGN, L"*=");
                        next();
                    }
                    else
                    {
                        push(TokenType::OP_MUL, L"*");
                    }
                    next();
                }
            }
            else if (match('/'))
            {
                if (test('/'))
                {
                    push(TokenType::OP_INTDIV_ASSIGN, L"//");
                    next();
                }
                else
                {
                    if (test('='))
                    {
                        push(TokenType::OP_DIV_ASSIGN, L"/=");
                        next();
                    }
                    else
                    {
                        push(TokenType::OP_DIV, L"/");
                    }
                }
            }
            else if (match('%'))
            {
                if (test('%'))
                {
					push(TokenType::OP_QUOTE, L"%%");
                    next();
                }
                else
                {
                    if (test('='))
                    {

                        push(TokenType::OP_MOD_ASSIGN, L"%=");
                        next();
                    }
                    else
                    {
                        push(TokenType::OP_MOD, L"%");
                    }
                    next();
                }
            }
            else if (match('|'))
            {
                if (test('|'))
                {
                    push(TokenType::OP_LOGIC_OR, L"||");
                    next();
                }
                else
                {
                    if (test('='))
                    {
                        push(TokenType::OP_BIT_OR_ASSIGN, L"|=");
                        next();
                    }
                    else
                    {
                        push(TokenType::OP_BIT_OR, L"|");
                    }
                }
            }
            else if (match('&'))
            {
                if (test('&'))
                {
                    push(TokenType::OP_LOGIC_AND, L"&&");
                    next();
                }
                else
                {
                    if (test('='))
                    {
                        push(TokenType::OP_BIT_OR_ASSIGN, L"&=");
                        next();
                    }
                    else
                    {
                        push(TokenType::OP_BIT_OR, L"&");
                    }
                }
            }
			else if (match('='))
			{
				auto replace = [&](TokenType t1, TokenType t2, std::wstring s) {
					if (sequence.back().type == t1)
					{
						sequence.pop_back();
						push(t2, s);
					}
				};

				replace(TokenType::OP_POW, TokenType::OP_POW_ASSIGN, L"**=");
				replace(TokenType::OP_QUOTE, TokenType::OP_QUOTE_ASSIGN, L"%%=");
				replace(TokenType::OP_INTDIV, TokenType::OP_INTDIV_ASSIGN, L"//=");

			}
            else if (match('('))
            {
                push(TokenType::OP_PAREN_BEGIN, L"(");
            }
            else if (match(')'))
            {
                push(TokenType::OP_PAREN_END, L")");
            }
            else if (match('['))
            {
                push(TokenType::OP_BRACKET_BEGIN, L"[");
            }
            else if (match(']'))
            {
                push(TokenType::OP_BRACKET_END, L"]");
            }
            else if (match('{'))
            {
                push(TokenType::OP_BRACE_BEGIN, L"{");
            }
            else if (match('}'))
            {
                push(TokenType::OP_BRACE_END, L"}");
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
    void Lexer::error(std::wstring info)
    {
        throw LexError{ L"SyntaxError: " + info, context.lineOffset, context.charOffset };
    }
}
}
