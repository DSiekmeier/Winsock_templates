/**
 * \brief       A simple template for a UDP client using Winsock
 *
 *              This example is not meant as an bullet proof implementation for commercial
 *              application. Its purpose is to serve as a starting point to avoid reinventing
 *              the wheel for socket programming. It might be helpful for beginners to see the
 *              general structure this kind of programs.
 *              Nevertheless, if you find any serious vulnerabilities please contact me.
 *
 * \author      Dennis Siekmeier (contact@DSiekmeier.de)
 * \date        26/04/2016 (last edit)
 *
 * \copyright   Coffee-Ware: Feel free to use this software "as-is", but buy me a coffee if we
                ever meet each other.
 */
    
#include <stdio.h>
#include <winsock2.h>

#define BUFSIZE 256
#define PORT 15123
#define ADDRESS "127.0.0.1"

// Initialize and define Winsock version which should be used
int startWinsock( void ) {
    WSADATA wsa;
    return WSAStartup( MAKEWORD(2,0), &wsa );
}

int main( void ) {
    long rc;
    SOCKET s;
    char buf[BUFSIZE];
    SOCKADDR_IN addr;
    SOCKADDR_IN remoteAddr;
    int remoteAddrLen = sizeof( SOCKADDR_IN );
    
    // 1) Call the initialization function
    rc = startWinsock();
    if( rc!=0 ) {
        printf( "Initialization failed. Error code: %ld\n", rc );
        return 1;
    }

    // 2) Create the socket
    s = socket( AF_INET, SOCK_DGRAM, 0 );
    if( s==INVALID_SOCKET ) {
        // WSAGetLastError() for more information
        printf( "Socket creation failed. Error code: %d\n", WSAGetLastError() );
        return 1;
    }

    // 3) Prepare address
    memset( &addr, 0, sizeof( SOCKADDR_IN ) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons( PORT );
    addr.sin_addr.s_addr = inet_addr( ADDRESS );

    // 4) Exchange data
    while( 1 ) {
        printf( "\nEnter string: " );
        fgets( buf, BUFSIZE, stdin );
        
        rc = sendto( s, buf, strlen(buf), 0, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN) );
        if( rc==SOCKET_ERROR ) {
            printf( "Send failed. Error code: %d\n", WSAGetLastError() );
            break;
        }

        rc = recvfrom( s, buf, BUFSIZE, 0, (SOCKADDR*)&remoteAddr, &remoteAddrLen );
        if( rc==SOCKET_ERROR ) {
            printf( "Receive failed. Error code: %d\n", WSAGetLastError() );
            break;
        }
        else
        {
            buf[rc]= '\0';
            printf( "Server echo\t: %s", buf );
        }
    }

    return 0;
}
