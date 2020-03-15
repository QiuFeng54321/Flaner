#ifndef _FLANER_PARSER_PARSER_HH_
#define _FLANER_PARSER_PARSER_HH_

#include <lexer/lexer.hh>
#include <parser/statement.hh>

namespace flaner
{
namespace parser
{
	class Parser
	{
	public:
		Parser(std::string path)
			: tokenOffset(0)
		{
			lexer::Lexer lexer(path);
			tokenStream = lexer.process();
		}
		~Parser() {}

	public:
		using Token = lexer::Lexer::Token;
		using Type = lexer::Lexer::TokenType;

	public:
		Token getNextToken();

	public:
		void parseExpression();
		syntax::StatementSequence getProgram();

	public:
		bool isLiteral(Token token);
		bool getLiteral(Token token);
		bool isKeyword(Token token);
		bool isIdentifier(Token token);
		bool isOperator(Token token);

	public:
		struct SyntaxError
		{
			std::wstring info;
			size_t line, offset;
			SyntaxError(std::wstring s)
			{
				info = L"SyntaxError: " + s;
			}
		};

	private:
		std::vector<Token> tokenStream;
		size_t tokenOffset;
		syntax::StatementSequence program;
	};
}
}

#endif // !_FLANER_PARSER_PARSER_HH_
