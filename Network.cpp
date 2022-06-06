#include "Network.h"

#include <assert.h>

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
    #define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
    #define PLATFORM PLATFORM_MAC
#else
    #define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS
    #include <winsock2.h>
    #pragma comment(lib, "wsock32.lib")
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <fcntl.h>
#endif

class Address
{
public:
    Address() :
        address(0),
        port(0)
    {

    }

    Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port) :
        address((a << 24) | (b << 16) | (c << 8) | d),
        port(port)
    {

    }

    Address(unsigned int address, unsigned short port) :
        address(address),
        port(port)
    {

    }

    unsigned int GetAddress() const {return address;}

//    unsigned char GetA() const {return address;}
//    unsigned char GetB() const {return address;}
//    unsigned char GetC() const {return address;}
//    unsigned char GetD() const {return address;}

    unsigned short GetPort() const {return port;}

private:

    unsigned int address;
    unsigned short port;
};

class Socket
{
public:
    Socket();
    ~Socket();
    
    bool Open( unsigned short port );
    
    void Close();
    
    bool IsOpen() const;
    
    bool Send(const Address & destination, const void * data, int size);
    
    int Receive(Address & sender, void * data, int size);

private:
    int handle;
};

Socket::Socket() :
    handle(0)
{
}

Socket::~Socket()
{
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
    close(handle);
#elif PLATFORM == PLATFORM_WINDOWS
    closesocket(handle);
#endif
}

bool Socket::Open(unsigned short port)
{
    handle = (int)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (handle <= 0)
    {
        assert( "failed to create socket" );
        return false;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(handle, (const sockaddr*) &address, sizeof(sockaddr_in)) < 0)
    {
        assert( "failed to bind socket" );
        return false;
    }

#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
    int nonBlocking = 1;
    if (fcntl(handle, F_SETFL, O_NONBLOCK, nonBlocking ) == -1)
    {
        assert( "failed to set non-blocking" );
        return false;
    }
#elif PLATFORM == PLATFORM_WINDOWS
    DWORD nonBlocking = 1;
    if (ioctlsocket(handle, FIONBIO, &nonBlocking) != 0)
    {
        assert( "failed to set non-blocking" );
        return false;
    }
#endif

    return true;
}

void Socket::Close()
{
}

bool Socket::IsOpen() const
{
    return true;
}

bool Socket::Send(const Address & destination, const void * data, int size)
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(destination.GetAddress());
    addr.sin_port = htons(destination.GetPort());

    int bytes_sent = sendto( 
        handle, 
        (const char*)data, 
        size,
        0, 
        (sockaddr*)&addr, 
        sizeof(sockaddr_in));

    if (bytes_sent != size)
    {
        assert( "failed to send packet" );
        return false;
    }

    return true;
}

int Socket::Receive(Address & sender, void * data, int size)
{
#if PLATFORM == PLATFORM_WINDOWS
    typedef int socklen_t;
#endif

    sockaddr_in from;
    socklen_t fromLength = sizeof( from );

    int bytes = recvfrom( 
        handle, 
        (char*)data,
        size,
        0, 
        (sockaddr*)&from, 
        &fromLength);

    unsigned int from_address = ntohl(from.sin_addr.s_addr);
    unsigned int from_port = ntohs(from.sin_port);
    sender = Address(from_address, from_port);

    return bytes;
}

bool InitializeSockets()
{
#if PLATFORM == PLATFORM_WINDOWS
    WSADATA WsaData;
    if (WSAStartup(MAKEWORD(2,2), &WsaData) == NO_ERROR)
    {
        return true;
    }
    else
    {
        assert( "failed to initialise sockets" );
        return false;
    }
#else
    return true;
#endif
}

void ShutdownSockets()
{
#if PLATFORM == PLATFORM_WINDOWS
    WSACleanup();
#endif
}

bool TestConnection()
{
    // create socket

    const int port = 30000;

    Socket socket;

    if (!socket.Open( port ))
    {
        assert( "failed to create socket!" );
        return false;
    }

    // send a packet

    const char data[] = "hello world!";

    socket.Send(Address(127,0,0,1,port), data, sizeof( data ));

    // receive packets

    while ( true )
    {
        Address sender;
        unsigned char buffer[256];
        int bytes_read = socket.Receive(sender, buffer, sizeof(buffer));

        if (bytes_read <= 0)
        {
            break;
        }
     
        // process packet
    }

    return true;
}
