

file(GLOB SB_SOURCES ${PROJECT_SOURCE_DIR}/SandBox/*.cpp)


add_executable(SandBox ${SB_SOURCES})

target_link_directories(SandBox PUBLIC ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(SandBox test)



