cmake_minimum_required(VERSION 3.17)
project(finallexer C)

set(CMAKE_C_STANDARD 11)

add_executable(finallexer lextest.c lexframe.c lex.h code.h parser.c parser.h)