cmake_minimum_required (VERSION 3.10.2)
project (PlaceholderName)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories (depend/include)
include_directories (include)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	link_directories(depend/lib/x64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	link_directories(depend/lib/x86)
endif()

add_executable (placeholder
        src/Config.cpp
        src/Engine.cpp
        src/logger.cpp
        src/main.cpp
)