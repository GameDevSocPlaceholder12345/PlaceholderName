cmake_minimum_required (VERSION 3.10.2)
project (PlaceholderName)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories ($(MY_SOURCE_DIR)/depend/include)
include_directories ($(MY_SOURCE_DIR)/include)

add_executable (placeholder
        src/Config.cpp
        src/Engine.cpp
        src/logger.cpp
        src/main.cpp
)