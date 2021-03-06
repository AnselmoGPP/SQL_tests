
CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

PROJECT(world_cities
	VERSION 1.0
	DESCRIPTION "Using SQLite with C++"
	LANGUAGES CXX
)

ADD_EXECUTABLE(${PROJECT_NAME}
	main.cpp

	auxiliar.hpp
	../../databases/worldcities.csv
)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
	.
	../../extern/sqlite-amalgamation-3340100
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} #PUBLIC
	${PROJECT_SOURCE_DIR}/../../_BUILD/extern/sqlite-amalgamation-3340100/libsqlite3.a
	-pthread -ldl
)


