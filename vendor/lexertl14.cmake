add_library(lexertl INTERFACE)

target_include_directories(
    lexertl
    INTERFACE ${CMAKE_CURRENT_LIST_DIR}/lexertl14/include
)