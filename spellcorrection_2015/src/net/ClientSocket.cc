
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>

#include "ClientSocket.h"
#include "InetAddress.h"

ClientSocket::ClientSocket()
{}

void ClientSocket::init(int protocol)
{
    if(IPPROTO_TCP == protocol)
    {
        sockfd_ = ::socket(AF_INET, SOCK_STREAM, protocol);
        if(-1 == sockfd_)
        {
            perror("create socket error");
            //exit(EXIT_FAILURE);
        }
    }
    else
    {
        sockfd_ = ::socket(AF_INET, SOCK_DGRAM, protocol);
        if(-1 == sockfd_)
        {
            perror("create socket error");
            //exit(EXIT_FAILURE);
        }
    }
}


void ClientSocket::connect(InetAddress &addr)
{
    if(-1 == ::connect(sockfd_, 
        (struct sockaddr *)addr.getSockAddrInet(),
        sizeof(struct sockaddr)))
    {
        perror("connect failed");
        //exit(EXIT_FAILURE);
    }
}

int ClientSocket::get_socket()const
{
    return sockfd_;
}


