# https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake
# Installing sqlite3 library in Linux (in this case, this CMake wouldn't be needed): apt-get install sqlite3
# Generating the sqlite3 C library for your project yourself:

CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

set(LIB_VERSION 3.34.1)

PROJECT(sqlite3
	VERSION ${LIB_VERSION}
	DESCRIPTION "Compiled SQLite C library"
	LANGUAGES C
)

ADD_LIBRARY(${PROJECT_NAME} STATIC			# STATIC SHARED MODULE 
	shell.c
	sqlite3.c

	sqlite3.h
	sqlite3ext.h
)

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES 
	VERSION ${LIB_VERSION}
)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC .)	# PUBLIC PRIVATE INTERFACE
