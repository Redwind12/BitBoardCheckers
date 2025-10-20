cmake_minimum_required(VERSION 3.26)
cmake_minimum_required(VERSION 3.26)
project(BitBoardCheckers C)

set(CMAKE_C_STANDARD 11)

add_executable(BitBoardCheckers
        main.c
        bitops.c
        checkers.c
)


