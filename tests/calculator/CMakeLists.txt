
file(GLOB SRCS *.cpp *.h)

add_executable(calculator_test ${SRCS})

target_link_libraries(
    calculator_test 
    PUBLIC Catch2 calculib
)