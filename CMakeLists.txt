# Min cmake version
cmake_minimum_required(VERSION 3.28)

# Proj name + exe name
project(watchdog VERSION 1.0)

# Min c++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_DEPENDS_USE_COMPILER FALSE)

# Exe target name
add_executable(watchdog
    src/main.cc
    src/watcher.cc
    src/fileutil.cc
)

# Dir containing watchdog's headers
target_include_directories(watchdog PRIVATE include)

# Where to place compiled exe
set_target_properties(watchdog
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# --- Dependencies ---

# Add the subdirectory containing the logger's CMakeLists.txt
add_subdirectory(libs/cpp_logger)

# Link logger library to the watchdog executable
target_link_libraries(watchdog PRIVATE cpp_logger)