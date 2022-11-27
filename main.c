#include <stdio.h>
#include "server/server.h"

int main(int argc, char const *argv[]) {

    /**
     * We beginnen met het configureren van de HTTP server.
     *
     */
    http_server_configuration_t configuration = {
            "http_server.localhost",
            8081
    };

    /**
     * De HTTP server starten.
     *
     * TODO: moeten we de HTTP server niet starten in een losse thread?
     */
    int result = http_server_start(configuration);
    if (result != 0) {
        printf("HTTP server starten is mislukt.");
        return 1;
    }

    return 0;
}
