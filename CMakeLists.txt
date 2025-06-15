cmake_minimum_required(VERSION 3.30.2)

# Set the project name and version
project(MinecraftClone)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

# include directories for dependencies
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/include)

# set output directories for executables
set(CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/build")

# build executable
file(GLOB SOURCES 
    "src/*.cpp" 
    "src/LoggingService/*.cpp"
    "src/WindowService/*.cpp"
    "src/RenderService/*.cpp"
)
add_executable(${PROJECT_NAME} ${SOURCES})

# define GLEW_STATIC for static linking
add_definitions(-DGLEW_STATIC)

# link libraries
target_link_libraries(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/dependencies/lib/libglfw3.a")
target_link_libraries(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/dependencies/lib/libglew32.a")
target_link_libraries(${PROJECT_NAME} "opengl32")

# build resources
set(RESOURCES_DIR "${PROJECT_SOURCE_DIR}/resources")
set(OUTPUT_RESOURCES_DIR "${CMAKE_BINARY_DIR}/resources")
file(MAKE_DIRECTORY ${OUTPUT_RESOURCES_DIR})
add_custom_target(CopyResources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${RESOURCES_DIR}
    ${OUTPUT_RESOURCES_DIR}
    DEPENDS ${RESOURCES_DIR}
)
add_dependencies(${PROJECT_NAME} CopyResources)
