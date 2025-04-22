cmake_minimum_required(VERSION 3.8)

project(BlokchainSimSubProject LANGUAGES CXX)

# (Optional) Hard-code path to Qt install. Otherwise pass -DCMAKE_PREFIX_PATH=... on the command line.
# set(CMAKE_PREFIX_PATH "C:/Qt/6.8.2/msvc2022_64")

# Let CMake automatically process MOC, UIC, RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 1) Find Qt + OpenSSL
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(OpenSSL REQUIRED)

# 2) Create the executable target
add_executable(BlokchainSim
    main.cpp
    Block/Block.cpp
    Block/Block.h
    Blockchain/Blockchain.cpp
    Blockchain/Blockchain.h
    UI/mainwindow.cpp
    UI/mainwindow.h
    Node/Node.h 
    Node/FullNode.h 
    Node/MinerNode.h)

target_sources(BlokchainSim PRIVATE
    Blockchain/Transaction.h
    Blockchain/FileManager.h
    Blockchain/ChainStats.h
    Blockchain/concepts.hpp
)

target_include_directories(BlokchainSim PRIVATE nlohmann)


# 3) Set C++ standard (C++20)
if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET BlokchainSim PROPERTY CXX_STANDARD 20)
endif()

# 4) Link against Qt and OpenSSL
target_link_libraries(BlokchainSim
    PRIVATE
    Qt6::Widgets
    OpenSSL::SSL
    OpenSSL::Crypto
)
