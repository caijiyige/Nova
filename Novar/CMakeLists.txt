
file(GLOB CORE_SOURCES ${PROJECT_SOURCE_DIR}/Novar/Core/*.cpp)
file(GLOB PLAT_SOURCES ${PROJECT_SOURCE_DIR}/Novar/PlatForm/OpenGL/*.cpp ${PROJECT_SOURCE_DIR}/Novar/PlatForm/Windows/*.cpp)
file(GLOB IMGUI_SOURCES ${PROJECT_SOURCE_DIR}/Novar/Imgui/*.cpp ${PROJECT_SOURCE_DIR}/Novar/vendor/imgui/*.cpp)
file(GLOB GLM_SOURCES ${PROJECT_SOURCE_DIR}/Novar/vendor/glm/glm/*.cpp)
file(GLOB RENDERER_SOURCES ${PROJECT_SOURCE_DIR}/Novar/Renderer/*.cpp)





link_directories(${PROJECT_SOURCE_DIR}/bin)




add_library(test STATIC ${CORE_SOURCES} ${IMGUI_SOURCES}  ${GLM_SOURCES} ${RENDERER_SOURCES} ${PROJECT_SOURCE_DIR}/Novar/vendor/stb_image/stb_image.cpp ${PLAT_SOURCES} )
target_link_libraries(test PUBLIC glad libglfw3.a opengl32.lib)
target_precompile_headers(test PRIVATE Core/PCH.h)
target_compile_definitions(test PUBLIC NV_EXPORT)
target_include_directories(test PUBLIC ${PROJECT_SOURCE_DIR}/Novar/Core)
target_link_options(test PRIVATE
            -flto
        )





