
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

# # Setup CMake to use GTK+, tell the compiler where to look for headers
# # and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})
# set(GLIB_INLCUDE_DIRS "/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/include/glib-2.0")

# set(GTK3_INCLUDE_DIRS "/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/include/gtk-3.0")

# include_directories("/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/include/glib-2.0")
# include_directories("/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/lib/glib-2.0/include")
# include_directories("/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/include/pango-1.0/")
# include_directories("/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/include/cairo/")
# include_directories("/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/include/gdk-pixbuf-2.0/")
# include_directories("/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/include/atk-1.0/")


#link_directories("/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/lib/")



# Add other flags to the compiler
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})
add_executable(hello gtksample.cpp)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# target_link_libraries(hello PRIVATE "/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/lib/libgtk-3.so" 
# "/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/lib/libgdk-3.so" 
# "/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/lib/libglib-2.0.so" 
# "/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/lib/libffi.so" 
# "/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/lib/libcairo.so" 

# "/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/lib/libgdk_pixbuf-2.0.so"
# "/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/lib/libgobject-2.0.so")
set(waylandlib  "/home/tyler/Downloads/wayland-1.21.0/build/src/libwayland-client.so" "/usr/lib/x86_64-linux-gnu/libwayland-egl.so.1" "/usr/lib/x86_64-linux-gnu/libEGL.so")

target_link_libraries(hello PRIVATE ${GTK3_LIBRARIES} ${waylandlib} ${OPENGL_LIBRARY} )
# Add other flags to the compiler
target_compile_definitions(hello PRIVATE ${GTK_CFLAGS_OTHER})

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries

target_include_directories(hello PRIVATE  ${GTK3_INCLUDE_DIRS})

target_link_directories(hello PRIVATE ${GTK3_LIBRARY_DIRS} )


# # Thanks to @danger89 and @Ilothar for updating the gist.
# # Set the name and the supported language of the project
# project(hello-world C CXX)
# # include(FindGTK)

# # Set the minimum version of cmake required to build this project
# cmake_minimum_required(VERSION 3.12)


# #link_directories("/home/tyler/develop/unityTrunk/unity/artifacts/Stevedore/linux-sysroot-amd64_21db/usr/lib/")

# add_executable(hello gtksample.cpp)


# # Add other flags to the compiler

# set(GTK3_INCLUDE_DIRS "/opt/gtk/include" "/opt/gtk/include/gtk-3.0" "/usr/include/glib-2.0" "/usr/lib/x86_64-linux-gnu/glib-2.0/include" "/usr/include/pango-1.0" "/usr/include/harfbuzz" "/usr/include/cairo" "/usr/include/gdk-pixbuf-2.0" "/usr/include/atk-1.0")
# set(GTK3_LIBRARY_DIRS "/opt/gtk/lib")

# target_include_directories(hello PRIVATE  ${GTK3_INCLUDE_DIRS})
# target_link_directories(hello PRIVATE ${GTK3_LIBRARY_DIRS})

