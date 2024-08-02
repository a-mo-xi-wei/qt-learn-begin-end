#include<QCoreApplication>
#include"HttpServer.h"

int main(int argc,char* argv[]){
	QCoreApplication a(argc,argv);
	HttpServer server;
	server.router("/version", [=](const HttpServerRequest& requeset)
		{
			return R"({"version":"1.0.1","name":"weisang server"})";
		});


	server.router("/login", [=](const HttpServerRequest& requeset)
		{
			return R"({"code":"success","message":"登录成功"})";
		});

	server.router("/user_list", [=](const HttpServerRequest& requeset)
		{


			return R"({"code":"success","message":"登录成功"})";
		});
	server.listen(QHostAddress::Any, 8888);
	return a.exec();	
}
