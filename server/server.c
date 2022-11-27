#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "server.h"

typedef struct http_request_t http_request_t;

/**
 * HTTP Server
 *
 * @return
 */
int http_server_start(http_server_configuration_t configuration) {
    /**
     * We beginnen met het aanmaken van een socket
     * voordat we de while loop starten zodat
     * de socket beschikbaar is voor de hele
     * server.
     */
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Het aanmaken van de server socket is mislukt.");
        return 1;
    }

    /**
     * De socket configureren.
     */
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    /**
     * Het adres van de socket configureren.
     */
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(configuration.port);

    /**
    * Bind the socket to the address
    *
    */
    if (bind(server_socket, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    /**
     * Listen for connections on the socket.
     *
     */
    if (listen(server_socket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }


    int running = 1;
    while (running) {

        /**
         * Accept a connection on the socket.
         *
         */
        int new_socket = accept(server_socket, (struct sockaddr *) &address, (socklen_t *) &addrlen);
        if (new_socket < 0) {
            perror("accept");
            close(new_socket);
            continue;
        }

        /**
         * Lees het request van de client.
         *
         */
        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);

        /**
         * buffer parsen naar een string
         */
        char *request = malloc(sizeof(char) * strlen(buffer));
        strcpy(request, buffer);


        /**
         * Handle HTTP/1.1 request
         *
         */
        char readBuffer[128];
        char *sendBuffer = "HTTP/1.0 200 OK\r\n"
                           "Content-Type: text/html\r\n"
                           "Content-Length: 30\r\n\r\n"
                           "<html><body>test</body></html>";
        ssize_t status;

        do {
            status = recv(new_socket, readBuffer, sizeof readBuffer, MSG_DONTWAIT);
        } while (status > 0);


        send(new_socket, sendBuffer, (int) strlen(sendBuffer), 0);

    }

    return 0;
}




//
//
//
//    return 0;
//}
