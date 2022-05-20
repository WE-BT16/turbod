# minimum CMake version required
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
# Project name and version
project(turbod VERSION 0.1.0 LANGUAGES CXX)

# Add libs
add_subdirectory(libs/spdlog)
add_subdirectory(libs/yaml-cpp)

include_directories(${PROJECT_NAME} PRIVATE
    libs/spdlog/include
    libs/yaml-cpp/include
)

# Create an executable
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)

# Headers
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Set C++ version
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)