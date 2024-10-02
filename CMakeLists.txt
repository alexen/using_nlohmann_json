cmake_minimum_required(VERSION 3.15)

project(using_nlohnamm_json)

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
     cmake_policy(SET CMP0135 NEW)
endif()

find_package(Boost COMPONENTS headers REQUIRED)

include(3rdparty/nlohmann_json.cmake)
add_executable(
     ${PROJECT_NAME}
          src/main.cpp
          src/serialize.cpp
          src/serialize_io.cpp
     )
target_sources(
     ${PROJECT_NAME}
     PRIVATE
          src/json_types.h
          src/json_types_io.h
          src/serialize.h
          src/serialize_io.h
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
target_include_directories(${PROJECT_NAME} PRIVATE src)

include(CTest)
if(BUILD_TESTING)
     find_package(Boost COMPONENTS unit_test_framework REQUIRED)
     set(UTEST_NAME ${PROJECT_NAME}-test)
     add_executable(
          ${UTEST_NAME}
               src/serialize.cpp
               src/test/start_test.cpp
               src/test/serialize_test.cpp
          )
     target_link_libraries(
          ${UTEST_NAME}
          PRIVATE
               Boost::unit_test_framework

          )
     target_include_directories(
          ${UTEST_NAME}
          PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME},INCLUDE_DIRECTORIES>
          )
     add_test(NAME ${PROJECT_NAME} COMMAND ${UTEST_NAME})
endif()
