cmake_minimum_required (VERSION 3.0)
project (flaner)

aux_source_directory (src SRC)

add_executable (flaner ${SRC})
set_property (TARGET flaner PROPERTY CXX_STANDARD 17)
target_include_directories (flaner PUBLIC include)
