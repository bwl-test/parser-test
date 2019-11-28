
file(GLOB SRCS *.cpp *.h)

add_executable(lexer_test ${SRCS})

target_link_libraries(
    lexer_test 
    PUBLIC Catch2 lexertl
)