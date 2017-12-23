CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

SET(PROJECT LEDTable)
PROJECT(${PROJECT})

SET(ENGINE_SRC
  engine/baseController.cpp
  engine/baseInput.cpp
  engine/baseApplication.cpp
  engine/keyboardDebouncer.cpp
  engine/font.cpp
  engine/matrixScrollText.cpp
  engine/audio.c
 )

 SET(CTRL_SRC
   controllers/desktopController.cpp
   controllers/matrixController.cpp
   controllers/TPM2.cpp
 )

SET(INPUT_HANDLER_SRC
  inputHandlers/keyboard.cpp
  inputHandlers/keyboardInput.cpp
)
 SET(APP_SRC
  applications/update.cpp
  applications/textMenu.cpp
  applications/textDisplay.cpp
  applications/testPatterns.cpp
  applications/snake.cpp
  applications/imageSlideShow.cpp
  applications/pong.cpp
  applications/fractal.cpp
  applications/shaders.cpp
  applications/gameOfLife.cpp
  applications/flappyBird.cpp
  applications/tetris.cpp
 )

SET(CMAKE_CXX_STANDARD 11)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

INCLUDE (CheckIncludeFiles)
CHECK_INCLUDE_FILES("linux/i2c-dev.h" I2C_FOUND)
IF(I2C_FOUND)
  MESSAGE(STATUS "I2C-tools found. I2C controller support enabeled.")
  LIST(APPEND INPUT_HANDLER_SRC
    inputHandlers/gameControllerInput.cpp
  )
ELSE()
  MESSAGE(STATUS "I2C-tools not found! I2C controller support disabeled.")
ENDIF()

ADD_EXECUTABLE(${PROJECT}
    main.cpp
    ${ENGINE_SRC}
    ${CTRL_SRC}
    ${INPUT_HANDLER_SRC}
    ${APP_SRC}
)

IF(I2C_FOUND)
  TARGET_COMPILE_DEFINITIONS(${PROJECT} PRIVATE HAVE_I2C_H_=1)
ENDIF()

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})

FIND_PACKAGE (Threads)
TARGET_LINK_LIBRARIES (${PROJECT} ${CMAKE_THREAD_LIBS_INIT})

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
