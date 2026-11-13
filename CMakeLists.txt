cmake_minimum_required(VERSION 3.21)
project(StrangeGame)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(game)
add_subdirectory(testing)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra -std=c++20")
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static -lpthread")

target_link_libraries(StrangeGame Game)

find_package(glad CONFIG REQUIRED)
target_link_libraries(Game glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Game glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(Game glm::glm)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(Game imgui::imgui)

include(FetchContent)
FetchContent_Declare(
        tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
        GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)
target_link_libraries(Game tomlplusplus::tomlplusplus)