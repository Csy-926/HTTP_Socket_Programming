// This header defines all the functions, inclusions and functions
// used in implementing the HTTP protocol.
// It also defines the important data types used
#include "parser.h"
// Definitions for request methods
#define GET_REQUEST "GET"
#define POST_REQUEST "POST"
// Definition for response messages
#define OK_MSG "OK"
#define OK_CODE "200"
#define NOT_FOUND_MSG "Not Found"
#define NOT_FOUND_CODE "404"
// Definitions for HTTP version
#define HTTP_0 "HTTP/1.0"
#define HTTP_1 "HTTP/1.1"

// The data structures required

// This structure is for keeping an HTTP response
// HTTP response format:
/*
    version Status_code Status_message
    header field name: value
    ..
    header field name: value

    entity body
*/
typedef struct
{
    // The http version used
    string version;
    // The status code
    string status_code;
    // The status message
    string status_message;
    // The headers as a map mapping header field name to its value
    map<string, string> headers;
    // The data provided in case of a response to a get request
    string entity_body;
} http_response;

// This structure is for keeping an HTTP request
// HTTP request format:
/*
    method URL version
    header field name: value
    ..
    header field name: value

    entity body
*/
typedef struct
{
    // The method in the request line
    // we support GET and POST requests
    string method;
    // The URL in the request line
    string url;
    // The version of the HTTP used (version http/1.1 for this assignment)
    string version;
    // A map mapping each header field to its value
    map<string, string> headers;
    // The data in the request body in case of response
    string entity_body;
} http_request;

// The functions implemented for the requests
http_request create_get_request(string url, map<string, string> headers);
http_request create_post_request(string url, string data, map<string, string> headers);
string request_to_string(http_request request);
http_request string_to_request(string request_string);
// The functions implemented for responses
http_response create_ok_response(string data, map<string, string> headers);
http_response create_not_found_response(map<string, string> headers);
string response_to_string(http_response response);
http_response string_to_response(string response_string);