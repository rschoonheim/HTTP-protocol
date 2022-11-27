#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 1996

int main(int argc, char const *argv[]) {
    int new_socket, valread;
    int opt = 1;
    char buffer[1024] = {0};

    /**
     * Creating a stream socket
     * AF_INET: IPv4 Internet protocols
     * SOCK_STREAM: Provides sequenced, reliable, two-way, connection-based byte streams.
     * 0: Default protocol (TCP in this case)
     */
    int httpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (httpSocket == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    /**
     * Start listening for connections on
     * port `1996`.
     *
     * `setsockopt` is used to allow the port to be reused
     * immediately after the program exits.
     *
     * fd: The socket file descriptor
     * level: The level at which the option resides
     *    `SOL_SOCKET` for socket options at the socket API level
     * optname: The option to set
     *   `SO_REUSEADDR` to reuse the address
     *   `SO_REUSEPORT` to allow multiple sockets to bind to the same port
     * optval: The value to set the option to
     * optlen: The size of the option value
     */
    setsockopt(httpSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    /**
     * Create the socket address
     *
     * sa_family_t: Address family
     * sin_port: Port number
     * sin_addr: IP address
     */
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);


    /**
     * Bind the socket to the address
     *
     */
    if (bind(httpSocket, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    /**
     * Listen for connections on the socket.
     *
     */
    if (listen(httpSocket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }


    /**
     * Accept a connection on the socket.
     *
     */
    if ((new_socket
                 = accept(httpSocket, (struct sockaddr *) &address,
                          (socklen_t *) &addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read(new_socket, buffer, 1024);

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



    return 0;
}
