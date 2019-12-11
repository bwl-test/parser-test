
file(GLOB SRCS *.cpp *.h)

add_executable(parsebase_test ${SRCS})

target_link_libraries(
    parsebase_test 
    PRIVATE Catch2 parserbase
)