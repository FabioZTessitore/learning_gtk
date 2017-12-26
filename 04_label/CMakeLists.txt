CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(Label)

SET(CMAKE_C_FLAGS "-std=gnu99 -Wall -Wextra -rdynamic")

find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK REQUIRED gtk+-3.0)

INCLUDE_DIRECTORIES(${GTK_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTK_CFLAGS_OTHER})

add_executable(Label main.c)
target_link_libraries(Label ${GTK_LIBRARIES})
