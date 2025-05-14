

file(GLOB SB_SOURCES ${PROJECT_SOURCE_DIR}/NovaEditor/*.cpp)


include_directories(
    ${PROJECT_SOURCE_DIR}/NovaEditor)
add_executable(NovaEditor ${SB_SOURCES})

target_link_directories(NovaEditor PUBLIC ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(NovaEditor test)



