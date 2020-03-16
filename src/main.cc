#include <parser/parser.hh>

int main(int argc, char* argv[])
{
    using namespace flaner::lexer;
	using namespace flaner::parser;

	Parser parser(R"(D:\hello.fln)");
	auto vec = parser.shuntingYard();


	std::cout << "\nFlaner Programming Language.\n--------\n\n";

	try
	{
		for (auto i : vec)
		{
			std::wcout << "[type: " << static_cast<int>(i.type) << ", value: " << i.value << "]\n";
		}
	}
	catch (const Lexer::LexError& e)
	{
		std::wcout << "Lexing error: " << e.info << "\nline " << e.line << ", offset " << e.offset << ".";
	}

    
    Lexer lexer(R"(D:\hello.fln)");


    std::cout << "\n\n";
    
    return 0;
}
