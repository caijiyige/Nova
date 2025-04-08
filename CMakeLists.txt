cmake_minimum_required(VERSION 4.0)

project(Nova VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_definitions("-Wall -g")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)#将所有生成的可执行文件与动态库放在bin

include_directories(${PROJECT_SOURCE_DIR}/Nova/include)

add_definitions(
    -DNV_PLATFORM_WINDOWS
)

add_subdirectory(Nova)
add_subdirectory(SandBox)
