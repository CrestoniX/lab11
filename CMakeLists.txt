cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(
  HUNTER_CACHE_SERVERS
  "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
  CACHE STRING "Default cache server"
)

include("tools/gate/cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/ruslo/hunter/archive/v0.23.214.tar.gz"
  SHA1 "e14bc153a7f16d6a5eeec845fb0283c8fad8c358"
)

project(AutoPacker)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wnon-virtual-dtor")

hunter_add_package(
  Boost
  COMPONENTS
  program_options
  thread
  log
  filesystem
)
find_package(
  Boost
  CONFIG REQUIRED
  program_options
  thread
  log
  log_setup
  filesystem
)
hunter_add_package(Async++)
find_package(Async++ CONFIG REQUIRED)
find_package(Threads)

add_executable(
  ${PROJECT_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources1/Command.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources1/Settings.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/LogSetup.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/Timer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/Utils.cpp
)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/boost-process/include
)

target_link_libraries(
  ${PROJECT_NAME}
  Async++::Async++
  Boost::program_options
  Boost::thread
  Boost::log
  Boost::log_setup
  Boost::filesystem
)
if(BUILD_COVERAGE)
    set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    find_package(codecov)
    # TODO: change template word to project name and delete this comment
    add_coverage(template)
    add_coverage(tests)
    list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/tests/*'")
    coverage_evaluate()
endif()
