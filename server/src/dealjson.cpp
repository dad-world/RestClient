//
// Created by root on 19-7-9.
//

#include "dealjson.h"
#include "json/json.h"
#include <string>
#include <iostream>
using namespace std;

std::string DealServerJson::deal_hello(std::string jsonContent)
{
    cout << jsonContent << endl;
    Json::Reader reader;
    Json::Value value;
    //拼接json格式字符串
    Json::Value login;
    login["method"] = "login";
    login["generic"]["user"] = "admin";
    login["generic"]["passwd"] = "123456";
    std::string getLogin = login.toStyledString();

    if (reader.parse(jsonContent,value))
    {
    std::string  strKey =  value["id"].asString();
    cout<< " id is "<<strKey<<endl;
    return getLogin;
    };
};