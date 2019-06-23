#include "TcpSocket.h"
#include "../Saper_server/common.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>


TcpSocket::TcpSocket(int port)
{
    int option = 1;

    this->port = port;
    this->client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    this->addr.sin_addr.s_addr = INADDR_ANY;
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(this->port);

#ifdef LANADDR
    this->addr.sin_addr.s_addr = inet_addr(LANADDR);
#endif

    if (setsockopt(this->client_socket_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
    {
        perror("ERROR: ");
        throw std::string("setsockopt fail");
    }

    if (connect(this->client_socket_fd, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0)
    {
        if(errno == EINPROGRESS)
        {
            close(this->client_socket_fd);
            perror("ERROR: ");
            throw std::string("connect in progress");
        }
        else if(errno != EADDRINUSE)
        {
            close(this->client_socket_fd);
            perror("ERROR: ");
            throw std::string("addr in use");
        }
    }

    this->set_socket_non_blocking();

    std::cout << "Client connected to port " << this->port << std::endl;
}

TcpSocket::~TcpSocket()
{
    close(this->client_socket_fd);
}

void TcpSocket::send_message(void *message, size_t size)
{
    if(sendto(this->client_socket_fd, message, size, 0, (struct sockaddr*) &this->addr, sizeof(this->addr)) < 0)
    {
        close(this->client_socket_fd);
        perror("ERROR: ");
        throw std::string("sendto fail");
    }
}

void TcpSocket::set_socket_non_blocking()
{
    int flags = fcntl(this->client_socket_fd, F_GETFL, 0);
    flags = (flags == -1 ? 0 : flags);
    fcntl(this->client_socket_fd, F_SETFL, flags | O_NONBLOCK);
}

size_t TcpSocket::receive_msg()
{
    ssize_t bytesReceived = this->recv_message();

    if (bytesReceived == 0)
    {
        std::cout << "Server closed connection" << std::endl;
        close(this->client_socket_fd);
        exit(0);
    }
}

void TcpSocket::handleConnections()
{
    this->receive_msg();
}
