cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME yt-search.h)
set(PROJECT_NAME_STATIC ${PROJECT_NAME}_static)

if (NOT NLOHMANN_JSON_INCLUDE_DIR)
  message("-- Variable NLOHMANN_JSON_INCLUDE_DIR not defined")
endif()
if (NOT CURLPP_INCLUDE_DIR)
  message("-- Variable CURLPP_INCLUDE_DIR not defined")
endif()
if (NOT CURLPP_LIB_DIR)
  message("-- Variable CURLPP_LIB_DIR not defined")
endif()

set(PROJECT_INCLUDE_DIR ${NLOHMANN_JSON_INCLUDE_DIR} ${CURLPP_INCLUDE_DIR})
set(PROJECT_LIB_DIR ${CURLPP_LIB_DIR})

project(${PROJECT_NAME}
  LANGUAGES CXX)

file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/yt-search/*")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/yt-search/*")

option(YTSEARCH_BUILD_SHARED_LIBS "Build shared libraries" ON)

if(YTSEARCH_BUILD_SHARED_LIBS)
  add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
  target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" ${PROJECT_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} curlpp)
  target_link_directories(${PROJECT_NAME} PUBLIC ${PROJECT_LIB_DIR})

  set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    SOVERSION 1
    VERSION 1.0.0
    EXPORT_COMPILE_COMMANDS ON)
endif()

add_library(${PROJECT_NAME_STATIC} STATIC ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME_STATIC} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" ${PROJECT_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME_STATIC} curlpp)
target_link_directories(${PROJECT_NAME_STATIC} PUBLIC ${PROJECT_LIB_DIR})

set_target_properties(${PROJECT_NAME_STATIC} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  SOVERSION 1
  VERSION 1.0.0
  EXPORT_COMPILE_COMMANDS ON)

add_executable(example ${HEADER_FILES} src/example.cpp ${SOURCE_FILES})
target_link_libraries(example ${PROJECT_NAME} curl)
