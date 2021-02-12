cmake_minimum_required(VERSION 3.0)

PROJECT(GuiLite)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

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

#set_target_properties(add_lib_shared PROPERTIES OUTPUT_NAME "add")
#set_target_properties(add_lib_static PROPERTIES OUTPUT_NAME "add")

#set_target_properties(add_lib_shared PROPERTIES VERSION 1.0 SOVERSION 1)

#INSTALL(TARGETS add_lib_shared add_lib_static
#    LIBRARY DESTINATION lib
#    ARCHIVE DESTINATION lib)

#INSTALL(FILES ${h_files}  DESTINATION include)