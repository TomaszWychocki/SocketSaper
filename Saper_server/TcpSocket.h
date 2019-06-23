#ifndef SAPER_SERVER_TCPSOCKET_H
#define SAPER_SERVER_TCPSOCKET_H

#include "Player.h"
#include <vector>
#include <netinet/in.h>
#include <sys/epoll.h>


class TcpSocket
{
public:
    TcpSocket(int port, std::vector<Player*> &players);
    ~TcpSocket();
    void handle_connections();

private:
    void set_socket_non_blocking(int socket_fd);
    void accept_new_connection();
    void read_incoming_data(int event_fd);
    void closeConnection(int event_fd);
    virtual ssize_t recv_message(int event_fd) = 0;

    int port;
    int server_socket_fd, epoll_fd;
    struct sockaddr_in addr;
    struct sockaddr client;
    struct epoll_event event;
    struct epoll_event *events;
    const int max_number_of_events;

protected:
    std::vector<Player*> &players;
    void send_message(int socket_fd, void *message, size_t size);
    virtual void onCloseConnection(Player* player);
};


#endif //SAPER_SERVER_TCPSOCKET_H
