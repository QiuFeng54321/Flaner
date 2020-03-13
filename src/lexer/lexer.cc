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

		while (!context.isEnd())
		{
			wchar_t ch = next();

			while (util::isBlank(ch))
			{
				ch = next();
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
					if (test('=', 2))
					{
						push(TokenType::OP_POW_ASSIGN, L"**=");
						next();
					}
					else
					{
						push(TokenType::OP_POW, L"**");
					}
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
				}
			}
			else if (match('/'))
			{
				if (test('/'))
				{
					if (test('=', 2))
					{
						push(TokenType::OP_INTDIV_ASSIGN, L"//=");
						next();
					}
					else
					{
						push(TokenType::OP_INTDIV_ASSIGN, L"//");
					}
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
					if (test('='))
					{

						push(TokenType::OP_QUOTE_ASSIGN, L"%%=");
						next();
					}
					else
					{
						push(TokenType::OP_QUOTE, L"%%");
					}
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
				if (test('='))
				{

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
}
}
