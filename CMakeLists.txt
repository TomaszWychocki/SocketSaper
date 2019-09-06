cmake_minimum_required(VERSION 3.14)
project(Saper)

set(CMAKE_CXX_STANDARD 14)

add_executable(Client 
               Saper_client/main.cpp
               Saper_client/TcpSocket.cpp Saper_client/TcpSocket.h
               Saper_client/PlayerHandler.cpp Saper_client/PlayerHandler.h
               Saper_client/GameBoard.cpp Saper_client/GameBoard.h
               Saper_client/colors.h)

add_executable(Server
               Saper_server/main.cpp
               Saper_server/TcpSocket.cpp Saper_server/TcpSocket.h
               Saper_server/Player.cpp Saper_server/Player.h
               Saper_server/PlayerHandler.cpp Saper_server/PlayerHandler.h
               Saper_server/GameBoard.cpp Saper_server/GameBoard.h
               Saper_server/common.h)
