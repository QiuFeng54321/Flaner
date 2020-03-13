#ifndef _FLANER_LEXER_LEXER_HH_
#define _FLANER_LEXER_LEXER_HH_

#include <lexer/context.hh>
#include <vector>

namespace flaner
{
namespace lexer
{
	class Lexer
	{
	public:
		Lexer(std::string path)
			: context(path),
			sequence()
		{}

		~Lexer() {}

	private:
		Context context;

	public:
		enum class TokenType : uint16_t
		{
			UNKNOWN,
			END_OF_FILE,

			BOOLEAN,
			NUMBER,
			STRING,
			BIGINT,
			RATIONAL,

			IDENTITY,

			KEYWORD_IF,
			KEYWORD_ELSE,
			KEYWORD_SWITCH,
			KEYWORD_WHILE,
			KEYWORD_DO,
			KEYWORD_FOR,
			KEYWORD_BREAK,
			KEYWORD_CONTINUE,
			KEYWORD_RETURN,
			KEYWORD_THROW,

			KEYWORD_LET,
			KETWORD_CONST,
			KEYWORD_CLASS,
			KEYWORD_IMPORT,
			KEYWORD_EXPORT,
			KEYWORD_AS,

			OP_ADD,
			OP_MINUS,
			OP_MUL,
			OP_INTDIV,
			OP_DIV,
			OP_MOD,
			OP_QUOTE,
			OP_POW,

			OP_ADD_ASSIGN,
			OP_MINUS_ASSIGN,
			OP_MUL_ASSIGN,
			OP_INTDIV_ASSIGN,
			OP_DIV_ASSIGN,
			OP_MOD_ASSIGN,
			OP_QUOTE_ASSIGN,
			OP_POW_ASSIGN,

			OP_LOGIC_NEGATE,
			OP_LOGIC_OR,
			OP_LOGIC_AND,

			OP_BIT_NEGATE,
			OP_BIT_OR,
			OP_BIT_AND,
			OP_BIT_XOR,

			OP_BIT_NEGATE_ASSIGN,
			OP_BIT_OR_ASSIGN,
			OP_BIT_AND_ASSIGN,
			OP_BIT_XOR_ASSIGN,

			OP_LESS_THAN,
			OP_GREAT_THAN,
			OP_LESS_EQUAL,
			OP_GREAT_EQUAL,
			OP_EQUAL,
			OP_NOT_EUQAL,

			OP_ASSIGN,
			OP_COLON,
			OP_QUESTION,
			OP_COMMA,
			OP_DOT,
			OP_DOT_DOT,
			OP_DOT_DOT_DOT,

			OP_PAREN_BEGIN,
			OP_PAREN_END,
			OP_BRACKET_BEGIN,
			OP_BRACKET_END,
			OP_BRACE_BEGIN,
			OP_BRACE_END,
		};

		struct Token
		{
			TokenType type;
			std::wstring value;
			Token(TokenType a, std::wstring b) : type(a), value(b) {}
		};

	private:
		std::vector<Token> sequence;
		
	public:
		std::vector<Token> process();
	};
}
}

#endif // !_FLANER_LEXER_LEXER_HH_
