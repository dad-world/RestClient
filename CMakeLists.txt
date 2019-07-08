cmake_minimum_required(VERSION 3.13)

project(httpclient)

SET(CMAKE_CXX_STANDARD 11)

#设置头文件目录
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/3rdparty/include)

# 设置链接库名称
SET(LINK_LIB_LIST jsoncpp curl)
# 设置链接库目录
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/3rdparty/lib)

ADD_SUBDIRECTORY(client)
ADD_SUBDIRECTORY(server)