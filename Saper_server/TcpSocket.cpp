#include "TcpSocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cerrno>
#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <algorithm>
#include <vector>


TcpSocket::TcpSocket(int port, std::vector<Player*> &players)
    : max_number_of_events(255)
    , players(players)
{
    this->port = port;
    this->server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(static_cast<uint16_t>(this->port));
    this->addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(this->server_socket_fd, (struct sockaddr*)&this->addr, sizeof(this->addr)) == -1)
    {
        perror("ERROR: ");
        throw std::string("Bind fail");
    }

    this->set_socket_non_blocking(this->server_socket_fd);

    if(listen(this->server_socket_fd, 5) == -1)
    {
        perror("ERROR: ");
        throw std::string("Listen fail");
    }

    if((this->epoll_fd = epoll_create(this->max_number_of_events)) == -1)
    {
        perror("ERROR: ");
        throw std::string("Bind fail");
    }

    std::memset(&(this->event), 0, sizeof(struct epoll_event));
    this->event.data.fd = this->server_socket_fd;
    this->event.events = EPOLLIN | EPOLLET;

    if(epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->server_socket_fd, &this->event) == -1)
    {
        perror("ERROR: ");
        throw std::string("Epoll_ctl fail");
    }

    this->events = new epoll_event[this->max_number_of_events];
    std::memset(this->events, 0, sizeof(struct epoll_event) * this->max_number_of_events);

    std::cout << "Listen on port " << port << std::endl;
}

TcpSocket::~TcpSocket()
{
    for(auto player : this->players)
    {
        close(player->getSocketFd());
    }

    delete[] this->events;
    close(this->server_socket_fd);
}

void TcpSocket::set_socket_non_blocking(int socket_fd)
{
    int flags = fcntl(socket_fd, F_GETFL, 0);
    flags = (flags == -1 ? 0 : flags);
    fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
}

void TcpSocket::handle_connections()
{
    int number_of_incoming_events;

    number_of_incoming_events = epoll_wait(this->epoll_fd, this->events, this->max_number_of_events, 0);

    for(int i = 0; i < number_of_incoming_events; i++)
    {
        if(this->events[i].events & EPOLLIN)
        {
            if (this->server_socket_fd == this->events[i].data.fd)
            {
                this->accept_new_connection();
            }
            else
            {
                this->read_incoming_data(this->events[i].data.fd);
            }
        }
        else
        {
            close(this->events[i].events);
        }
    }
}

void TcpSocket::accept_new_connection()
{
    socklen_t client_socket_length = sizeof(this->client);
    int new_client_fd;

    while((new_client_fd = accept(this->server_socket_fd, &this->client, &client_socket_length)) != -1)
    {
        this->set_socket_non_blocking(new_client_fd);

        this->event.data.fd = new_client_fd;
        this->event.events = EPOLLIN | EPOLLET;

        if(epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, new_client_fd, &this->event) == -1)
        {
            perror("ERROR: ");
            throw std::string("Epoll_ctl fail");
        }

        std::cout << "New client connected: " << new_client_fd << std::endl;
    }
}

void TcpSocket::read_incoming_data(int event_fd)
{
    ssize_t bytesReceived = this->recv_message(event_fd);

    //std::cout << "Received " << bytesReceived << " bytes from " << event_fd << std::endl;

    if(bytesReceived == -1 && errno != EAGAIN)
    {
        if (errno == ECONNRESET)
        {
            this->closeConnection(event_fd);
        }
        else
        {
            perror("ERROR: ");
            throw std::string("Read fail");
        }
    }
    else if(bytesReceived == 0)
    {
        this->closeConnection(event_fd);
    }
}

void TcpSocket::send_message(int socket_fd, void *message, size_t size)
{
    if(sendto(socket_fd, message, size, 0, (struct sockaddr*) &this->addr, sizeof(this->addr)) < 0)
    {
        close(socket_fd);
        perror("ERROR: ");
        throw std::string("sendto fail");
    }
}

void TcpSocket::closeConnection(int event_fd)
{
    auto player =
        std::find_if(this->players.begin(), this->players.end(), [this, &event_fd](Player* player)
        {
            return player->getSocketFd() == event_fd;
        });

    if (player != this->players.end())
    {
        this->onCloseConnection(*player);
        std::cout << "Player disconnected: " << (*player)->getName() << std::endl;
        delete *player;
        this->players.erase(player);
        close(event_fd);
    }
    else
    {
        std::cout << "Unknown player disconnected: " << event_fd << std::endl;
    }
}

void TcpSocket::onCloseConnection(Player *player)
{

}
