#include <parser/parser.hh>

int main(int argc, char* argv[])
{
    using namespace flaner::lexer;
    using namespace flaner::parser;

    Lexer lexer("D:\\hello.fln");

    std::cout << "\nFlaner Programming Language.\n--------\n\n";

    try
    {
		auto tokens = lexer.getSequence();
		for (auto i : tokens)
		{
			std::cout << "[type: " << static_cast<int>(i.type) << ", value: " << i.value << "]\n";
		}		
    }
    catch (const Lexer::LexError& e)
    {
        std::cout << "Lexing error: " << e.info << "\nline " << e.line << ", offset " << e.offset << ".";
    }

    std::cout << "\n\n--------\n\n";

    Parser parser(lexer);
        
    std::cout << "\n--------\n\n";
    return 0;
}
