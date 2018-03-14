CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

SET(PROJECT LEDTableEngine)
PROJECT(${PROJECT})

SET(ENGINE_SRC
  src/core/baseController.cpp
  src/core/baseInput.cpp
  src/core/baseApplication.cpp
  src/core/keyboardDebouncer.cpp
  src/core/font.cpp
  src/core/color.cpp
  src/core/matrixScrollText.cpp
  src/core/audio.cpp
  src/core/rasterizer.cpp
 )

SET(CTRL_SRC
  src/controllers/desktopController.cpp
  src/controllers/matrixController.cpp
  src/controllers/TPM2.cpp
)

SET(INPUT_HANDLER_SRC
  src/inputHandlers/keyboard.cpp
  src/inputHandlers/keyboardInput.cpp
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
    src/inputHandlers/gameControllerInput.cpp
  )
ELSE()
  MESSAGE(STATUS "I2C-tools not found! I2C controller support disabeled.")
ENDIF()

ADD_LIBRARY(${PROJECT} STATIC
    ${ENGINE_SRC}
    ${CTRL_SRC}
    ${INPUT_HANDLER_SRC}
)

TARGET_INCLUDE_DIRECTORIES(${PROJECT} PUBLIC include)

IF(I2C_FOUND)
  TARGET_COMPILE_DEFINITIONS(${PROJECT} PUBLIC LED_HAVE_I2C_H_=1)
ENDIF()

TARGET_INCLUDE_DIRECTORIES(${PROJECT} PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})

FIND_PACKAGE (Threads)
TARGET_LINK_LIBRARIES (${PROJECT} ${CMAKE_THREAD_LIBS_INIT})

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res/
      DESTINATION ${CMAKE_BINARY_DIR}/res)
