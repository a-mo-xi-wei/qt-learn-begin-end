#include<QApplication>
#include"RemoteFileDlg.h"
int main(int argc,char* argv[]){
	QApplication a(argc,argv);
	qSetMessagePattern("[%{type}] %{function} %{line} %{message}");
	RemoteFileDlg w;
	w.show();
	return a.exec();	
}