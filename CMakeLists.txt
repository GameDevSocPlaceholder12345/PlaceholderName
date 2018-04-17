cmake_minimum_required (VERSION 3.10.2)
project (PlaceholderName)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

include_directories (${CMAKE_SOURCE_DIR}/depend/include)
include_directories (${CMAKE_SOURCE_DIR}/include)

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

target_link_libraries(placeholder
	lua53
	SDL2
	SDL2main
	SDL2_ttf
	SDL2_image
	SDL2_mixer
	SDL2_ttf
)