/**
 * \brief       A simple template for a TCP server using Winsock
 *
 *              This example is not meant as an bullet proof implementation for commercial
 *              application. Its purpose is to serve as a starting point to avoid reinventing
 *              the wheel for socket programming. It might be helpful for beginners to see the
 *              general structure this kind of programs.
 *              Nevertheless, if you find any serious vulnerabilities please contact me.
 *
 * \author      Dennis Siekmeier (contact@DSiekmeier.de)
 * \date        24/11/2016 (last edit)
 *
 * \copyright   Coffee-Ware: Feel free to use this software "as-is", but buy me a coffee if we
                ever meet each other.
 */

#include <stdio.h>
#include <winsock2.h>

#define RX_BUFSIZE  256
#define TX_BUFSIZE  300
#define PORT        15123

// Initialize and define Winsock version which should be used
int startWinsock( void ) {
    WSADATA wsa;
    return WSAStartup( MAKEWORD(2,0), &wsa );
}

int main( void ) {
    long rc;
    SOCKET acceptSocket;
    SOCKET connectedSocket;
    SOCKADDR_IN addr;
    char buf[RX_BUFSIZE];
    char buf2[TX_BUFSIZE];

    // 1) Call the initialization function
    rc = startWinsock();
    if( rc!=0 ) {
        printf( "Initialization failed. Error code: %ld\n", rc );
        return 1;
    }

    // 2) Create the socket
    acceptSocket = socket( AF_INET,SOCK_STREAM, 0 );
    if( acceptSocket==INVALID_SOCKET ) {
        // WSAGetLastError() for more information
        printf( "Socket creation failed. Error code: %d\n", WSAGetLastError() );
        return 1;
    }

    // 3) Bind the socket to address
    memset( &addr, 0, sizeof( SOCKADDR_IN ) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons( PORT );
    addr.sin_addr.s_addr = ADDR_ANY;
    
    rc = bind( acceptSocket, (SOCKADDR*)&addr, sizeof( SOCKADDR_IN ) );
    if( rc==SOCKET_ERROR ) {
        printf( "Binding failed. Error code: %d\n", WSAGetLastError() );
        return 1;
    }

    // 4) Start listening for incoming connection requests
    rc = listen( acceptSocket, 10 ); // maximum pending requests: 10
    if( rc==SOCKET_ERROR ) {
        printf( "Entering listen mode failed. Error code: %d\n", WSAGetLastError() );
        return 1;
    }

    // 5) Accept a connection
    connectedSocket = accept( acceptSocket, NULL, NULL );
    if( connectedSocket==INVALID_SOCKET ) {
        printf( "Accepting connection failed. Error code: %d\n", WSAGetLastError() );
        return 1;
    }

    // 6) Exchange data
    while( rc!=SOCKET_ERROR ) {
        rc = recv( connectedSocket, buf, RX_BUFSIZE, 0 );
        
        if( rc==0 ) {
            printf( "Connection was interrupted.\n" );
            break;
        }
    
        if( rc==SOCKET_ERROR ) {
            printf( "Receive failed. Error code: %d\n", WSAGetLastError() );
            break;
        }

        buf[rc] = '\0';
        printf( "Client sent: %s\n", buf );
        // Modify the echo a litte bit to see the difference on client side
        sprintf( buf2, "Received: %s", buf );
        rc = send( connectedSocket, buf2, strlen( buf2 ), 0 );
    }

    // 7) Clean up everything again
    closesocket( acceptSocket );
    closesocket( connectedSocket );
    WSACleanup();
    
    return 0;
}
