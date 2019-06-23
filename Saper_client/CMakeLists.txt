cmake_minimum_required(VERSION 3.14)
project(Saper_client)

set(CMAKE_CXX_STANDARD 14)

add_executable(Saper_client main.cpp TcpSocket.cpp TcpSocket.h PlayerHandler.cpp PlayerHandler.h GameBoard.cpp GameBoard.h)