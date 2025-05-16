
file(GLOB CORE_SOURCES ${PROJECT_SOURCE_DIR}/Novar/Core/*.cpp)
file(GLOB PLAT_OPENGL_SOURCES ${PROJECT_SOURCE_DIR}/Novar/PlatForm/OpenGL/*.cpp )
file(GLOB PLAT_WINDOWS_SOURCES ${PROJECT_SOURCE_DIR}/Novar/PlatForm/Windows/*.cpp)
file(GLOB IMGUI_SOURCES ${PROJECT_SOURCE_DIR}/Novar/Imgui/*.cpp)
file(GLOB RENDERER_SOURCES ${PROJECT_SOURCE_DIR}/Novar/Renderer/*.cpp)
file(GLOB SCENE_SOURCES ${PROJECT_SOURCE_DIR}/Novar/Scene/*.cpp)
file(GLOB GUI_SOURCES ${PROJECT_SOURCE_DIR}/Novar/vendor/imgui/*.cpp)

file(GLOB PHYSICS_SOURCES ${PROJECT_SOURCE_DIR}/Novar/Physics/*.cpp)


add_subdirectory(${PROJECT_SOURCE_DIR}/Novar/vendor/yaml)
add_subdirectory(${PROJECT_SOURCE_DIR}/Novar/vendor/box2d)

link_directories(${PROJECT_SOURCE_DIR}/Novar/bin)

add_compile_options(-w)

add_library(test OBJECT ${CORE_SOURCES} ${IMGUI_SOURCES} ${GUI_SOURCES} ${RENDERER_SOURCES} ${PHYSICS_SOURCES}  ${SCENE_SOURCES} ${PROJECT_SOURCE_DIR}/Novar/vendor/stb_image/stb_image.cpp ${PLAT_OPENGL_SOURCES} ${PLAT_WINDOWS_SOURCES})
target_link_libraries(test PUBLIC glad yaml-cpp libglfw3.a opengl32.lib libbox2d.a)

target_precompile_headers(test PRIVATE PCH.h)
target_compile_definitions(test PUBLIC NV_EXPORT)
#target_compile_options(test PUBLIC -O3)
target_include_directories(test PUBLIC ${PROJECT_SOURCE_DIR}/Core)






