cmake_minimum_required(VERSION 3.30.2)

# Set the project name and version
project(MinecraftClone)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include directories for dependencies
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/include)

# build executable
file(GLOB SOURCES 
    "src/*.cpp" 
    "src/LoggingService/*.cpp"
    "src/WindowService/*.cpp"
)
add_executable(${PROJECT_NAME} ${SOURCES})

# link libraries
target_link_libraries(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/dependencies/lib/libglfw3.a")
target_link_libraries(${PROJECT_NAME} "opengl32")