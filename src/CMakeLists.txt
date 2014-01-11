add_definitions(-W -Wall -ansi -pedantic -Wextra -g -O2 -std=c89)

aux_source_directory(. sources)

add_executable(avl_test ${sources})
