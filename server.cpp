// Implements the multithreaded server
/*
    first we try the multiprocess approach (offers more portability) 
    then can be changed to multithreading
*/
// Based on the tutorial for socket programming provided by Beejy
#include "server.h"

// The variables used
string port_number = DEFAULT_PORT;

// The main function for running the server
// Should contain an infinite loop where the listener keeps
// listening to the incoming connection requests
int main(int argc, char **argv)
{
    // In case a port number was specified we initialize it
    // Server is called by ./my_server port_number
    if (argc > 1)
    {
        port_number = argv[1];
    }
    cout << "Server running on port " << port_number << endl;
    // file descriptor of the socket at which this server is listening
    int listen_fd = get_server_fd(port_number);
    if (listen_fd == -1)
    {
        fprintf(stderr, "error occured in obtainnig the server fd.\n");
        exit(1);
    }
    // initialize first the signal handler to reap te zombie processes
    reap_zombies();
    cout << "Server started listening on localhost at port number " << port_number << endl;
    cout << "waiting for connections...." << endl;
    handle_connections(listen_fd);
    return 0;
}
// A handler to handle zombie children
// n this way the parent wait for any child processes (pid = -1)
// and while there are zombie process (waitpid() return value is >0)
// it keep looping on calling wait.
void sigchild_handler(int s)
{
    (void)s; // quiet unused variable warning

    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    errno = saved_errno;
}

// Obtain the file descriptor of the socket at which the server is listening
// The server also starts listening on this port number
int get_server_fd(string port_number)
{
    // Initialized with -1 to indicate errors.
    int listen_sockfd = -1;
    // The address info structs
    // hints: the hints provided to obtain the address info of the server
    struct addrinfo hints, *servinfo, *it;
    int error_get_addr;
    // for the workaround
    int yes = 1;
    // initializing hints as specified in the requirements:
    // Local host(my IP),IP V4, TCP
    memset(&hints, 0, sizeof hints);
    // IP V4
    hints.ai_family = AF_INET;
    // Reliable stream (TCP)
    hints.ai_socktype = SOCK_STREAM;
    // Use my IP (Local host)
    hints.ai_flags = AI_PASSIVE;

    // Obtaining the server address info
    if ((error_get_addr = getaddrinfo(NULL, port_number.c_str(), &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error_get_addr));
        return -1;
    }
    // loop through all the results and bind to the first socket we can
    for (it = servinfo; it != NULL; it = it->ai_next)
    {
        // Try obtaining the socket and returning its fd
        // returns -1 for error
        if ((listen_sockfd = socket(it->ai_family, it->ai_socktype,
                                    it->ai_protocol)) == -1)
        {
            // prints a message corresponding to value of errno
            perror("In calling function get_server_fd, server: socket");
            continue;
        }
        // A workaround described by Beejy to overcome the case where address is returned to be in use
        if (setsockopt(listen_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1)
        {
            perror("in calling function get_server_fd, setsockopt");
            // The program exits as port is used
            exit(1);
        }
        // Trying to bind the socket to the port
        if (bind(listen_sockfd, it->ai_addr, it->ai_addrlen) == -1)
        {
            close(listen_sockfd);
            perror("in calling function get_server_fd, server: bind");
            // Try the next socket
            continue;
        }
        // If suitable socket found break
        break;
    }
    // Freeing the structure as it is not needed any more
    freeaddrinfo(servinfo);
    // If no suitable socket was found
    if (it == NULL)
    {
        fprintf(stderr, "server: failed to find a suitable socket to bind\n");
        exit(1);
    }
    // Try listening on this socket
    if (listen(listen_sockfd, BACKLOG) == -1)
    {
        perror("In calling function get_server_fd, listen");
        exit(1);
    }
    return listen_sockfd;
}

void reap_zombies()
{
    struct sigaction s;
    // reap all dead processes using the implemented sigchild handler
    s.sa_handler = sigchild_handler;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &s, NULL) == -1)
    {
        perror("when calling reap_zmbies, sigaction");
        exit(1);
    }
}

// Non-persistent at first then would be updated to be persistent by use of select()
void handle_connections(int listen_fd)
{
    // The fd of the client trying to connect
    int client_fd;
    // keeps the client's address information
    struct sockaddr_storage client_addr;
    socklen_t sin_size = sizeof client_addr;
    while (1)
    {
        // Listener (parent) process would accept a connection if one exists
        // Then it would delegate the hanlding to a child process
        client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &sin_size);
        if (client_fd == -1)
        {
            perror("In handle_connections, accept");
            continue;
        }
        // To obtain the correct byte ordering (endian)
        // consider IPV4 only
        char s[INET_ADDRSTRLEN];
        if (client_addr.ss_family == AF_INET)
        {
            inet_ntop(AF_INET, &(((struct sockaddr_in *)&client_addr)->sin_addr),
                      s, sizeof s);
            cout << "Accepted connection from: " << s << endl;
        }
        else
        {
            cout << "server supports only IPV4" << endl;
            continue;
        }
        // Creating a child process to delegate the work with the accepted connection to it
        if (!fork())
        { // this is the child worker process
            // child doesn't need the listener
            close(listen_fd);
            // Receiving the client request to process it
            int num_received;
            char buf[MAX_DATA_SIZE];
            if ((num_received = recv(client_fd, buf, MAX_DATA_SIZE - 1, 0)) == -1)
            {
                perror("recv");
                exit(1);
            }
            // assuring that the buffer ends with a null character
            buf[num_received] = '\0';
            // parsing the received request to handle it
            // get and post requests are handled
            string request_string = string(buf);
            cout << "received request from " << s << " :" << endl
                 << request_string << endl;
            // handling the request
            http_request request = string_to_request(request_string);
            handle_request(request, client_fd);
            close(client_fd);
            // Exit the child process for now
            exit(0);
        }
        // parent doesn't need this for now connection is non-persistent
        close(client_fd);
    }
}

void handle_request(http_request request, int client_fd)
{
    request.url = handle_spaces(request);
    // get request
    // Send ok response and send the file data if found
    // else send a not found response
    if (request.method == GET_REQUEST)
    {
        string file_path = request.url.substr(1, request.url.length());
        if (file_path.length() == 0)
        {
            file_path = "index.html";
        }
        // may be required
        map<string, string> headers;
        // if file not found
        if (!file_exists(file_path))
        {
            http_response response = create_not_found_response(headers);
            string response_string = response_to_string(response);
            if (send(client_fd, response_string.c_str(), response_string.length(), 0) == -1)
            {
                perror("handle_request, send");
                return;
            }
        }
        // if file exists, send the data in the file
        else
        {
            // creating an OK response with no data still
            http_response response = create_ok_response("", headers);
            int empty_response_length = response_to_string(response).length();
            // reading the file to send it in chuncks
            string file_data = read_file_bin(file_path);
            response.entity_body = file_data;
            // sending the first packet
            string response_string = response_to_string(response);
            int num_bytes;
            if ((num_bytes = send(client_fd, response_string.c_str(), response_string.length(), 0)) == -1)
            {
                perror("handle_request, send");
                return;
            }
            // if the file wasn't completely sent, send the remaining of it
            int sent = num_bytes - empty_response_length;
            while (sent < (int)file_data.length())
            {
                if ((num_bytes = send(client_fd, file_data.substr(sent, file_data.length() - sent).c_str(), file_data.length() - sent, 0)) == -1)
                {
                    perror("handle_request, send");
                    return;
                }
                sent += num_bytes;
            }
        }
    }
    // post request
    // Just send ok response and write received data into the file corresponding to this client
    if (request.method == POST_REQUEST)
    {
        // may be required
        map<string, string> headers;
        // creating an OK response with no data
        http_response response = create_ok_response("", headers);
        string response_string = response_to_string(response);
        if (send(client_fd, response_string.c_str(), response_string.length(), 0) == -1)
        {
            perror("handle_request, send");
            return;
        }
        // writing the request entity body into a file
        string file_path = "socket descriptor " + client_fd;
        write_file(file_path, request.entity_body);
    }
}

string handle_spaces(http_request request)
{
    // handle spaces in request url
    string url_with_space = "";
    for (int i = 0; i < (int)request.url.length(); i++)
    {
        url_with_space += request.url[i];
        if (url_with_space.length() >= 3)
        {
            // replace %20 with space
            if (url_with_space.substr(url_with_space.length() - 3, 3) == "%20")
            {
                url_with_space = url_with_space.substr(0, url_with_space.length() - 3) + " ";
            }
        }
    }
    return url_with_space;
}