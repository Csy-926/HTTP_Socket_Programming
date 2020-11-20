// Used to implement the functionalities needed to read from and write to files
// we need to support 
/*
    Text files.
    HTML files.
    IMAGES.
*/
#include "common.h"
#include <iostream>
#include <fstream>

// Function returning a string corresponsing to a read file (text, html, image).
// We should support reading the file in any format using a single function for
// convient usage.
string read_file_bin(string file_path);
// checks if the file with the given path exists
bool file_exists(string file_path); 
void write_file(string file_path, string data);