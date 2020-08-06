cmake_minimum_required (VERSION 3.0)
project (flaner)

aux_source_directory (src MAIN)
aux_source_directory (src/lexer LEXER)
set (PARSER src/parser/ast.cc)
aux_source_directory (src/runtimes RUNTIME)

add_executable (flaner ${MAIN} ${LEXER} ${PARSER} ${RUNTIME})
set_property (TARGET flaner PROPERTY CXX_STANDARD 17)
target_include_directories (flaner PUBLIC src)
