CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

SET(PROJECT LEDTableApps)
PROJECT(${PROJECT})

SET(APP_SRC
  src/update.cpp
  src/textMenu.cpp
  src/textDisplay.cpp
  src/testPatterns.cpp
  src/snake.cpp
  src/imageSlideShow.cpp
  src/pong.cpp
  src/fractal.cpp
  src/shaders.cpp
  src/gameOfLife.cpp
  src/flappyBird.cpp
  src/tetris.cpp
  src/minesweeper.cpp
  src/clock.cpp
)

SET(CMAKE_CXX_STANDARD 11)

ADD_LIBRARY(${PROJECT} STATIC
    ${APP_SRC}
)

TARGET_INCLUDE_DIRECTORIES(${PROJECT} PUBLIC include)
TARGET_LINK_LIBRARIES(${PROJECT} LEDTableEngine)

FIND_PACKAGE (Threads)
TARGET_LINK_LIBRARIES (${PROJECT} ${CMAKE_THREAD_LIBS_INIT})


FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res/
      DESTINATION ${CMAKE_BINARY_DIR}/res)
