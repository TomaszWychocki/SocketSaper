#ifndef SAPER_CLIENT_TCPSOCKET_H
#define SAPER_CLIENT_TCPSOCKET_H

#include <netinet/in.h>


class TcpSocket
{
public:
    TcpSocket(int port);
    ~TcpSocket();
    void handleConnections();
    virtual ssize_t recv_message() = 0;

private:
    void set_socket_non_blocking();

    struct sockaddr_in addr;
    int port;

protected:
    void send_message(void *message, size_t size);
    size_t receive_msg();

    int client_socket_fd;
};


#endif //SAPER_CLIENT_TCPSOCKET_H