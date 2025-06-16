cmake_minimum_required(VERSION 3.10)
project(FancyTicTacToe VERSION 0.1.0 LANGUAGES C CXX)

set(SFML_DIR "C:/Libraries/SFML-3.0.0/lib/cmake/SFML" CACHE STRING "SFML directory")

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/FancyTicTacToe_Background.c
    COMMAND C:/msys64/usr/bin/xxd.exe -i -n FancyTicTacToe_Background ${CMAKE_CURRENT_SOURCE_DIR}/images/FancyTicTacToe_Background.png > ${CMAKE_CURRENT_SOURCE_DIR}/FancyTicTacToe_Background.c
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/images/FancyTicTacToe_Background.png
    COMMENT "Embedding FancyTicTacToe_Background.png as C source"
)
add_custom_target(embed_bg ALL DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/FancyTicTacToe_Background.c)

add_executable(FancyTicTacToe FancyTicTacToe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FancyTicTacToe_Background.c
)
add_dependencies(FancyTicTacToe embed_bg)

target_include_directories(FancyTicTacToe PRIVATE "C:/Libraries/SFML-3.0.0/include")
target_include_directories(FancyTicTacToe PRIVATE ${CMAKE_BINARY_DIR})

target_link_libraries(FancyTicTacToe
    SFML::Graphics
    SFML::Window
    SFML::System
)

set_property(TARGET FancyTicTacToe PROPERTY CXX_STANDARD 17)