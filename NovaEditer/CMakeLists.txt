

file(GLOB SB_SOURCES ${PROJECT_SOURCE_DIR}/NovaEditer/*.cpp)


include_directories(
    ${PROJECT_SOURCE_DIR}/NovaEditer)
add_executable(NovaEditer ${SB_SOURCES})

target_link_directories(NovaEditer PUBLIC ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(NovaEditer test)



