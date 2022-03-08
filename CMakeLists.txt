cmake_minimum_required(VERSION 3.21)
project(StrangeGame)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -stdlib=libc++ -lc++abi")

add_subdirectory(glfw)
add_subdirectory(game)

include_directories(dependency/glad/)
include_directories(glfw/include)
include_directories(glm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra -std=c++14")

set(SOURCE_FILES main.cpp dependency/glad/glad.c)
add_executable(StrangeGame ${SOURCE_FILES})

target_link_libraries(StrangeGame glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})
target_link_libraries(StrangeGame Game)

set(IMGUI_PATH  "imgui/")
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp ${IMGUI_PATH}/*.h)
add_library("ImGui" STATIC ${IMGUI_SOURCES})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})
target_link_libraries(StrangeGame ImGui)

