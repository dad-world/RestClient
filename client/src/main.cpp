//
// Created by root on 19-6-27.
//

#include "iostream"
#include "string"
#include "restclient.h"
#include "json/json.h"

int main (int argc, char **argv)
{
    //RestClient::Response r = RestClient::post("http://192.168.5.103:18085/test/get/sayhello", "application/json;charset=utf-8", "{\"key\": \"hellooooooo\"}");
    //封装json函数格式
    Json::Value login;
    login["key"] = "nihao";
    std::string getLogin = login.toStyledString();
    //添加测试用header头,添加请求头的内容为appkey
    RestClient::Response r = RestClient::post("http://192.168.5.105:18085/test/get/sayhello", "application/json;charset=utf-8",getLogin);
    std::cout<<r.code<<std::endl;
    std::cout<<r.body<<std::endl;
    std::map<std::string, std::string>::iterator it;
    it = r.headers.begin();
    for (it;it != r.headers.end();it++)
    {
        std::cout<<it->first<<" "<<it->second<<std::endl;
        it++;

    }

}




