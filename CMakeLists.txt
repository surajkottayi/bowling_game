cmake_minimum_required(VERSION 3.16)
project("BowlingGame"
    VERSION 0.1
    LANGUAGES CXX
    DESCRIPTION
    "A s/w which calculates the score in a bowling game"
)
set(CMAKE_CXX_STANDARD 17)
file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME}
    ${SRC}
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
