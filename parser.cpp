#include "parser.h"
vector<string> split_to_lines(string request)
{
    int request_length = request.length();
    // We split by finding cr and lf
    // finding 2 cr lf after each other corresponds to the empty line
    int string_begining = 0;
    vector<string> result;
    for (int i = 1; i < request_length; i++)
    {
        // The case where a line feed and a carriage return was obtained
        if (request[i - 1] == CARRIAGE_RETURN && request[i] == LINE_FEED)
        {
            // The case where they represent the empty line
            if (string_begining == i - 1)
            {
                 result.push_back(EMPTY_LINE);
                result.push_back(request.substr(i + 1, request_length - i));
                break;
            }
            // other wise push the line and advance the beggining
            result.push_back(request.substr(string_begining, i - string_begining - 1));
            string_begining = i + 1;
        }
    }
    return result;
}
vector<string> split_to_words(string line, char delim){
    int length = line.length();
    // We split by finding SPACE
    int string_begining = 0;
    vector<string> result;
    for (int i = 1; i < length; i++)
    {
        // The case where a line feed and a carriage return was obtained
        if (line[i] == delim)
        {
            // push the word and advance the beggining
            result.push_back(line.substr(string_begining, i - string_begining));
            string_begining = i + 1;
        }
    }
    result.push_back(line.substr(string_begining, length - string_begining));
    return result;
}