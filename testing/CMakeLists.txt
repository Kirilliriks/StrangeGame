include_directories(../dependency/glad/)
include_directories(../glfw/include)
include_directories(../glm/)
include_directories(../imgui/)

add_executable(Catch_tests_run2 Testing.cpp Testing.h docktest.h)

target_link_libraries(Catch_tests_run2 Game)