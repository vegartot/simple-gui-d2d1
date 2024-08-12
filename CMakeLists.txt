cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 23)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
  message(FATAL_ERROR "In-tree build. Make a separate output folder")
endif()

if(CMAKE_SYSTEM MATCHES Windows)
  message(STATUS "Target system: Windows")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(exe_name "win64")

project(app)
add_executable(${exe_name})

if (CMAKE_COMPILER_IS_GNUCXX)
  message(STATUS "Compiler is GNU g++")
  set_target_properties(win64 PROPERTIES LINK_FLAGS "-municode -mwin32 -mwindows")
  add_compile_definitions(UNICODE)
  target_compile_options(win64 PUBLIC "-g")
endif()

file(GLOB_RECURSE src_files CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE header_files CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)


#add_library(${PROJECT_NAME} ${src_files} ${header_files} )

target_include_directories(${exe_name} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_sources(${exe_name} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/main.h" "${CMAKE_CURRENT_SOURCE_DIR}/resource/resource.rc")
#target_sources(${exe_name} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
target_link_libraries(${exe_name} PUBLIC d2d1.lib dxgi.lib dwrite.lib) 

