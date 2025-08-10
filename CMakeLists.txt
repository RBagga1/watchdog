# Min cmake version
cmake_minimum_required(VERSION 3.28)

# Proj name + exe name
project(watchdog VERSION 1.0)

# Min c++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# === Core library ===
add_library(watchdog_core
    src/watcher.cc
    src/fileutil.cc
    src/config.cc
    src/checkargs.cc
    src/system.cc
)

# Core library includes
target_include_directories(watchdog_core PUBLIC include)

# Link dependencies for core library
target_link_libraries(watchdog_core
    PRIVATE cpp_logger
    PRIVATE nlohmann_json::nlohmann_json
)

# === Executable ===
add_executable(watchdog src/main.cc)

# Executable depends on core library
target_link_libraries(watchdog PRIVATE watchdog_core cpp_logger nlohmann_json::nlohmann_json)

# Dir containing watchdog's headers
target_include_directories(watchdog PRIVATE include)

# Where to place compiled exe
set_target_properties(watchdog
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# --- Dependencies ---
add_subdirectory(libs/cpp_logger)
add_subdirectory(libs/json)
