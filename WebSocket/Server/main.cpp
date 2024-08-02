#include<QApplication>
#include"Server.h"

int main(int argc,char* argv[]){
	QApplication a(argc,argv);
	Server server;
	server.start(QHostAddress::Any,8888);
	return a.exec();	
}