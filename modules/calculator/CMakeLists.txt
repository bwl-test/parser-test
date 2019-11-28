
file(GLOB SRCS *.cpp *.h)

add_executable(caculator ${SRCS})

target_link_libraries(
    caculator 
    PRIVATE lexertl
)