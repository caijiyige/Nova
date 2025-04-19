

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)#将所有生成的可执行文件与动态库放在bin

file(GLOB CORE_SOURCES ${PROJECT_SOURCE_DIR}/Novar/Core/*.cpp)
file(GLOB PLAT_SOURCES ${PROJECT_SOURCE_DIR}/Novar/PlatForm/OpenGL/*.cpp ${PROJECT_SOURCE_DIR}/Novar/PlatForm/Windows/*.cpp)
file(GLOB IMGUI_SOURCES ${PROJECT_SOURCE_DIR}/Novar/Imgui/*.cpp ${PROJECT_SOURCE_DIR}/Novar/vendor/imgui/*.cpp)
file(GLOB GLM_SOURCES ${PROJECT_SOURCE_DIR}/Novar/vendor/glm/glm/*.cpp)
file(GLOB RENDERER_SOURCES ${PROJECT_SOURCE_DIR}/Novar/Renderer/*.cpp)





link_directories(${PROJECT_SOURCE_DIR}/bin)

add_compile_options(-w)

add_library(test STATIC ${CORE_SOURCES} ${IMGUI_SOURCES}  ${GLM_SOURCES} ${RENDERER_SOURCES} ${PROJECT_SOURCE_DIR}/Novar/vendor/stb_image/stb_image.cpp ${PLAT_SOURCES} )
target_link_libraries(test PUBLIC libglad.a libglfw3.a opengl32.lib)
target_precompile_headers(test PUBLIC Core/PCH.h)
target_compile_definitions(test PUBLIC NV_EXPORT)




