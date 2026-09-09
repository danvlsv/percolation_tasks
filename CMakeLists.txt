cmake_minimum_required(VERSION 3.20)

project(percolation VERSION 0.1 LANGUAGES CXX)

find_package(fmt REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE PERCOLATION_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(percolation_core STATIC ${PERCOLATION_SOURCES})
target_include_directories(percolation_core
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(percolation_core PUBLIC fmt::fmt)

target_compile_features(percolation_core PUBLIC cxx_std_23)

file(GLOB TASK_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/tasks/*.cpp"
)

foreach(TASK_SRC IN LISTS TASK_SOURCES)
    get_filename_component(TASK_NAME ${TASK_SRC} NAME_WE)
    add_executable(${TASK_NAME} ${TASK_SRC})
    target_link_libraries(${TASK_NAME} PRIVATE percolation_core)
endforeach()
