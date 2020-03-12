#include <lexer/lexer.hh>

int main(int argc, char* argv[])
{
	using namespace flaner::lexer;
	setlocale(LC_ALL, "chs");
	std::cout << "\nFlaner Programming Language.\n--------\n\n";
	
	Lexer lexer(R"(D:\hello.fln)");
	auto tokens = lexer.process();

	for (auto i : tokens)
	{
		std::wcout << "[type: " << static_cast<int>(i.type) << ", value: " << i.value << "]\n";
	}

	std::cout << "\n\n";
	
	return 0;
}
