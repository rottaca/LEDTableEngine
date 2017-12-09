cmake_minimum_required(VERSION 3.1)

project(LEDTable)
set (CMAKE_CXX_STANDARD 11)

add_executable(LEDTable
      main.cpp
      engine/baseController.cpp
      engine/baseInput.cpp
      engine/baseApplication.cpp
      engine/keyboardDebouncer.cpp
      engine/font.cpp

      controllers/desktopController.cpp
      controllers/matrixController.cpp
      controllers/TPM2.cpp


      inputHandlers/keyboard.cpp
      inputHandlers/keyboardInput.cpp

      applications/textMenu.cpp
      applications/testPatterns.cpp
      applications/snake.cpp
      applications/imageSlideShow.cpp
      applications/pong.cpp
      applications/fractal.cpp
      applications/shaders.cpp
      applications/gameOfLife.cpp

)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(LEDTable ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})

find_package (Threads)
target_link_libraries (LEDTable ${CMAKE_THREAD_LIBS_INIT})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
