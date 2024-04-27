set(SRCS
        dce.cpp
        mem2reg.cpp
        zero_prop.cpp
)

add_library(passes ${SRCS})
target_include_directories(passes PRIVATE ${CMAKE_BINARY_DIR})