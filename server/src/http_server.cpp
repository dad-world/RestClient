#include <utility>
#include "http_server.h"
#include "dealjson.h"
#include "json/json.h"
void HttpServer::Init(const std::string &port)
{
	m_port = port;
	s_server_option.enable_directory_listing = "yes";
	s_server_option.document_root = s_web_dir.c_str();

	// ����http����

	// ���� CORS������ֻ�����ҳ������Ч
	// s_server_option.extra_headers = "Access-Control-Allow-Origin: *";
}

bool HttpServer::Start()
{
	mg_mgr_init(&m_mgr, NULL);
	mg_connection *connection = mg_bind(&m_mgr, m_port.c_str(), HttpServer::OnHttpWebsocketEvent);
	if (connection == NULL)
		return false;
	// for both http and websocket
	mg_set_protocol_http_websocket(connection);

	printf("starting http server at port: %s\n", m_port.c_str());
	// loop
	while (true)
		mg_mgr_poll(&m_mgr, 500); // ms

	return true;
}

void HttpServer::OnHttpWebsocketEvent(mg_connection *connection, int event_type, void *event_data)
{
	// ����http��websocket
	if (event_type == MG_EV_HTTP_REQUEST)
	{
		http_message *http_req = (http_message *)event_data;
		HandleHttpEvent(connection, http_req);
	}
	else if (event_type == MG_EV_WEBSOCKET_HANDSHAKE_DONE ||
		     event_type == MG_EV_WEBSOCKET_FRAME ||
		     event_type == MG_EV_CLOSE)
	{
		websocket_message *ws_message = (struct websocket_message *)event_data;
		HandleWebsocketMessage(connection, event_type, ws_message);
	}
}

// ---- simple http ---- //
static bool route_check(http_message *http_msg, char *route_prefix)
{
	if (mg_vcmp(&http_msg->uri, route_prefix) == 0)
		return true;
	else
		return false;

	// TODO: �������ж� GET, POST, PUT, DELTE�ȷ���
	//mg_vcmp(&http_msg->method, "GET");
	//mg_vcmp(&http_msg->method, "POST");
	//mg_vcmp(&http_msg->method, "PUT");
	//mg_vcmp(&http_msg->method, "DELETE");
}

void HttpServer::AddHandler(const std::string &url, ReqHandler req_handler)
{
	if (s_handler_map.find(url) != s_handler_map.end())
		return;

	s_handler_map.insert(std::make_pair(url, req_handler));
}

void HttpServer::RemoveHandler(const std::string &url)
{
	auto it = s_handler_map.find(url);
	if (it != s_handler_map.end())
		s_handler_map.erase(it);
}

void HttpServer::SendHttpRsp(mg_connection *connection, std::string rsp)
{
	// --- δ����CORS
	// �����ȷ���header, ��ʱ��������HTTP/2.0
	mg_printf(connection, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
	// ��json��ʽ����
	mg_printf_http_chunk(connection,rsp.c_str());
	// ���Ϳհ��ַ��죬������ǰ��Ӧ
	mg_send_http_chunk(connection, "", 0);

	// --- ����CORS
	/*mg_printf(connection, "HTTP/1.1 200 OK\r\n"
			  "Content-Type: text/plain\n"
			  "Cache-Control: no-cache\n"
			  "Content-Length: %d\n"
			  "Access-Control-Allow-Origin: *\n\n"
			  "%s\n", rsp.length(), rsp.c_str()); */
}

void HttpServer::HandleHttpEvent(mg_connection *connection, http_message *http_req)
{
	std::string req_str = std::string(http_req->message.p, http_req->message.len);
	//req_str 请求头信息
	//输出请求头信息
	printf("got request: %s\n", req_str.c_str());
	//获取请求头参数
    std::string header_names = std::string(http_req->header_names[0].p,http_req->header_names[0].len);
    std::string header_values = std::string(http_req->header_values[0].p,http_req->header_values[0].len);
    std::string header_values1 = std::string(http_req->header_names[1].p,http_req->header_names[1].len);
    std::string header_values2 = std::string(http_req->header_values[1].p,http_req->header_values[1].len);

	//uri.p 请求的方法
	std::string url = std::string(http_req->uri.p, http_req->uri.len);
	//body中含有传入的json参数
	std::string body = std::string(http_req->body.p, http_req->body.len);
	auto it = s_handler_map.find(url);
	if (it != s_handler_map.end())
	{
		ReqHandler handle_func = it->second;
		handle_func(url, body, connection, &HttpServer::SendHttpRsp);
	}

	// ��������
	if (route_check(http_req, "/")) // index page
		mg_serve_http(connection, http_req, s_server_option);
	else if (route_check(http_req, "/api/hello")) 
	{
		//对请求作储处理
		DealServerJson dealserverjson;
		std::string backbody = dealserverjson.deal_hello(body);
		//拼接json格式字符串,响应头
		SendHttpRsp(connection, backbody);
	}
	else if (route_check(http_req, "/api/sum"))
	{
		// ��post���󣬼ӷ��������
		char n1[100], n2[100];
		double result;

		/* Get form variables */
		mg_get_http_var(&http_req->body, "n1", n1, sizeof(n1));
		mg_get_http_var(&http_req->body, "n2", n2, sizeof(n2));

		/* Compute the result and send it back as a JSON object */
		result = strtod(n1, NULL) + strtod(n2, NULL);
		SendHttpRsp(connection, std::to_string(result));
	}
	else
	{
		mg_printf(
			connection,
			"%s",
			"HTTP/1.1 501 Not Implemented\r\n" 
			"Content-Length: 0\r\n\r\n");
	}
}

// ---- websocket ---- //
int HttpServer::isWebsocket(const mg_connection *connection)
{
	return connection->flags & MG_F_IS_WEBSOCKET;
}

void HttpServer::HandleWebsocketMessage(mg_connection *connection, int event_type, websocket_message *ws_msg)
{
	if (event_type == MG_EV_WEBSOCKET_HANDSHAKE_DONE)
	{
		printf("client websocket connected\n");
		// ��ȡ���ӿͻ��˵�IP�Ͷ˿�
		char addr[32];
		mg_sock_addr_to_str(&connection->sa, addr, sizeof(addr), MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
		printf("client addr: %s\n", addr);

		// ��� session
		s_websocket_session_set.insert(connection);

		SendWebsocketMsg(connection, "client websocket connected");
	}
	else if (event_type == MG_EV_WEBSOCKET_FRAME)
	{
		mg_str received_msg = {
			(char *)ws_msg->data, ws_msg->size
		};

		char buff[1024] = {0};
		strncpy(buff, received_msg.p, received_msg.len); // must use strncpy, specifiy memory pointer and length

		// do sth to process request
		printf("received msg: %s\n", buff);
		SendWebsocketMsg(connection, "send your msg back: " + std::string(buff));
		//BroadcastWebsocketMsg("broadcast msg: " + std::string(buff));
	}
	else if (event_type == MG_EV_CLOSE)
	{
		if (isWebsocket(connection))
		{
			printf("client websocket closed\n");
			// �Ƴ�session
			if (s_websocket_session_set.find(connection) != s_websocket_session_set.end())
				s_websocket_session_set.erase(connection);
		}
	}
}

void HttpServer::SendWebsocketMsg(mg_connection *connection, std::string msg)
{
	mg_send_websocket_frame(connection, WEBSOCKET_OP_TEXT, msg.c_str(), strlen(msg.c_str()));
}

void HttpServer::BroadcastWebsocketMsg(std::string msg)
{
	for (mg_connection *connection : s_websocket_session_set)
		mg_send_websocket_frame(connection, WEBSOCKET_OP_TEXT, msg.c_str(), strlen(msg.c_str()));
}

bool HttpServer::Close()
{
	mg_mgr_free(&m_mgr);
	return true;
}