#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")

    #define close_socket(s) closesocket(s)

    static inline void net_init(void) {
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
    }
    static inline void net_cleanup(void) {
        WSACleanup();
    }

#else
    /* Linux / macOS — standard POSIX */
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>

    #define close_socket(s) close(s)

    static inline void net_init(void)    {}
    static inline void net_cleanup(void) {}

#endif

#endif /* PLATFORM_H */
