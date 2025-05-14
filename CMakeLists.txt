cmake_minimum_required(VERSION 4.0)

project(Nova VERSION 0.1.0 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
 message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)#将所有生成的可执行文件与动态库放在bin
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
include_directories(

    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/Novar/vendor/spdlog/include
    ${PROJECT_SOURCE_DIR}/Novar/vendor/glfw/include
    ${PROJECT_SOURCE_DIR}/Novar/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/Novar/vendor/imgui
    ${PROJECT_SOURCE_DIR}/Novar/vendor/glm
    ${PROJECT_SOURCE_DIR}/Novar/vendor/stb_image
    ${PROJECT_SOURCE_DIR}/Novar/vendor/entt
    ${PROJECT_SOURCE_DIR}/Novar/vendor/yaml/include
    ${PROJECT_SOURCE_DIR}/Novar/vendor/box2d/include
)

link_directories(
    ${PROJECT_SOURCE_DIR}/bin
)



#add_subdirectory(vendor/glad)
add_subdirectory(Novar)
option(BUILD_NovaEditor "Build NovaEditor" ON)
option(BUILD_SandBox "Build SandBox" OFF)
if (BUILD_NovaEditor)
    add_subdirectory(NovaEditor)
    set_target_properties(NovaEditor PROPERTIES FOLDER "NovaEditor")
endif()
if(BUILD_SandBox)
    add_subdirectory(SandBox)
    set_target_properties(SandBox PROPERTIES FOLDER "SandBox")
endif()
