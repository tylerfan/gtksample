
# Thanks to @danger89 and @Ilothar for updating the gist.
# Set the name and the supported language of the project
project(hello-world C CXX)
# include(FindGTK)

# Set the minimum version of cmake required to build this project
cmake_minimum_required(VERSION 3.10)

# Use the package PkgConfig to detect GTK+ headers/library files
# GTK 3 dependency
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
# FindGTK3()

# # Setup CMake to use GTK+, tell the compiler where to look for headers
# # and to the linker where to look for libraries
# INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
# LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

add_executable(hello gtksample.cpp)
target_link_libraries(hello PRIVATE ${GTK3_LIBRARIES})


# Add other flags to the compiler
target_compile_definitions(hello PRIVATE ${GTK_CFLAGS_OTHER})


# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries

target_include_directories(hello PRIVATE ${GTK3_INCLUDE_DIRS})
target_link_directories(hello PRIVATE ${GTK3_LIBRARY_DIRS})