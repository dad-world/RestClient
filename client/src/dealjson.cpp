//
// Created by root on 19-7-3.
//

#include "dealjson.h"
#include <string>
#include <iostream>
using namespace std;

int  DealJson::deal_test(std::string jsonContent) {
    cout << jsonContent << endl;
    Json::Reader reader;
    Json::Value value;

    if (reader.parse(jsonContent,value))
    {
        std::string  strKey =  value["token"].asString();
        cout<<strKey<<endl;
    };
    return 0;
}

std::string DealJson::get_token(std::string jsonContent){
    cout << jsonContent << endl;
    Json::Reader reader;
    Json::Value value;

    if (reader.parse(jsonContent,value))
    {
        std::string  strKey =  value["token"].asString();
        cout<< "tuyashebei  id is "<<strKey<<endl;
        return strKey;
    };
}