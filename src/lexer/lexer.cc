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
		auto next = [&]() {
			return context.getNextChar();
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
			auto test = [&](wchar_t s) {
				return context.lookNextChar() == s;
			};

			if (iswdigit(ch))
			{
				std::wstring s;
				while (iswdigit(ch))
				{
					s += ch;
					ch = next();
				}
				push(TokenType::NUMBER, s);
				//ch = context.getLastChar();
			}			
			else if (match('+'))
			{
				push(TokenType::OP_ADD, L"+");
			}
			else if (match('-'))
			{
				push(TokenType::OP_MINUS, L"-");
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
					push(TokenType::OP_MUL, L"*");
				}
			}
			else if (match('/'))
			{
				if (test('/'))
				{
					push(TokenType::OP_INTDIV, L"//");
					next();
				}
				else
				{
					push(TokenType::OP_DIV, L"/");
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
					push(TokenType::OP_MOD, L"%");
				}
			}
			else if (match('('))
			{
				push(TokenType::OP_PAREN_BEGIN, L"(");
			}
			else if (match(')'))
			{
				std::cout << "-------\n" << std::endl;
				push(TokenType::OP_PAREN_END, L")");
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
