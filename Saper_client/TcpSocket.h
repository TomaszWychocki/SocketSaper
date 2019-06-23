#ifndef SAPER_CLIENT_TCPSOCKET_H
#define SAPER_CLIENT_TCPSOCKET_H

#include <netinet/in.h>


class TcpSocket
{
public:
    TcpSocket(int port);

    virtual ~TcpSocket();

private:
    void set_socket_non_blocking();

    int client_socket_fd;
    struct sockaddr_in addr;
    int port;

protected:
    void send_message(void *message, size_t size);
};


#endif //SAPER_CLIENT_TCPSOCKET_H