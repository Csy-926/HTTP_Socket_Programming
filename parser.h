#include "common.h"
// This function takes a request string and splits it into components (line + headers + data)
vector<string> split_to_lines(string request);
// This function takes the string and splits it into words
vector<string> split_to_words(string line,char delim);