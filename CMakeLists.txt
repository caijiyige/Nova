cmake_minimum_required(VERSION 4.0)

project(Nova VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_MP_FLAG "-j$ENV{NUM_PROCESSORS}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)#将所有生成的可执行文件与动态库放在bin

include_directories(
    ${PROJECT_SOURCE_DIR}/Novar/Core
    ${PROJECT_SOURCE_DIR}/Novar/Events
    ${PROJECT_SOURCE_DIR}/Novar/Imgui
    ${PROJECT_SOURCE_DIR}/Novar/Renderer
    
    
    ${PROJECT_SOURCE_DIR}/Novar/vendor/spdlog/include
    ${PROJECT_SOURCE_DIR}/Novar/vendor/glfw/include
    ${PROJECT_SOURCE_DIR}/Novar/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/Novar/vendor/imgui
    ${PROJECT_SOURCE_DIR}/Novar/vendor/glm/glm
    ${PROJECT_SOURCE_DIR}/Novar/vendor/stb_image

    
    ${PROJECT_SOURCE_DIR}/Novar/PlatForm/OpenGL 
    ${PROJECT_SOURCE_DIR}/Novar/PlatForm/Windows
)

link_directories(
    ${PROJECT_SOURCE_DIR}/Novar/bin
)

add_definitions(
    -DNV_PLATFORM_WINDOWS
)

#add_subdirectory(Novar/vendor/glad)
add_subdirectory(Novar)
add_subdirectory(SandBox)
