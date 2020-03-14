#include <lexer/lexer.hh>

namespace flaner
{
namespace lexer
{
	bool Lexer::isBlank(wchar_t ch)
	{
		std::wstring blanks = L"\n\r\t\f \x0b\xa0\u2000"
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

	Lexer::TokenType Lexer::getKeywordOrIdentity(std::wstring s)
	{
		TokenType type;

		try
		{
			type = map.at(s);
		}
		catch (const std::exception&)
		{
			type = TokenType::IDENTITY;
		}
		return type;
	}

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

            if (isBlank(ch))
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
				wchar_t nextChar = context.lookNextChar(1);
                while (iswdigit(nextChar))
                {
					ch = next();
					nextChar = context.lookNextChar(1);
                    s += ch;
                }
                push(TokenType::NUMBER, s);
            }      
			else if (iswalpha(ch) || ch == L'_' || ch == L'$')
			{
				std::wstring word{ ch };
				wchar_t nextChar = context.lookNextChar(1);
				while (iswalnum(nextChar) || ch == L'_' || ch == L'$')
				{
					ch = next();
					nextChar = context.lookNextChar(1);
					word += ch;
				}
				if (sequence.size() != 0 && sequence.back().type == TokenType::OP_DOT)
				{
					push(TokenType::IDENTITY, word);
				}
				else
				{
					push(getKeywordOrIdentity(word), word);
				}
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
			else if (match('<'))
			{
				if (test('<'))
				{
					push(TokenType::OP_SHIFT_LEFT, L"<<");
					next();
				}
				else
				{
					if (test('='))
					{
						push(TokenType::OP_LESS_EQUAL, L"<=");
						next();
					}
					else
					{
						push(TokenType::OP_LESS_THAN, L"<");
					}
				}
			}
			else if (match('>'))
			{
				if (test('>'))
				{
					push(TokenType::OP_SHIFT_RIGHT, L">>");
					next();
				}
				else
				{
					if (test('='))
					{
						push(TokenType::OP_GREATER_EQUAL, L">=");
						next();
					}
					else
					{
						push(TokenType::OP_GREATER_THAN, L">");
					}
				}
			}
			else if (match('='))
			{
				bool pureAssignment = true;
				auto replace = [&](TokenType t1, TokenType t2, std::wstring s) {
					if (sequence.back().type == t1)
					{
						sequence.pop_back();
						push(t2, s);
						pureAssignment = false;
					}
				};

				replace(TokenType::OP_POW, TokenType::OP_POW_ASSIGN, L"**=");
				replace(TokenType::OP_QUOTE, TokenType::OP_QUOTE_ASSIGN, L"%%=");
				replace(TokenType::OP_INTDIV, TokenType::OP_INTDIV_ASSIGN, L"//=");
				replace(TokenType::OP_SHIFT_LEFT, TokenType::OP_SHIFT_LEFT_ASSIGN, L"<<=");
				replace(TokenType::OP_SHIFT_RIGHT, TokenType::OP_SHIFT_RIGHT_ASSIGN, L">>=");

				if (pureAssignment)
				{
					push(TokenType::OP_ASSIGN, L"=");
				}

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
			else if (match('.'))
			{
				if (sequence.back().type == TokenType::OP_DOT_DOT)
				{
					push(TokenType::OP_DOT_DOT_DOT, L"...");
				}
				else if (test('.'))
				{
					push(TokenType::OP_DOT_DOT, L"..");
					next();
				}
				else
				{
					push(TokenType::OP_DOT, L".");
				}
			}
			else if (match(':'))
			{
				push(TokenType::OP_COLON, L":");
			}
			else if (match('?'))
			{
				push(TokenType::OP_QUESTION, L"?");
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
