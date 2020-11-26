// Defines the necessary functions and libraries needed for 
// implementing the multithreaded server.
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
// The includes needed for multiprocessing
#include <sys/wait.h>
#include <signal.h>
// chosen a random initial port larger than 1024 to prevent any required privelege constraints
#define DEFAULT_PORT "2000"
#define BACKLOG 100 // how many pending connections queue will hold

// Functions implemented

// A handler for children processes that just clears zombie processes without blocking.
void sigchild_handler(int s);
// Returns the socket file descriptor of the socket at which the server is listening
int get_server_fd(string port_number);
// Sets the signal child handler to reap zombie children
void reap_zombies();
// The main infinite loop where the server is handling the incoming connections and requests
void handle_connections(int listen_fd);
// functions that handles a received request
void handle_request(http_request request, int client_fd);
// handles spaces in url
string handle_spaces(http_request request);
// obtains the timeout value and changes it according to number of active connections
float change_timeout(int current_timeout,int number_connections);