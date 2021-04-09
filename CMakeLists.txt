cmake_minimum_required(VERSION 3.16)
project(live)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package (Threads)
find_package(GLEW REQUIRED)

if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

set(SFML_LIBS sfml-graphics sfml-window sfml-system)
include_directories(. imgui ${OPENGL_INCLUDE_DIRS})
add_executable(live main.cpp
        imgui/imgui.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_draw.cpp
        imgui/imgui-SFML.cpp
        Executing.cpp
        include/Executing.h World.cpp include/World.h include/Basics.h Settings.cpp include/Settings.h Shader.cpp include/Shader.h include/WorldObjects.h Basics.cpp ImplementationExecutableFunctions.cpp include/ImplementationExecutableFunctions.h WorldObjects.cpp)


target_link_libraries(live  ${OPENGL_LIBRARY} ${SFML_LIBS} ${GLEW_LIBRARIES} Threads::Threads)