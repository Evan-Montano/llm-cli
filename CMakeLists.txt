cmake_minimum_required(VERSION 3.10)
project(llm-cli)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# FTXUI must be declared FIRST
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v4.1.1
)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.11.2
)

# Important: FTXUI must be made available before json
FetchContent_MakeAvailable(ftxui json)

file(GLOB SOURCES "src/*.cpp")
add_executable(llm_cli ${SOURCES})

# Link correct FTXUI targets
target_link_libraries(llm_cli PRIVATE 
  ftxui::component   # For interactive components
  ftxui::dom         # For DOM elements
  ftxui::screen      # For screen handling
)

target_link_libraries(llm_cli PRIVATE nlohmann_json::nlohmann_json)