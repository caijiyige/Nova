
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/Nova/src/*.cpp)


include_directories(${HEADERS})
add_library(test SHARED ${SOURCES})

target_compile_definitions(test PRIVATE NV_EXPORT)



