#include "common.h"
#include "http.h"
#include "parser.h"
#include "file.h"
void test_http();
int main()
{
    // test_http();
    return 0;
}
void test_http()
{
    map<string, string> headers;
    headers["length"] = "15";
    headers["date"] = "current date";
    map<string,string> empty_header;
    string get_r = request_to_string(create_get_request("/test/test1/textfile.txt", headers));
    string post_r = request_to_string(create_post_request("/test/test1/textfile.txt", "some data should be placed here.\nI mean really you should take \t care!", empty_header));
    string ok_r = response_to_string(create_ok_response("some data to response\n.fuckin \thell",headers));
    string not_found_r = response_to_string(create_not_found_response(empty_header));
    cout << "testing get:" << endl
         << get_r;
    cout<< "testing string to request:" <<endl
        << request_to_string(string_to_request(get_r));
    cout << "testing post:" << endl
         << post_r<<endl;
    cout<< "testing string to request:" <<endl
        << request_to_string(string_to_request(post_r))<<endl;
    cout<<"-----------------------------"<<endl;
    cout << "testing ok:" << endl
         << ok_r<<endl;
    cout<< "testing string to response:" <<endl
        << response_to_string(string_to_response(ok_r))<<endl;
    cout << "testing not found:" << endl
         << not_found_r;
    cout<< "testing string to response:" <<endl
        << response_to_string(string_to_response(not_found_r));
}
