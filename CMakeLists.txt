cmake_minimum_required(VERSION 3.30.2)

# Set the project name and version
project(MinecraftClone)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(${PROJECT_NAME} src/main.cpp)