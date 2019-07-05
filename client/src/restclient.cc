/**
 * @file restclient.cpp
 * @brief implementation of the restclient class
 *
 * This just provides static wrappers around the Connection class REST
 * methods. However since I didn't want to have to pull in a whole URI parsing
 * library just now, the Connection constructor is passed an empty string and
 * the full URL is passed to the REST methods. All those methods to is
 * concatenate them anyways. So this should do for now.
 *
 * @author Daniel Schauenberg <d@unwiredcouch.com>
 */

#include "restclient.h"

#include <curl/curl.h>
//通过查看__cplusplus的值查看编译器的C++标准
//__cplusplus被定义为199711L，而在C++11中，__clpusplus则被定义为201103L
#if __cplusplus >= 201402L
#include <memory>
#endif

#include "version.h"
#include "connection.h"


/**
 * 获取图鸭压缩设备的token值
 *
 * */

std::string RestClient::get_token(const std::string &url, const std::string &data) {
    std::string  ret;
    RestClient::Connection *conn = new RestClient::Connection("");
    conn->AppendHeader("Content-Type", "application/json");
    ret = conn->get_token(url, data);
    return ret;
};


/**
 * @brief global init function. Call this before you start any threads.
 */
int RestClient::init() {
  CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
  if (res == CURLE_OK) {
    return 0;
  } else {
    return 1;
  }
}

/**
 * @brief global disable function. Call this before you terminate your
 * program.
 */
void RestClient::disable() {
  curl_global_cleanup();
}

/**
 * @brief HTTP GET method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::Response RestClient::get(const std::string& url) {
#if __cplusplus >= 201402L
  auto conn = std::make_unique<RestClient::Connection>("");
  return conn->get(url);
#else
  RestClient::Response ret;
  RestClient::Connection *conn = new RestClient::Connection("");
  ret = conn->get(url);
  delete conn;
  return ret;
#endif
}

/**
 * @brief HTTP POST method
 *
 * @param url to query
 * @param ctype content type as string
 * @param data HTTP POST body
 *
 * @return response struct
 */
RestClient::Response RestClient::post(const std::string& url,
                                      const std::string& ctype,
                                      const std::string& data) {
#if __cplusplus >= 201402L
  auto conn = std::make_unique<RestClient::Connection>("");
  conn->AppendHeader("Content-Type", ctype);
  return conn->post(url, data);
#else
  RestClient::Response ret;
  RestClient::Connection *conn = new RestClient::Connection("");
  conn->AppendHeader("Content-Type", ctype);
  ret = conn->post(url, data);
  delete conn;
  return ret;
#endif
}


/**
 * 添加请求头appey认证
 */
RestClient::Response RestClient::post(const std::string& url,
                                      const std::string& ctype,
                                      const std::string& ctypeapykry,
                                      const std::string& data) {
#if __cplusplus >= 201402L
    auto conn = std::make_unique<RestClient::Connection>("");
  conn->AppendHeader("Content-Type", ctype);
  return conn->post(url, data);
#else
    RestClient::Response ret;
    RestClient::Connection *conn = new RestClient::Connection("");
    conn->AppendHeader("AppKey",ctypeapykry);
    conn->AppendHeader("Content-Type", ctype);
    ret = conn->post(url, data);
    delete conn;
    return ret;
#endif
}

/**
 * @brief HTTP PUT method
 *
 * @param url to query
 * @param ctype content type as string
 * @param data HTTP PUT body
 *
 * @return response struct
 */
RestClient::Response RestClient::put(const std::string& url,
                                     const std::string& ctype,
                                     const std::string& data) {
#if __cplusplus >= 201402L
  auto conn = std::make_unique<RestClient::Connection>("");
  conn->AppendHeader("Content-Type", ctype);
  return conn->put(url, data);
#else
  RestClient::Response ret;
  RestClient::Connection *conn = new RestClient::Connection("");
  conn->AppendHeader("Content-Type", ctype);
  ret = conn->put(url, data);
  delete conn;
  return ret;
#endif
}

/**
 * @brief HTTP PATCH method
 *
 * @param url to query
 * @param ctype content type as string
 * @param data HTTP PATCH body
 *
 * @return response struct
 */
RestClient::Response RestClient::patch(const std::string& url,
                                     const std::string& ctype,
                                     const std::string& data) {
  RestClient::Response ret;
  RestClient::Connection *conn = new RestClient::Connection("");
  conn->AppendHeader("Content-Type", ctype);
  ret = conn->patch(url, data);
  delete conn;
  return ret;
}

/**
 * @brief HTTP DELETE method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::Response RestClient::del(const std::string& url) {
#if __cplusplus >= 201402L
  auto conn = std::make_unique<RestClient::Connection>("");
  return conn->del(url);
#else
  RestClient::Response ret;
  RestClient::Connection *conn = new RestClient::Connection("");
  ret = conn->del(url);
  delete conn;
  return ret;
#endif
}

/**
 * @brief HTTP HEAD method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::Response RestClient::head(const std::string& url) {
#if __cplusplus >= 201402L
  auto conn = std::make_unique<RestClient::Connection>("");
  return conn->head(url);
#else
  RestClient::Response ret;
  RestClient::Connection *conn = new RestClient::Connection("");
  ret = conn->head(url);
  delete conn;
  return ret;
#endif
}

/**
 * @brief HTTP OPTIONS method
 *
 * @param url to query
 *
 * @return response struct
 */
RestClient::Response RestClient::options(const std::string& url) {
  RestClient::Response ret;
  RestClient::Connection *conn = new RestClient::Connection("");
  ret = conn->options(url);
  delete conn;
  return ret;
}
