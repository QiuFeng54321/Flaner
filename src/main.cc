#include <lexer/lexer.hh>

int main(int argc, char* argv[])
{
	using namespace flaner::lexer;
	setlocale(LC_ALL, "chs");
	std::cout << "\nFlaner Programming Language.\n--------\n\n";
	Context context(R"(D:\hello.fln)");
	while (!context.isEnd())
	{
		std::wstring ch{ context.getNextChar() };
		std::wcout << ch;
	}

	std::cout << "\n\n";
	
	return 0;
}
