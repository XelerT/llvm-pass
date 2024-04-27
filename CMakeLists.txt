cmake_minimum_required(VERSION 3.13.4)
project(stasyan)

# Build flags
include(cmake/build_flags.cmake)

# Build mode
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose Release or Debug" FORCE)
endif()

# collect srcs

add_subdirectory(src)
message(STATUS "Collected srcs: ${SRC_LIST}")

message("Building paracl in ${CMAKE_BUILD_TYPE}.")

message("CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}")
message("CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(LLVM_LIBS support core irreader passes)
if (NOT TARGET LLVMCore)
        set (LLVM_LIBS "LLVM")
endif ()

# srcs

foreach(SRC ${SRC_LIST})
        target_compile_options(${SRC} PUBLIC "$<$<CONFIG:Debug>:${DEBUG_BUILD}>")
        target_link_options(${SRC}    PUBLIC "$<$<CONFIG:Debug>:${DEBUG_LINK}>")

        target_compile_features(${SRC} PUBLIC cxx_std_20)
        target_include_directories(${SRC} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/${SRC})
        target_include_directories(${SRC} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

        install(TARGETS ${SRC} DESTINATION bin)
endforeach()

# Create main.cpp

add_executable(${PROJECT_NAME} main.cpp)

target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Debug>:${DEBUG_BUILD}>")
target_link_options(${PROJECT_NAME}    PUBLIC "$<$<CONFIG:Debug>:${DEBUG_LINK}>")

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_link_libraries(${PROJECT_NAME} ${SRC_LIST} ${LLVM_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS ${PROJECT_NAME})

# Tests

option(UNITESTS "Enable/disable tests" OFF)
if (UNITESTS)
        add_subdirectory(tests)
endif()
#

install()
