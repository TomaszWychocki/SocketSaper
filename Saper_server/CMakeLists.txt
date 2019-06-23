cmake_minimum_required(VERSION 3.14)
project(Saper_server)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        Saper_server
        main.cpp
        TcpSocket.cpp TcpSocket.h
        Player.cpp Player.h
        PlayerHandler.cpp PlayerHandler.h
        GameBoard.cpp GameBoard.h
        common.h)