#ifndef _CLIENTSOCKET_H
#define _CLIENTSOCKET_H

class InetAddress;

class ClientSocket
{
public:
    ClientSocket();
    ~ClientSocket();

    void init(int protocol);
    void connect(InetAddress &);
    int get_socket() const;

private:
    int sockfd_;
};

#endif
