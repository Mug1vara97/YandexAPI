cmake_minimum_required(VERSION 3.20)

project(test)

set(CMAKE_CXX_STANDARD 11)

find_package(CURL REQUIRED)
find_package(cpr REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(test src/main.cpp)

target_include_directories(test PRIVATE ${CURL_INCLUDE_DIRS})
target_include_directories(test PRIVATE ${CPR_INCLUDE_DIRS})
target_include_directories(test PRIVATE ${nlohmann_json_INCLUDE_DIRS})

target_link_libraries(test PRIVATE ${CURL_LIBRARIES})
target_link_libraries(test PRIVATE ${CPR_LIBRARIES})
target_link_libraries(test PRIVATE nlohmann_json::nlohmann_json)