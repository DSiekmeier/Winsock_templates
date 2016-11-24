/**
 * \brief       A simple template for a TCP client using Winsock
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

#define BUFSIZE     256
#define PORT        15123
#define ADDRESS     "127.0.0.1"

// Initialize and define Winsock version which should be used
int startWinsock( void ) {
    WSADATA wsa;
    return WSAStartup( MAKEWORD(2,0), &wsa );
}

int main( void ) {
    long rc;
    SOCKET s;
    SOCKADDR_IN addr;
    char buf[BUFSIZE];
    
    // 1) Call the initialization function
    rc = startWinsock();
    if( rc!=0 ) {
        printf( "Initialization failed. Error code: %ld\n", rc );
        return 1;
    }

    // 2) Create the socket
    s = socket( AF_INET, SOCK_STREAM, 0 );
    if( s==INVALID_SOCKET ) {
        // WSAGetLastError() for more information
        printf( "Socket creation failed. Error code: %d\n", WSAGetLastError() );
        return 1;
    }

    // 3) Connect to the server
    memset( &addr, 0, sizeof( SOCKADDR_IN ) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons( PORT );
    addr.sin_addr.s_addr = inet_addr( ADDRESS );

    rc = connect( s, (SOCKADDR*)&addr, sizeof( SOCKADDR ) );
    if( rc == SOCKET_ERROR ) {
        printf( "Connection failed. Error code: %d\n", WSAGetLastError() );
        return 1;
    }

    // 4) Exchange data
    while( rc!=SOCKET_ERROR ) {
        printf( "\nEnter string: " );
        fgets( buf, BUFSIZE, stdin );
        send( s, buf, strlen( buf ), 0 );
        rc = recv( s, buf, BUFSIZE, 0 );
        
        if( rc==0 ) {
            printf( "Connection was interrupted.\n" );
            break;
        }
        
        if( rc==SOCKET_ERROR ) {
            printf( "Receive failed. Error code: %d\n", WSAGetLastError() );
            break;
        }

        buf[rc] = '\0';
        printf( "Server echo\t: %s", buf );
    }
    
    // 5) Clean up everything again
    closesocket( s );
    WSACleanup();
    
    return 0;
}
