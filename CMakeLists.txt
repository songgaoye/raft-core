cmake_minimum_required(VERSION 3.20)

project(raft_core VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(raft_core
  src/ballot.cc
  src/cluster_config.cc
  src/simulated_network.cc
)
add_library(raft_core::raft_core ALIAS raft_core)

target_include_directories(raft_core
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_options(raft_core PRIVATE -Wall -Wextra -Wpedantic)

option(RAFT_CORE_BUILD_TESTS "Build raft-core tests" ON)

if(RAFT_CORE_BUILD_TESTS)
  enable_testing()

  add_executable(raft_core_membership_ballot_test
    tests/01_membership_ballot_test.cc
  )
  target_link_libraries(raft_core_membership_ballot_test PRIVATE raft_core)
  add_test(NAME raft_core_membership_ballot_test
           COMMAND raft_core_membership_ballot_test)

  add_executable(raft_core_simulated_network_test
    tests/02_simulated_network_test.cc
  )
  target_link_libraries(raft_core_simulated_network_test PRIVATE raft_core)
  add_test(NAME raft_core_simulated_network_test
           COMMAND raft_core_simulated_network_test)
endif()
