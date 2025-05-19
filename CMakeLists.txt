cmake_minimum_required(VERSION 3.10)

# Project Name
project(llm-cli)

# Use C++17
set(CMAKE_CXX_STANDARD 17)

# Include the headers from include/
include_directories(include)

# Grab all .cpp files in src/
file(GLOB SOURCES "src/*.cpp")

# Create an executable from those sources
add_executable(llm_cli ${SOURCES})
