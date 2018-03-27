CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

ADD_SUBDIRECTORY(LEDTableEngine)
ADD_SUBDIRECTORY(LEDTableApps)

SET(PROJECT LEDTableMain)
PROJECT(${PROJECT})

SET(CMAKE_CXX_STANDARD 11)

ADD_EXECUTABLE(${PROJECT} main.cpp)

TARGET_LINK_LIBRARIES(${PROJECT} LEDTableEngine LEDTableApps)

# Require dot, treat the other components as optional
FIND_PACKAGE(Doxygen)
IF(DOXYGEN_FOUND)
add_custom_target(doc doxygen Doxyfile.txt
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
  COMMENT "Generating doxygen documentation"
)
endif()
