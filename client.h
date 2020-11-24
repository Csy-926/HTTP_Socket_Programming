// Defines the necessary functions and libraries needed for
// implementing the client.
#include "common.h"
#include "http.h"
#include "file.h"

#include <unistd.h>
#include <errno.h>
// The includes needed for socket programming
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
// chosen a random initial port larger than 1024 to prevent any required privelege constraints
#define DEFAULT_PORT "80"
// defines the file at which the client requests are located
#define REQUESTS_PATH "client/commands"
#define CLIENT_GET "client_get"
#define CLIENT_POST "client_post"

// Functions implemented

// returns the file descriptor resulting from this connection 
int connect_to_server(string server_ip, string port_number);
http_response send_request(int server_fd,http_request request);
void handle_response(http_response response, http_request request);