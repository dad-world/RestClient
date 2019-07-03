//
// Created by root on 19-7-3.
//

#include "dealjson.h"
#include <string>
#include <iostream>
using namespace std;

int DealJson::deal_test(std::string jsonContent) {
    cout << jsonContent << endl;
    Json::Reader reader;
    Json::Value value;

    if (reader.parse(jsonContent,value))
    {
        std::string  strKey =  value["result"].asString();
        cout<<strKey<<endl;
    };
    return 0;
}