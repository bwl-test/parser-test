file(GLOB_RECURSE SRCS *.cpp *.h *.hpp)

add_library(parserbase ${SRCS})

target_include_directories(
    parserbase
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)