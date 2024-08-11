cmake_minimum_required(VERSION 3.18)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
  message(FATAL_ERROR "In-tree build. Make a separate output folder")
endif()

if(CMAKE_SYSTEM MATCHES Windows)
  message(STATUS "Target system: Windows")
endif()

project(app)
add_executable(win64)

if (CMAKE_COMPILER_IS_GNUCXX)
  message(STATUS "Compiler is GNU g++")
  set_target_properties(win64 PROPERTIES LINK_FLAGS "-municode -mwin32 -mwindows")
  add_compile_definitions(UNICODE)
  target_compile_options(win64 PUBLIC "-g")
endif()

target_sources(win64 PRIVATE main.cpp)
target_link_libraries(win64 PUBLIC d2d1.lib dxgi.lib dwrite.lib)
