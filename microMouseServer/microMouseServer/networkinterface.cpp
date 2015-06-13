#include "networkinterface.h"

networkInterface::networkInterface()
{
    char name[255];
    _socket = INVALID_SOCKET;
    _add_inf = NULL;
    _ip = NULL;
    if(WSAStartup(MAKEWORD(2,2), &_wsaData) != NO_ERROR)
    {
        qDebug("WSA library could not be found!");
    }
    else
    {
        if(gethostname(name, sizeof(name)) == 0) //get local hostname
        {
            if((_hostinfo = gethostbyname(name)) != NULL) //get IP by hostname
            {
                // if hostname is associated with more than 1 IP then select the last one
                for(int nCount = 0;_hostinfo->h_addr_list[nCount]; nCount++)
                    _ip = inet_ntoa(*(struct in_addr *)_hostinfo->h_addr_list[nCount]);
            }
        }
    }

}

void networkInterface::connectLocal()
{
    this->connect(_ip);
}

void networkInterface::connect(const char *ip)
{
    if(_socket != INVALID_SOCKET)
    {
        qDebug("Socket already in use!");
        return;
    }

    int iResult;
    struct addrinfo hints;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(ip, _port, &hints, &_add_inf);
    if(iResult != 0)
    {
        qDebug() << "failed with error code: " << iResult;
        return;
    }

    // Create SOCKET
    _socket = socket(_add_inf->ai_family, _add_inf->ai_socktype,
        _add_inf->ai_protocol);
    if (_socket == INVALID_SOCKET) {
        qDebug() << "socket failed with error: " << WSAGetLastError();
    }

    if (::connect(_socket, _add_inf->ai_addr, _add_inf->ai_addrlen))
    {
        switch (WSAGetLastError()){
        case WSAEADDRINUSE:
            qDebug("The local address of the socket is already in use and the socket was not marked to allow address reuse");
            break;
        case WSAENETDOWN:
            qDebug("The network subsystem has failed.");
            break;
        case WSAEALREADY:
            qDebug("A nonblocking connect or WSAConnect call is in progress on the specified socket.");
            break;
        case WSAEADDRNOTAVAIL:
            qDebug("The remote address is not a valid address (such as ADDR_ANY).");
            break;
        case WSAECONNREFUSED:
            qDebug("The attempt to connect was rejected.");
            break;
        case  WSAEISCONN:
            qDebug("The socket is already connected (connection-oriented sockets only).");
            break;
        case WSAEHOSTUNREACH:
            qDebug("A socket operation was attempted to an unreachable host.");
            break;
        case WSAENOBUFS:
            qDebug("No buffer space is available. The socket cannot be connected.");
            break;
        case WSAETIMEDOUT:
            qDebug("Attempt to connect timed out without establishing a connection.");
            break;
        case WSAEACCES:
            qDebug("Attempt to connect datagram socket to broadcast address failed because setsockopt SO_BROADCAST is not enabled.");
            break;
        default:
            break;
        }
        this->disconnect(); //there was a problem so clear the socket
    }
    freeaddrinfo(&hints);
}

bool networkInterface::isConnectionReady()
{
    return _socket != INVALID_SOCKET ? true : false;
}

void networkInterface::disconnect()
{
   freeaddrinfo(_add_inf);

   //shutdown socket and free all asociated memory
   shutdown(_socket, SD_BOTH);
   if(closesocket(_socket))
   {
       switch (WSAGetLastError()) {
        case WSANOTINITIALISED:
            qDebug("WSA library was not initilaized, WSA startup must be called first");
            break;
        case WSAENETDOWN:
            qDebug("The network subsytem failed");
            break;
        case WSAEINPROGRESS:
            qDebug("A blocking WSA call is in progress");
            do{
                if(!closesocket(_socket))break;
            }while(WSAGetLastError() != WSAEINPROGRESS);
            break;
        case WSAEWOULDBLOCK:
             qDebug("Call would block WSA subsytem");
             do{
                closesocket(_socket);
            }while(WSAGetLastError() != WSAEWOULDBLOCK);
            break;
        default:
            break;
       }
   }

   _socket = INVALID_SOCKET;
   _add_inf = NULL;
}

int networkInterface::readData(char *buffer, int count)
{
    if(_socket == INVALID_SOCKET)
    {
        qDebug("Socket is not connected!");
        return 0;
    }
    int dataRead = 0;
    dataRead = recv(_socket,buffer, count, 0);
    if(dataRead == 0)
    {
        qDebug("connection closed remotly");
        this->disconnect();
    }
    else if(dataRead == SOCKET_ERROR)
    {
        switch (WSAGetLastError()) {
        case WSAENETDOWN:
            qDebug("The network subsystem has failed.");
            break;
        case WSAEFAULT:
            qDebug("The buffer is not completely contained in a valid part of the user address space.");
            break;
        case WSAENOTCONN:
            qDebug("The socket is not connected.");
            break;
        case WSAEINTR:
            qDebug("The blocking call was canceled through WSACancelBlockingCall.");
            break;
        case WSAESHUTDOWN:
            qDebug("The socket has been shut down.");
            break;
        case WSAEMSGSIZE:
            qDebug("The message was too large to fit into the specified buffer and was truncated.");
            break;
        case WSAETIMEDOUT:
            qDebug("The connection has been dropped because of a network failure or because the peer system failed to respond.");
            break;
        case WSAECONNRESET:
            qDebug("The virtual circuit was reset by the remote side executing a hard or abortive close.");
            this->disconnect();
            break;
        default:
            break;
        }
        return 0;
    }
    return dataRead;
}

int networkInterface::sendData(char *buffer, int count)
{
    int dataSent =0;
    dataSent = send(_socket, buffer, count, 0);
    if(dataSent == SOCKET_ERROR)
    {
        switch (WSAGetLastError()) {
        case WSAENETDOWN:
            qDebug("The network subsystem has failed.");
            break;
        case WSAEFAULT:
            qDebug("The buffer is not completely contained in a valid part of the user address space.");
            break;
        case WSAENOBUFS:
            qDebug("No buffer space is available.");
            break;
        case WSAENOTCONN:
            qDebug("The socket is not connected.");
            break;
        case WSAESHUTDOWN:
            qDebug("The socket has been shut down.");
            break;
        case WSAEWOULDBLOCK:
            qDebug("The socket is marked as nonblocking and the requested operation would block.");
            break;
        case WSAEMSGSIZE:
            qDebug("The message is larger than the maximum supported by the underlying transport.");
            break;
        case WSAEHOSTUNREACH:
            qDebug("The remote host cannot be reached from this host at this time.");
            break;
        case WSAECONNABORTED:
            qDebug("The virtual circuit was terminated due to a time-out or other failure.");
            this->disconnect();
            break;
        case WSAECONNRESET:
            qDebug("The virtual circuit was reset by the remote side executing a hard or abortive close.");
            this->disconnect();
            break;
        default:
            break;
        }
        return 0;
    }
    return dataSent;
}

void networkInterface::startServer()
{
    SOCKET holder = INVALID_SOCKET;
    if(_socket != INVALID_SOCKET)
    {
        qDebug("Cannont start listening on selected socket as it is alread in use");
        return;
    }

    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == INVALID_SOCKET)
    {
        qDebug() << "socket failed with error: " << WSAGetLastError();
        return;
    }

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(this->localPort);

    if(bind(_socket, (SOCKADDR*) &service, sizeof(service)) == SOCKET_ERROR)
    {
        qDebug("Could not bind socket.");
        closesocket(_socket);
        return;
    }
    qDebug() << "listening on port: " << service.sin_port;
    if(listen(_socket, 1) == SOCKET_ERROR)
    {
        qDebug("Could not listen on port!");
        closesocket(_socket);
        return;
    }

    holder = accept(_socket, NULL, NULL);
    if(holder == INVALID_SOCKET)
    {
        qDebug("Could not accept connection from client!");
    }
    else
        qDebug("connected to client.");
    closesocket(_socket);
    _socket = holder;
}
