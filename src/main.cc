#include <lexer/lexer.hh>

int main(int argc, char* argv[])
{
    using namespace flaner::lexer;

    std::cout << "\nFlaner Programming Language.\n--------\n\n";
    
    Lexer lexer(R"(D:\hello.fln)");
    try
    {
		auto tokens = lexer.process();
		for (auto i :tokens)
		{
			std::wcout << "[type: " << static_cast<int>(i.type) << ", value: " << i.value << "]\n";
		}
    }
    catch (const Lexer::LexError& e)
    {
        std::wcout << "Lexing error: " << e.info << "\nline " << e.line << ", offset " << e.offset << ".";
    }

    std::cout << "\n\n";
    
    return 0;
}
