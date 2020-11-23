#include "file.h"
// Implementing the functions
string read_file_bin(string file_path)
{

    // oppening the file by using a file stream
    ifstream file(file_path, ios::binary);
    // using a string stream to obtain the read data as a string
    ostringstream ostrm;
    // reading the file in binary format
    ostrm << file.rdbuf();
    // converting to string format
    string content_as_string(ostrm.str());
    file.close();
    return content_as_string;
}

// Uses ifstream to check
bool file_exists(string file_path)
{
    ifstream file(file_path);
    string s;
    bool exists = file.good();
    file.close();
    return exists;
}

// Uses ifstream to write to the file
void write_file(string file_path, string data)
{
    ofstream file(file_path);
    file << data;
    file.close();
}