# CMake config
# panui cross-platform gui lib 

# min version
cmake_minimum_required(VERSION 2.8)

# project name
set    (  PROJECT panui)
project(${PROJECT} )

# source files
set (srcDir .)

# sub
add_subDirectory (build/android)
add_subDirectory (build/ubuntu)
