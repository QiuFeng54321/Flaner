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
		std::shared_ptr<syntax::ListLiteral> parseListLiteral();
		std::shared_ptr<syntax::ObjectLiteral> parseObjectLiteral();
		std::shared_ptr<syntax::Expression> parsePrimary();
		std::shared_ptr<syntax::Expression> parseExpression(
			std::shared_ptr<syntax::Expression> rhs, Priority base);
		syntax::StatementSequence getProgram();
		std::vector<Token> shuntingYard();
		std::pair<std::vector<Token>, std::vector<Token>> convertExpression();

	public:
		std::shared_ptr<syntax::ExpressionStatement> parseExpressionStatement();

	public:
		bool isBaseLiteral(Token token);
		syntax::ListLiteral getListLiteral(Token token);
		syntax::ObjectLiteral getObjectLiteral(Token token);

		bool isKeyword(Token token);
		bool isIdentifier(Token token);

		bool isBeginOperator(Token token);
		bool isEndOperator(Token token);

		bool isOperator(Token token);
		bool isAssignment(Token token);
		bool isLeftAssociation(Token token);
		bool isRightAssociation(Token token);
		Priority getPriority(Token token, bool isNegation = false);
		bool isUnaryOperator(Token token);
		bool isBinaryOperator(Token token);

	public:
		struct SyntaxError
		{
			std::string info;
			size_t line, offset;
			SyntaxError(){}
			SyntaxError(std::string s)
			{
				info = "SyntaxError: " + s;
			}
		};

		struct UnexpectedToken_SyntaxError : SyntaxError
		{
			UnexpectedToken_SyntaxError(Token token)
			{
				info = "SyntaxError: Unexpected token `" + token.value + "`";
			}
		};

	private:
		lexer::Lexer lexer;
		syntax::StatementSequence program;
	};
}
}

#endif // !_FLANER_PARSER_PARSER_HH_
