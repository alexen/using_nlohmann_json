cmake_minimum_required(VERSION 3.15)

project(using_nlohnamm_json)

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
     cmake_policy(SET CMP0135 NEW)
endif()

find_package(Boost REQUIRED)

include(3rdparty/nlohmann_json.cmake)
add_executable(
     ${PROJECT_NAME}
          src/main.cpp
          src/serialize.cpp
     )
target_link_libraries(
     ${PROJECT_NAME}
     PRIVATE
          nlohmann_json::nlohmann_json
     )
set_target_properties(
     ${PROJECT_NAME}
     PROPERTIES
          CXX_STANDARD 17
          CXX_STANDARD_REQUIRED ON
          CXX_EXTENSIONS OFF
     )
