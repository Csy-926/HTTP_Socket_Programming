#include "common.h"
#include "http.h"
#include "parser.h"
void test_http();
int main()
{
    test_http();
    return 0;
}
void test_http()
{
    map<string, string> headers;
    headers["length"] = "15";
    headers["date"] = "current date";
    string get_r = request_to_string(create_get_request("/test/test1/textfile.txt", headers));
    string post_r = request_to_string(create_post_request("/test/test1/textfile.txt", "some data should be placed here.\nI mean really you should take \t care!", headers));
    cout << "testing get:" << endl
         << get_r;
    cout<< "testing string to request:" <<endl
        << request_to_string(string_to_request(get_r));
    cout << "testing post:" << endl
         << post_r<<endl;
    cout<< "testing string to request:" <<endl
        << request_to_string(string_to_request(post_r))<<endl;
    // vector<string> request_string = split_to_lines(request_to_string(create_post_request("/test/test1/textfile.txt", "some data should be placed here.\nI mean really you should take \t care!", headers)));
    // for (int i = 0; i < request_string.size(); i++)
    // {
    //     cout << "line" << i << ": " << request_string[i] <<" length: " <<request_string[i].length()<< endl;
    //     vector<string> words = split_to_words(request_string[i],HEADER_SEPARATOR);
    //     for(int j = 0;j < words.size();j++){
    //         cout<<"word"<<j<<": "<<words[j] << " length: "<<words[j].length()<<endl;
    //     }
    // }
}
