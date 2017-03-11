#include "gtest/gtest.h"
#include "ProxyHandler.h"
#include "RequestHandler.h"
#include "Request.h"
#include "request_parser.h"
#include "Response.h"
#include "../nginx-configparser/config_parser.h"

#include <string>

class ProxyHandler_test:public ::testing::Test{
	protected:
		bool configParse(const std::string config) {
			std::stringstream config_stream(config);
			return parser.Parse(&config_stream, &parsedConfig);
		}

		RequestHandler::Status init(std::string prefix){
			return proxy_handler.Init(prefix, parsedConfig);
		}

	    void resetConfig() {
	        parsedConfig = NginxConfig();
	    }
		NginxConfigParser parser;
		NginxConfig parsedConfig;
		ProxyHandler proxy_handler;
};


//Init
TEST_F(ProxyHandler_test, InitTest) {
    configParse("host ucla.edu;\nport 80;");
    RequestHandler::Status status = init("/proxy");
    EXPECT_EQ(status, RequestHandler::OK);
    resetConfig();
}

//Handle a Request
TEST_F(ProxyHandler_test, HandleTest) {
    configParse("host ucla.edu;\nport 80;");
    init("/proxy");
    Request request;
    request.Parse("GET /proxy HTTP/1.1\nHost: localhost:3000\nConnection: keep-alive\nCache-Control: max-age=0");
    Response response;
    EXPECT_EQ(proxy_handler.HandleRequest(request, &response), RequestHandler::OK);
}
