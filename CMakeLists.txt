cmake_minimum_required(VERSION 3.1)

project(LEDTable)
set (CMAKE_CXX_STANDARD 11)

add_executable(LEDTable
      main.cpp
      LEDTableEngine/baseController.cpp
      LEDTableEngine/baseInput.cpp
      LEDTableEngine/baseApplication.cpp
      LEDTableEngine/keyboardDebouncer.cpp

      controllers/desktopController.cpp


      inputHandlers/keyboard.cpp
      inputHandlers/keyboardInput.cpp

      applications/testPatterns.cpp

)

find_package(GTK2 2.6 REQUIRED gtk)
if(GTK2_FOUND)
   include_directories(${GTK2_INCLUDE_DIRS})
   target_link_libraries(LEDTable ${GTK2_LIBRARIES})
endif()

find_package (Threads)
target_link_libraries (LEDTable ${CMAKE_THREAD_LIBS_INIT})
