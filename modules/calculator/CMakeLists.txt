file(GLOB_RECURSE LIBSRCS include/calculator/*.h libsrc/*.cpp)
add_library(calculib ${LIBSRCS})
target_include_directories(
    calculib
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(
    calculib
    PUBLIC lexertl fmt parserbase
)

set(EXESRCS main.cpp)
add_executable(calculator ${EXESRCS})
target_link_libraries(
    calculator 
    PRIVATE calculib
)