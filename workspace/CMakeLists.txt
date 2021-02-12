cmake_minimum_required(VERSION 3.0)

PROJECT(GuiLite)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

ADD_DEFINITIONS(-DGUILITE_ON)

# core
FILE(GLOB CORE_SRC      core/*.cpp)

IF (WIN32)  
    MESSAGE(STATUS "Now is windows")  
	FILE(GLOB CORE_ADAPTER  core/adapter/api_win.cpp)
ELSEIF (APPLE)  
    MESSAGE(STATUS "Now is Apple systens.")  
	FILE(GLOB CORE_ADAPTER  core/adapter/api_linux.cpp
                        core/adapter/audio_linux.cpp)
ELSEIF (UNIX)  
    MESSAGE(STATUS "Now is UNIX-like OS's. Including aPPLE os x  and CygWin")  
	FILE(GLOB CORE_ADAPTER  core/adapter/api_linux.cpp
                        core/adapter/audio_linux.cpp)
ELSE ()
	MESSAGE(STATUS "UnKonw systens.") 
	FILE(GLOB CORE_ADAPTER  core/adapter/api_unknown.cpp)
ENDIF () 


# gui
FILE(GLOB WIDGETS_SRC       widgets/*.cpp)

# build static library
ADD_LIBRARY(GuiLite STATIC ${CORE_SRC} ${CORE_ADAPTER} ${WIDGETS_SRC})

# build shared library
# ADD_LIBRARY(GuiLite SHARED ${CORE_SRC} ${CORE_ADAPTER} ${WIDGETS_SRC})

execute_process(COMMAND ${CMAKE_SOURCE_DIR}/.sync.sh GuiLite)
