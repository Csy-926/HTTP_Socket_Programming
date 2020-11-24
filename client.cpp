// Implements the client
#include "client.h"

// The main loop is here
int main(int argc, char **argv)
{

    if (argc == 1)
    {
        cout << "You must specify the server IP" << endl;
        exit(1);
    }
    string server_ip = argv[1];
    // The port to which we connect
    string port_number = DEFAULT_PORT;
    // In case a port number was specified we initialize it
    // client is called by ./my_client server_ip port_number
    if (argc > 2)
    {
        port_number = argv[2];
    }
    // Connecting to the server
    int server_fd = connect_to_server(server_ip, port_number);
    // obtaining all the requests from the file to send and receive with the server
    string requests_all = read_file_bin(REQUESTS_PATH);
    // sending each request and receiving the response as specified by the pseudo code
    vector<string> request_each = split_to_words(requests_all, '\n');

    // Empty headers as this client sends no headers
    map<string, string> headers;
    // handle each request and then close the connection
    for (int i = 0; i < (int)request_each.size(); i++)
    {
        string curr_req = request_each[i];
        // splitting the line into parts to process it
        vector<string> request_comp = split_to_words(curr_req, SPACE);
        string request_type = request_comp[0];
        string request_url = request_comp[1];
        // building the request sending it and handling the response
        if (request_type == CLIENT_GET)
        {
            http_request request = create_get_request(request_url, headers);
            http_response response = send_request(server_fd, request);
            handle_response(response, request);
        }
        else if (request_type == CLIENT_POST)
        {
            http_request request = create_post_request(request_url, "Post body data", headers);
            http_response response = send_request(server_fd, request);
            handle_response(response, request);
        }
    }
    // finished all requests so close the connection
    cout << "client shutting down" << endl;
    close(server_fd);
    return 0;
}

// This function sends a request and then receives and returns a response
http_response send_request(int server_fd, http_request request)
{
    http_response response;
    // send the request
    string request_string = request_to_string(request);
    if (send(server_fd, request_string.c_str(), request_string.length(), 0) == -1)
    {
        perror("send_request, send");
        exit(1);
    }
    // Receive the response and return it
    char buf[MAX_DATA_SIZE];
    int num_received;
    if ((num_received = recv(server_fd, buf, MAX_DATA_SIZE - 1, 0)) > 0)
    {
        buf[num_received] = '\0';
        response = string_to_response(string(buf));
    }
    else if (num_received == -1)
    {
        perror("recv");
        close(server_fd);
        exit(1);
    }
    else if (num_received == 0)
    {
        cout << "connection was closed " << server_fd << endl;
        close(server_fd);
        // Exit the child process for now
        exit(0);
    }
    return response;
}

void handle_response(http_response response, http_request request)
{
    // handle not found
    if (response.status_code == NOT_FOUND_CODE)
    {
        cout << "File not found: " << request.url << endl;
    }
    else
    {
        // If response has data write it
        if (response.entity_body.length() > 0)
        {
            cout << "File found: " << request.url << endl;
            vector<string> splitted = split_to_words(request.url, '/');
            string file_path = "client/" + splitted[(int)splitted.size() - 1];
            // In case the index.html was requested
            if(splitted[(int)splitted.size() - 1] == "/")
                file_path += "index.html";
            write_file(file_path, response.entity_body);
        }
    }
}
// This functions starts a TCP connection with the server
int connect_to_server(string server_ip, string port_number)
{
    // The socket file descriptor that we will return
    int server_fd;
    struct addrinfo hints, *server_info, *p;
    // To check if an error occured when obtaining the address info of the server
    int address_status;
    // Will be used to print the IP of server we connected to
    char s[INET_ADDRSTRLEN];
    // Setting the hints structure to zero (it will be used to help in
    // connecting to the server with correct protocols)
    memset(&hints, 0, sizeof hints);
    // IP V4
    hints.ai_family = AF_INET;
    // TCP
    hints.ai_socktype = SOCK_STREAM;

    if ((address_status = getaddrinfo(server_ip.c_str(), port_number.c_str(), &hints, &server_info)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(address_status));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for (p = server_info; p != NULL; p = p->ai_next)
    {
        if ((server_fd = socket(p->ai_family, p->ai_socktype,
                                p->ai_protocol)) == -1)
        {
            perror("connect_to_server, client: socket");
            continue;
        }
        // Try connecting to this socket
        if (connect(server_fd, p->ai_addr, p->ai_addrlen) == -1)
        {
            perror("connect_to_server, client: connect");
            close(server_fd);
            continue;
        }

        break;
    }

    // Check if we weren't able to connect to any socket
    if (p == NULL)
    {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(AF_INET, &(((struct sockaddr_in *)&p)->sin_addr),
              s, sizeof s);
    cout << "client connected to " << s << " at port " << port_number << endl;
    // all done with this structure
    freeaddrinfo(server_info);
    return server_fd;
}
