#include<QApplication>
#include"MainWindow.h"
#pragma comment(linker,"/subsystem:windows /entry:mainCRTStartup")
int main(int argc,char* argv[]){
	QApplication a(argc,argv);
	MainWindow w;
	w.show();
	return a.exec();	
}