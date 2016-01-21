project (SeedboxDaemon C CXX)

cmake_minimum_required (VERSION 3.1)

find_package (Threads)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_BUILD_TYPE Debug)

set (SeedboxDaemon_VERSION_MAJOR 0)
set (SeedboxDaemon_VERSION_MINOR 0)
set (SeedboxDaemon_VERSION_PATCH 0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

## Libraries
add_library(Utils utils.cxx)
add_library(Debug debug.cxx)
add_library(Socket socket.cxx)
add_library(Seedbox seedbox.cxx)
add_library(CLI cli.cxx)

set (EXTRA_LIBS ${EXTRA_LIBS} CLI Seedbox Socket Utils Debug)

add_executable (SeedboxDaemon main.cxx)
target_link_libraries (SeedboxDaemon ${EXTRA_LIBS} ${CMAKE_THREAD_LIBS_INIT})

install (TARGETS Utils DESTINATION bin)
install (TARGETS Debug DESTINATION bin)
install (TARGETS CLI DESTINATION bin)
install (TARGETS Socket DESTINATION bin)
install (TARGETS Seedbox DESTINATION bin)
install (TARGETS SeedboxDaemon DESTINATION bin)

install (FILES includes.h DESTINATION include)
install (FILES debug.h DESTINATION include)
install (FILES cli.h DESTINATION include)
install (FILES socket.h DESTINATION include)
install (FILES utils.h DESTINATION include)
install (FILES seedbox.h DESTINATION include)
