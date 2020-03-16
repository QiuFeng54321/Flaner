#ifndef _FLANER_PARSER_PARSER_HH_
#define _FLANER_PARSER_PARSER_HH_

#include <lexer/lexer.hh>
#include <parser/statement.hh>
#include <stack>

namespace flaner
{
namespace parser
{
	class Parser
	{
	public:
		Parser(std::string path)
			: lexer(path)
		{
			lexer.process();
		}
		~Parser() {}

	public:
		using Token = lexer::Lexer::Token;
		using Type = lexer::Lexer::TokenType;

	public:
		enum class Priority : uint16_t
		{
			comma,
			spread,
			yield,
			assignment,
			condition,
			logical_or,
			logical_and,
			bit_or,
			bit_xor,
			bit_and,
			equality,
			relationship,
			bit_shift,
			addtion,
			multition,
			power,
			unary_operator,
			visitor,
			group,
		};

	public:
		syntax::Expression parseExpression(Token firstToken, Priority priority);
		syntax::StatementSequence getProgram();
		std::vector<Token> shuntingYard();

	public:
		bool isBaseLiteral(Token token);
		syntax::ListLiteralNode getListLiteral(Token token);
		syntax::ObjectLiteralNode getObjectLiteral(Token token);
		bool isBeginOperator(Token token);
		bool isKeyword(Token token);
		bool isIdentifier(Token token);
		bool isOperator(Token token);
		bool isLeftAssociation(Token token);
		Priority getPriority(Token token, bool isNegation = false);
		bool isUnaryOperator(Token token);

	public:
		struct SyntaxError
		{
			std::wstring info;
			size_t line, offset;
			SyntaxError(){}
			SyntaxError(std::wstring s)
			{
				info = L"SyntaxError: " + s;
			}
		};

		struct UnexpectedToken_SyntaxError : SyntaxError
		{
			UnexpectedToken_SyntaxError(Token token)
			{
				info = L"SyntaxError: Unexpected token `" + token.value + L"`";
			}
		};

	private:
		lexer::Lexer lexer;
		syntax::StatementSequence program;
	};
}
}

#endif // !_FLANER_PARSER_PARSER_HH_
