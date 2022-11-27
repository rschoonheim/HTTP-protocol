#ifndef HTTP_SOCKET_SERVER_H
#define HTTP_SOCKET_SERVER_H

typedef struct HTTP_SERVER_CONFIGURATION_STRUCT {
    char *host;
    int port;
} http_server_configuration_t;

extern int http_server_start(http_server_configuration_t configuration);

#endif //HTTP_SOCKET_SERVER_H
