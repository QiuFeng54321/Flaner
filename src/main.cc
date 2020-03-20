#include <parser/parser.hh>

int main(int argc, char* argv[])
{
    using namespace flaner::lexer;
    using namespace flaner::parser;

    Parser parser(R"(D:\hello.fln)");
    parser.parseExpressionStatement();

    auto program = parser.getProgram();


    std::cout << "\nFlaner Programming Language.\n--------\n\n";

    try
    {
        std::cout << "Abstract Syntax Tree:\n";
        //    << program.head()->getTree() << std::endl;
        auto stm = syntax::BaseLiteralNode(
            Lexer::Token{ Lexer::TokenType::NUMBER, "100" });
        std::cout << stm.getTree().dump(4);
    }
    catch (const Lexer::LexError& e)
    {
        std::cout << "Lexing error: " << e.info << "\nline " << e.line << ", offset " << e.offset << ".";
    }

    
    Lexer lexer(R"(D:\hello.fln)");


    std::cout << "\n\n";
    
    return 0;
}
