cmake_minimum_required(VERSION 4.0)
project(glad)
add_library(glad SHARED src/glad.c)
target_include_directories(glad PUBLIC include/)
