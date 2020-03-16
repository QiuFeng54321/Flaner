#ifndef _FLANER_LEXER_LEXER_HH_
#define _FLANER_LEXER_LEXER_HH_

#include <lexer/context.hh>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace flaner
{
namespace lexer
{
	class Lexer
	{
	public:
		Lexer(std::string path)
			: context(path),
			sequence(), cursor(0)
		{}

		~Lexer() {}

	private:
		Context context;

	public:
		enum class TokenType : uint16_t
		{
			UNKNOWN,
			END_OF_FILE,

			KEYWORD_NONE,
			KEYWORD_TRUE,
			KEYWORD_FALSE,
			NUMBER,
			STRING,
			BIGINT,
			RATIONAL,

			IDENTIFIER,

			KEYWORD_IF,
			KEYWORD_ELSE,
			KEYWORD_SWITCH,
			KEYWORD_CASE,
			KEYWORD_DEFAULT,
			KEYWORD_WHILE,
			KEYWORD_DO,
			KEYWORD_FOR,
			KEYWORD_IN,
			KEYWORD_OF,
			KEYWORD_BREAK,
			KEYWORD_CONTINUE,
			KEYWORD_RETURN,
			KEYWORD_THROW,
			KEYWORD_YIELD,

			KEYWORD_LET,
			KEYWORD_CONST,
			KEYWORD_CLASS,
			KEYWORD_IMPORT,
			KEYWORD_EXPORT,
			KEYWORD_FROM,
			KEYWORD_AS,

			FUNCTION_ARROW,

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

			OP_BIT_OR_ASSIGN,
			OP_BIT_AND_ASSIGN,
			OP_BIT_XOR_ASSIGN,

			OP_SHIFT_LEFT,
			OP_SHIFT_RIGHT,
			OP_SHIFT_LEFT_ASSIGN,
			OP_SHIFT_RIGHT_ASSIGN,

			OP_LESS_THAN,
			OP_GREATER_THAN,
			OP_LESS_EQUAL,
			OP_GREATER_EQUAL,
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

			OP_SEMICOLON,
		};

		struct Token
		{
			TokenType type;
			std::wstring value;
			Token(TokenType a, std::wstring b) : type(a), value(b) {}
		};

	private:
		std::vector<Token> sequence;
		size_t cursor;
#define MAP(s, v) { L##s, TokenType::KEYWORD_##v },
		std::unordered_map<std::wstring, TokenType> keywordMap
		{
			MAP("none", NONE)
			MAP("true", TRUE)
			MAP("false", FALSE)
			MAP("if", IF)
			MAP("else", ELSE)
			MAP("switch", SWITCH)
			MAP("case", CASE)
			MAP("default", DEFAULT)
			MAP("while", WHILE)
			MAP("do", DO)
			MAP("for", FOR)
			MAP("in", IN)
			MAP("of", OF)
			MAP("break", BREAK)
			MAP("continue", CONTINUE)
			MAP("throw", THROW)
			MAP("return", RETURN)
			MAP("const", CONST)
			MAP("let", LET)
			MAP("import", IMPORT)
			MAP("export", EXPORT)
			MAP("as", AS)
			MAP("from", FROM)
		};
#undef MAP

		std::unordered_set<TokenType> operatorSet
		{
			TokenType::KEYWORD_IN,
			TokenType::KEYWORD_OF,

			TokenType::OP_ADD,
			TokenType::OP_MINUS,
			TokenType::OP_MUL,
			TokenType::OP_INTDIV,
			TokenType::OP_DIV,
			TokenType::OP_MOD,
			TokenType::OP_QUOTE,
			TokenType::OP_POW,

			TokenType::OP_ADD_ASSIGN,
			TokenType::OP_MINUS_ASSIGN,
			TokenType::OP_MUL_ASSIGN,
			TokenType::OP_INTDIV_ASSIGN,
			TokenType::OP_DIV_ASSIGN,
			TokenType::OP_MOD_ASSIGN,
			TokenType::OP_QUOTE_ASSIGN,
			TokenType::OP_POW_ASSIGN,

			TokenType::OP_LOGIC_NEGATE,
			TokenType::OP_LOGIC_OR,
			TokenType::OP_LOGIC_AND,

			TokenType::OP_BIT_NEGATE,
			TokenType::OP_BIT_OR,
			TokenType::OP_BIT_AND,
			TokenType::OP_BIT_XOR,

			TokenType::OP_BIT_OR_ASSIGN,
			TokenType::OP_BIT_AND_ASSIGN,
			TokenType::OP_BIT_XOR_ASSIGN,

			TokenType::OP_SHIFT_LEFT,
			TokenType::OP_SHIFT_RIGHT,
			TokenType::OP_SHIFT_LEFT_ASSIGN,
			TokenType::OP_SHIFT_RIGHT_ASSIGN,

			TokenType::OP_LESS_THAN,
			TokenType::OP_GREATER_THAN,
			TokenType::OP_LESS_EQUAL,
			TokenType::OP_GREATER_EQUAL,
			TokenType::OP_EQUAL,
			TokenType::OP_NOT_EUQAL,

			TokenType::OP_ASSIGN,
			TokenType::OP_COLON,
			TokenType::OP_QUESTION,
			TokenType::OP_COMMA,
			TokenType::OP_DOT,
			TokenType::OP_DOT_DOT,
			TokenType::OP_DOT_DOT_DOT,

			TokenType::OP_PAREN_BEGIN,
			TokenType::OP_PAREN_END,
			TokenType::OP_BRACKET_BEGIN,
			TokenType::OP_BRACKET_END,
			TokenType::OP_BRACE_BEGIN,
			TokenType::OP_BRACE_END,
		};

	public:
		bool isBlank(wchar_t ch);
		TokenType getKeywordOrID(std::wstring s);
		
	public:
		std::vector<Token> process();
		Token forwards(size_t n = 1);
		Token backwards(size_t n = 1);
		Token next(size_t n = 1);
		Token last(size_t n = 1);
		Token now();
		bool isEnd();
		std::unordered_map<std::wstring, TokenType> getKeywordMap();
		std::unordered_set<TokenType> getOperatorSet();

		struct LexError
		{
			std::wstring info;
			size_t line, offset;
			LexError(std::wstring s, size_t a, size_t b)
				: info(L"LexError: " + s),
				line(a), offset(b)
			{
			}
		};
		void error(std::wstring info);
	};
}
}

#endif // !_FLANER_LEXER_LEXER_HH_
