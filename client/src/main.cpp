//
// Created by root on 19-6-27.
//


#include "restclient.h"

int main (int argc, char **argv)
{
    //RestClient::Response r = RestClient::post("http://192.168.5.103:18085/test/get/sayhello", "application/json;charset=utf-8", "{\"key\": \"hellooooooo\"}");
    //添加测试用header头,添加请求头的内容为appkey
    for(;;){
        RestClient::Response r = RestClient::post("http://192.168.5.104:18085/test/get/sayhello", "application/json;charset=utf-8", "701818788","{\"key\": \"heeeee\"}");
    };

}




