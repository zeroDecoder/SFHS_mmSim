#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <QDebug>

class networkInterface
{
private:

    SOCKET _socket;
    WSADATA _wsaData;
    IN_ADDR _subNet;
    char _port[8],*_ip;
    char name[255];
    struct sockaddr_in _server;
    struct addrinfo *_add_inf;
    PHOSTENT _hostinfo;

public:
    static const int localPort = 2367;
    networkInterface();
    void startServer();
    void connect(const char *ip);
    void connectLocal();
    void disconnect();
    bool isConnectionReady();
    int readData(char *buffer, int count);
    int sendData(char *buffer, int count);
};

#endif // NETWORKINTERFACE_H
