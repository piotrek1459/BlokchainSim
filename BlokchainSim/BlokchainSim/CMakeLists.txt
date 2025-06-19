cmake_minimum_required(VERSION 3.14)        # FetchContent needs ≥3.14
project(BlokchainSimSubProject LANGUAGES CXX)
# ------------------------------------------------------------------
# Enable Qt automatic code generation (MOC / UIC / RCC)
# ------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ------------------------------------------------------------
# Qt & OpenSSL
# ------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(OpenSSL REQUIRED)

# ------------------------------------------------------------
# Google-Test via FetchContent
# ------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ------------------------------------------------------------
# 1) Core static library  (no GUI code)
# ------------------------------------------------------------
add_library(blockchain_core STATIC
    Block/Block.cpp
    Blockchain/Blockchain.cpp
)
target_include_directories(blockchain_core
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}   # so headers resolve everywhere
      nlohmann                      # JSON single-header folder
)
target_link_libraries(blockchain_core
    PUBLIC
      OpenSSL::SSL
      OpenSSL::Crypto
)

# ------------------------------------------------------------
# 2) GUI executable – links Qt + core lib
# ------------------------------------------------------------
add_executable(BlokchainSim
    main.cpp
    UI/mainwindow.cpp
    UI/mainwindow.h 
    Node/Node.h
    Node/FullNode.h
    Node/MinerNode.h
)
target_link_libraries(BlokchainSim
    PRIVATE
      Qt6::Widgets
      blockchain_core
)

# ------------------------------------------------------------
# 3) Google-Test target – links only core lib + GTest
# ------------------------------------------------------------
add_executable(tests
    tests/test_main.cpp
    tests/test_block.cpp
    tests/test_blockchain.cpp
    tests/test_transaction.cpp         
    tests/test_pow.cpp                 
    tests/test_chain_validation.cpp   
    tests/test_filemanager.cpp    
)
target_link_libraries(tests
    PRIVATE
      gtest_main
      blockchain_core
)

include(GoogleTest)          # <— one-time include (ships with GTest)
gtest_discover_tests(tests)  # <— registers individual TEST() cases

enable_testing()
add_test(NAME unit COMMAND tests)

# ------------------------------------------------------------
# Compiler options
# ------------------------------------------------------------
set_property(TARGET BlokchainSim tests blockchain_core
             PROPERTY CXX_STANDARD 20)
