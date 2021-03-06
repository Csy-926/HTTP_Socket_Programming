#include "http.h"

// Implementing the request functions needed

http_request create_get_request(string url, map<string, string> headers)
{
    // we just initialize the HTTP request struct and return it
    http_request request;
    request.url = url;
    request.method = GET_REQUEST;
    request.version = HTTP_1;
    request.headers = headers;
    return request;
}

http_request create_post_request(string url, string data, map<string, string> headers)
{
    // we just initialize the HTTP request struct and return it
    http_request request;
    request.url = url;
    request.method = POST_REQUEST;
    request.version = HTTP_1;
    request.headers = headers;
    request.entity_body = data;
    return request;
}

string request_to_string(http_request request)
{
    // Create the request in a single string as is defined in the .H file
    string res;
    // creating the request line
    string request_line = request.method + SPACE + request.url + SPACE + request.version + CARRIAGE_RETURN + LINE_FEED;
    // Creating the header lines
    string headers = "";
    // Iterating through the lines
    map<string, string>::iterator it;
    for (it = request.headers.begin(); it != request.headers.end(); it++)
    {
        headers += it->first + HEADER_SEPARATOR + it->second + CARRIAGE_RETURN + LINE_FEED;
    }
    // Creating the result string
    // Don't forget the empty line :)
    res = request_line + headers + EMPTY_LINE + request.entity_body;
    return res;
}

http_request string_to_request(string request_string)
{
    // splitting the request string to be line by line
    vector<string> request_by_line = split_to_lines(request_string);
    http_request result;
    // using the request line
    vector<string> request_line = split_to_words(request_by_line[0], SPACE);
    result.method = request_line[0];
    result.url = request_line[1];
    result.version = request_line[2];
    // Obtaining the headers
    int i = 1;
    while (request_by_line[i] != EMPTY_LINE)
    {
        vector<string> current_header = split_to_words(request_by_line[i], HEADER_SEPARATOR);
        result.headers[current_header[0]] = current_header[1];
        i++;
    }
    // if there is data keep it
    i++;
    if (i == (int)request_by_line.size() - 1)
    {
        result.entity_body = request_by_line[i];
    }
    return result;
}

// Implementing the response functions needed
http_response create_ok_response(string data, map<string, string> headers)
{
    // we just initialize the HTTP response struct and return it
    http_response response;
    response.version = HTTP_1;
    response.status_code = OK_CODE;
    response.status_message = OK_MSG;
    response.headers = headers;
    response.entity_body = data;
    return response;
}
http_response create_not_found_response(map<string, string> headers)
{
    // we just initialize the HTTP response struct and return it
    http_response response;
    response.version = HTTP_1;
    response.status_code = NOT_FOUND_CODE;
    response.status_message = NOT_FOUND_MSG;
    response.headers = headers;
    return response;
}
string response_to_string(http_response response)
{
    // Create the response in a single string as is defined in the .H file
    string res;
    // creating the status line
    string status_line = response.version + SPACE + response.status_code + SPACE + response.status_message + CARRIAGE_RETURN + LINE_FEED;
    // Creating the header lines
    string headers = "";
    // Iterating through the lines
    map<string, string>::iterator it;
    for (it = response.headers.begin(); it != response.headers.end(); it++)
    {
        headers += it->first + HEADER_SEPARATOR + it->second + CARRIAGE_RETURN + LINE_FEED;
    }
    // Creating the result string
    // Don't forget the empty line :)
    res = status_line + headers + EMPTY_LINE + response.entity_body;
    return res;
}
http_response string_to_response(string response_string){
    // splitting the response string to be line by line
    vector<string> response_by_line = split_to_lines(response_string);
    http_response result;
    // using the request line
    vector<string> status_line = split_to_words(response_by_line[0], SPACE);
    result.version = status_line[0];
    result.status_code = status_line[1];
    result.status_message = status_line[2];
    // Obtaining the headers
    int i = 1;
    while (response_by_line[i] != EMPTY_LINE)
    {
        vector<string> current_header = split_to_words(response_by_line[i], HEADER_SEPARATOR);
        result.headers[current_header[0]] = current_header[1];
        i++;
    }
    // if there is data keep it
    i++;
    if (i == (int)response_by_line.size() - 1)
    {
        result.entity_body = response_by_line[i];
    }
    return result;
}