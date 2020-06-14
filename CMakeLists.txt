cmake_minimum_required(VERSION 3.0.0)
project(GreedySnake VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(./include)
link_libraries(/usr/local/Cellar/ncurses/6.2/lib/libcurses.a)
aux_source_directory(./source SrcFiles)
add_executable(compile ${SrcFiles})
set(EXECUTABLE_OUTPUT_PATH ./bin)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)